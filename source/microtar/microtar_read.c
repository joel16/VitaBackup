#include "fs.h"
#include "log.h"
#include "menu_options.h"
#include "microtar.h"
#include "microtar_read.h"
#include "progress_bar.h"
#include "utils.h"

static mtar_t tar;

static SceInt MicrotarRead_ExtractFileFromTarRec(char *dst)
{
	SceInt ret = 0, written = 0;
	mtar_header_t header;

	// Read Success
	while ((mtar_read_header(&tar, &header)) != MTAR_ENULLRECORD)
	{
		if (header.type == MTAR_TDIR)
		{
			DEBUG_PRINT("Header name: %s\n\n", header.name);
			FS_RecursiveMakeDir(header.name);
		}
		else
		{
			char *data = calloc(1, header.size + 1);
			if (R_FAILED(ret = mtar_read_data(&tar, data, header.size)))
				return ret;

			DEBUG_PRINT("Header name: %s\nHeader size: %d\n\n", header.name, header.size);
			ProgressBar_DisplayProgress("Restoring in progress...", Utils_Basename(header.name), written, header.size);
			written = FS_WriteFile(header.name, data, header.size + 1);
			free(data);
		}

		mtar_next(&tar);
	}

	return 0;
}

SceInt MicrotarRead_ExtractTar(char *src, char *dst)
{
	/* Open archive for writing */
	mtar_open(&tar, src, "r");

	MicrotarRead_ExtractFileFromTarRec(dst);

	mtar_finalize(&tar); // Finalize archive
	mtar_close(&tar); // Close archive
}