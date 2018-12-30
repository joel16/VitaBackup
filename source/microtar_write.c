#include "fs.h"
#include "log.h"
#include "menu_options.h"
#include "microtar.h"
#include "microtar_write.h"
#include "miniz.h"
#include "progress_bar.h"
#include "utils.h"

static mtar_t tar;

static SceInt MicrotarWrite_AddFileToTar(char *src) {
	SceInt ret = 0;

	SceInt chunksize = (512 * 1024); // Chunk size
	char *buffer = (char *)malloc(chunksize); // Reading buffer

	size_t totalWritten = 0, totalRead = 0; 

	SceOff size = FS_GetFileSize(src); // Get input file size

	SceInt in = 0;

	// Opened file for reading
	if (R_SUCCEEDED(in = sceIoOpen(src, SCE_O_RDONLY, 0))) { // Open file for reading
		size_t bytesRead = 0; // Read byte count
		mtar_write_file_header(&tar, src, size);

		// Copy loop (512KB at a time)
		while((bytesRead = sceIoRead(in, buffer, chunksize)) > 0) {
			totalRead += bytesRead; // Accumulate read data
			totalWritten += mtar_write_data(&tar, buffer, bytesRead); // Write data
		
			ProgressBar_DisplayProgress("Backup in progress...", Utils_Basename(src), totalRead, size);
		}

		if (totalRead != totalWritten) // Insufficient copy
			ret = -3;
			
		if (R_FAILED(ret = sceIoClose(in))) {
			DEBUG_PRINT("sceIoClose(in) failed: 0x%lx\n\n", ret);
			free(buffer);
			return ret;
		}
	}
	else {
		DEBUG_PRINT("in = sceIoOpen(src, SCE_O_RDONLY, 0) failed: 0x%lx\n\n", in);
		free(buffer);
		return in;
	}
	
	free(buffer); // Free memory
	return ret; // Return result
}

static SceInt MicrotarWrite_AddFileToTarRec(char *src) {
	SceInt dir = 0;
	
	if (R_SUCCEEDED(dir = sceIoDopen(src))) {
		mtar_write_dir_header(&tar, src);
		SceInt ret = 1;

		while (ret > 0) {
			SceIoDirent entry;
			memset(&entry, 0, sizeof(entry));

			if (R_SUCCEEDED(ret = sceIoDread(dir, &entry))) {
				if (strlen(entry.d_name) > 0) {
					// Calculate Buffer Size
					int insize = strlen(src) + strlen(entry.d_name) + 2;

					char *inbuffer = (char *)malloc(insize);

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

		if (R_FAILED(ret = sceIoDclose(dir))) {
			DEBUG_PRINT("ret = sceIoDclose(dir) failed: 0x%lx\n\n", ret);
			return ret;
		}
	}
	else {
		DEBUG_PRINT("dir = sceIoDopen(src) failed: 0x%lx\n\n", dir);
		return dir;
	}

	return 0;
}

SceInt MicrotarWrite_AddToTar(char *src, SceInt compression) {
	SceDateTime time;
	sceRtcGetCurrentClockLocalTime(&time);

	char *path = malloc(256);
	char *compressed_path = malloc(256);
	char *dateStr = malloc(24);
	char *basename = malloc(128);

	snprintf(basename, 128, "%s.tar", Utils_Basename(src));

	Utils_GetDateString(dateStr, 0, time, SCE_FALSE);
	snprintf(path, 256, "%s:/data/VitaBackup/backups/%s-%s.tar", storage_location == SCE_FALSE? "ux0" : "ur0", 
		Utils_RemoveExt(Utils_Basename(src)), dateStr);

	if (FS_FileExists(path))
		FS_RemoveFile(path); // Delete output file (if existing)

	/* Open archive for writing */
	mtar_open(&tar, path, "w");

    SceIoStat stat;
	SceInt ret = 0;

	if (R_FAILED(ret = sceIoGetstat(src, &stat))) {
		free(path);
		free(compressed_path);
		free(dateStr);
		free(basename);
		return ret;
	}

	if (SCE_S_ISDIR(stat.st_mode))
		MicrotarWrite_AddFileToTarRec(src);
	else 
		MicrotarWrite_AddFileToTar(src);

	mtar_finalize(&tar); // Finalize archive
	mtar_close(&tar); // Close archive

	mz_zip_archive zip_archive = {0};
    mz_bool status = MZ_FALSE;
	memset(&zip_archive, 0, sizeof(zip_archive));

	snprintf(compressed_path, 256, "%s:/data/VitaBackup/backups/%s-%s.tar.zip", storage_location == SCE_FALSE? "ux0" : "ur0", 
		Utils_RemoveExt(Utils_Basename(src)), dateStr);

	status = mz_zip_writer_init_file(&zip_archive, compressed_path, 0);
	if (!status) {
		DEBUG_PRINT("mz_zip_writer_init fail");
		free(path);
		free(compressed_path);
		free(dateStr);
		free(basename);
		return -1;
	}

	status = mz_zip_writer_add_file(&zip_archive, basename, path, NULL, 0, (mz_uint)compression);
	if (!status) {
		DEBUG_PRINT("mz_zip_writer_add_file(basename:%s path:%s) fail", basename, path);
		free(path);
		free(compressed_path);
		free(dateStr);
		free(basename);
		return -1;
	}

	status = mz_zip_writer_finalize_archive(&zip_archive);
	if (!status) {
		DEBUG_PRINT("mz_zip_writer_finalize_archive fail");
		free(path);
		free(compressed_path);
		free(dateStr);
		free(basename);
		return -1;
	}

	status = mz_zip_writer_end(&zip_archive);
	if (!status) {
		DEBUG_PRINT("mz_zip_writer_end fail");
		free(path);
		free(compressed_path);
		free(dateStr);
		free(basename);
		return -1;
	}

	if (R_FAILED(ret = FS_RemoveFile(path))) {
		DEBUG_PRINT("FS_RemoveFile %s failed 0x%lx\n", path, ret);
		free(path);
		free(compressed_path);
		free(dateStr);
		free(basename);
		return -1;
	}

	free(path);
	free(compressed_path);
	free(dateStr);
	free(basename);
	return 0;
}
