#include "fs.h"
#include "log.h"
#include "menu_options.h"
#include "microtar.h"
#include "microtar_read.h"
#include "miniz.h"
#include "progress_bar.h"
#include "utils.h"

static mtar_t tar;

static SceInt MicrotarRead_ExtractFileFromTarRec(char *dst, SceOff size) {
	SceInt ret = 0;
	mtar_header_t header;

	// Read Success
	while ((mtar_read_header(&tar, &header)) != MTAR_ENULLRECORD) {
		if (header.type == MTAR_TDIR) {
			//DEBUG_PRINT("Dir name: %s\npos: %llu\nremaining_data: %llu\n\n", header.name, tar.pos, tar.remaining_data);
			FS_RecursiveMakeDir(header.name);
		}
		else {
			char *data = calloc(1, header.size + 1);
			if (R_FAILED(ret = mtar_read_data(&tar, data, header.size))) {
				DEBUG_PRINT("mtar_read_data failed: 0x%lx\n\n", ret);
				return ret;
			}

			//DEBUG_PRINT("File name: %s\nFile size: %d\npos: %llu\nremaining_data: %llu\n\n", header.name, header.size, tar.pos, tar.remaining_data);
			ProgressBar_DisplayProgress("Restore in progress...", Utils_Basename(header.name), tar.pos, size);
			FS_WriteFile(header.name, data, header.size + 1);
			free(data);
		}

		mtar_next(&tar);
	}

	return 0;
}

SceInt MicrotarRead_ExtractTar(char *src, char *dst) {
	SceInt ret = 0;
	mz_zip_archive zip_archive = {0};
    mz_bool status = MZ_FALSE;
	memset(&zip_archive, 0, sizeof(zip_archive));

	char *path = malloc(541);

	status = mz_zip_reader_init_file(&zip_archive, src, 0);
	if (!status) {
		DEBUG_PRINT("mz_zip_reader_init_file fail");
		free(path);
		return -1;
	}

	for (int i = 0; i < mz_zip_reader_get_num_files(&zip_archive); i++) {
		mz_zip_archive_file_stat file_stat;

		status = mz_zip_reader_file_stat(&zip_archive, i, &file_stat);
		if (!status) {
			DEBUG_PRINT("mz_zip_reader_file_stat fail");
			mz_zip_reader_end(&zip_archive);
			free(path);
			return -1;
		}

		snprintf(path, 541, "%s:/data/VitaBackup/backups/%s", storage_location == SCE_FALSE? "ux0" : "ur0", file_stat.m_filename);
		
		if (!mz_zip_reader_extract_to_file(&zip_archive, i, path, MZ_NO_COMPRESSION)) {
			DEBUG_PRINT("mz_zip_reader_extract_to_file: %s failed.\n", path);
			free(path);
			return -1;
		}
	}

	status = mz_zip_reader_end(&zip_archive);
	if (!status) {
		DEBUG_PRINT("mz_zip_reader_end fail");
		free(path);
		return -1;
	}

	SceOff tarfile_size = FS_GetFileSize(path); // Get input file size

	/* Open archive for writing */
	mtar_open(&tar, path, "r");

	MicrotarRead_ExtractFileFromTarRec(dst, tarfile_size);

	mtar_finalize(&tar); // Finalize archive
	mtar_close(&tar); // Close archive

	if (R_FAILED(ret = FS_RemoveFile(path))) {
		DEBUG_PRINT("FS_RemoveFile %s failed 0x%lx\n", path, ret);
		free(path);
		return -1;
	}

	memset(path, 0, strlen(path));
	free(path);
	return 0;
}
