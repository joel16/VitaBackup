#include "fs.h"
#include "menu_options.h"
#include "microtar.h"
#include "microtar_write.h"
#include "progress_bar.h"
#include "utils.h"

static mtar_t tar;

static SceInt MicrotarWrite_AddFileToTar(char *src)
{
	SceInt ret = 0;

	SceInt chunksize = (512 * 1024); // Chunk size
	char *buffer = (char *)malloc(chunksize); // Reading buffer

	size_t totalWritten = 0, totalRead = 0; 

	SceOff size = FS_GetFileSize(src); // Get input file size

	SceInt in = 0;

	// Opened file for reading
	if (R_SUCCEEDED(in = sceIoOpen(src, SCE_O_RDONLY, 0))) // Open file for reading
	{
		size_t bytesRead = 0; // Read byte count
		mtar_write_file_header(&tar, src, size);

		// Copy loop (512KB at a time)
		while((bytesRead = sceIoRead(in, buffer, chunksize)) > 0)
		{
			totalRead += bytesRead; // Accumulate read data
			totalWritten += mtar_write_data(&tar, buffer, bytesRead); // Write data
		
			ProgressBar_DisplayProgress("Backing up...", Utils_Basename(src), totalRead, size);
		}

		if (totalRead != totalWritten) // Insufficient copy
			ret = -3;
			
		if (R_FAILED(ret = sceIoClose(in)))
		{
			free(buffer);
			return ret;
		}
	}
	else
	{
		free(buffer);
		return in;
	}
	
	free(buffer); // Free memory
	return ret; // Return result
}

static SceInt MicrotarWrite_AddFileToTarRec(char *src)
{
	int dir = 0;
	
	if (R_SUCCEEDED(dir = sceIoDopen(src)))
	{
		mtar_write_dir_header(&tar, src);
		SceInt ret = 1;

		while (ret > 0)
		{
			SceIoDirent entry;
			memset(&entry, 0, sizeof(entry));

			if (R_SUCCEEDED(ret = sceIoDread(dir, &entry)))
			{
				if (strlen(entry.d_name) > 0)
				{
					// Calculate Buffer Size
					int insize = strlen(src) + strlen(entry.d_name) + 2;

					char * inbuffer = (char *)malloc(insize);

					// Puzzle Input Path
					strcpy(inbuffer, src);
					inbuffer[strlen(inbuffer) + 1] = 0;
					inbuffer[strlen(inbuffer)] = '/';
					strcpy(inbuffer + strlen(inbuffer), entry.d_name);

					// Another Folder
					if (SCE_S_ISDIR(entry.d_stat.st_mode))
						MicrotarWrite_AddFileToTarRec(inbuffer); // Copy Folder (via recursion)

					// Simple File
					else 
						MicrotarWrite_AddFileToTar(inbuffer);

					// Free Buffer
					free(inbuffer);
				}
			}
			else
				return ret;
		}

		if (R_FAILED(ret = sceIoDclose(dir)))
			return ret;
	}
	else
		return dir;

	return 0;
}

SceInt MicrotarWrite_AddToTar(char *src)
{
	char * path = (char *)malloc(256);
	snprintf(path, 256, "%s:/data/VitaBackup/backups/%s.tar", storage_location == SCE_FALSE? "ux0" : "ur0", Utils_RemoveExt(Utils_Basename(src)));

	if (FS_FileExists(path))
		FS_RemoveFile(path); // Delete output file (if existing)

	/* Open archive for writing */
	mtar_open(&tar, path, "w");

    SceIoStat stat;
	SceInt ret = 0;

	if (R_FAILED(ret = sceIoGetstat(src, &stat)))
	{
		free(path);
		return ret;
	}

	if (SCE_S_ISDIR(stat.st_mode))
		MicrotarWrite_AddFileToTarRec(src);
	else 
		MicrotarWrite_AddFileToTar(src);

	free(path);
	mtar_finalize(&tar); // Finalize archive
	mtar_close(&tar); // Close archive
}