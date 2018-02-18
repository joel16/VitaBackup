#include <stdio.h>
#include <string.h>

#include "fs.h"
#include "progress_bar.h"
#include "utils.h"

SceBool FS_FileExists(const char *path)
{
	SceUID file = 0;
	
	if (R_SUCCEEDED(file = sceIoOpen(path, SCE_O_RDONLY, 0777)))
	{
		sceIoClose(file);
		return SCE_TRUE;
	}
	
	return SCE_FALSE;
}

SceBool FS_DirExists(const char *path)
{	
	SceUID dir = 0;
	
	if (R_SUCCEEDED(dir = sceIoDopen(path)))
	{
		sceIoDclose(dir);
		return SCE_TRUE;
	}
	
	return SCE_FALSE;
}

char *FS_Gets(char *str, SceInt n, SceUID stream) 
{
	SceInt i = 0, bytes = 0;
	
	while((i < n) && (bytes = sceIoRead(stream, str + i, 1)) == 1)
	{
		if (str[i] == -1 || str[i] == '\n')
			break;
		
		i ++;
	}
	
	str[i] = 0;
	
	if (bytes != 1 && i == 0)
		return NULL;

	return str;
}

SceInt FS_ReadFile(char *path, SceVoid *buf, SceInt size) 
{
	SceUID file = 0;

	if (R_SUCCEEDED(file = sceIoOpen(path, SCE_O_RDONLY, 0)))
	{
		SceInt read = sceIoRead(file, buf, size);
		sceIoClose(file);
		return read;
	}
	
	return file;
}

SceInt FS_WriteFile(char *path, SceVoid *buf, SceInt size) 
{	
	SceUID file = 0;
	
	if (R_SUCCEEDED(file = sceIoOpen(path, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777)))
	{
		SceInt written = sceIoWrite(file, buf, size);
		sceIoClose(file);
		return written;
	}
		
	return file;
}

SceInt FS_MakeDir(const char *path)
{
	SceInt ret = 0;

	if (R_FAILED(ret = sceIoMkdir(path, 0777)))
		return ret;

	return 0;
}

SceVoid FS_RecursiveMakeDir(const char *path) 
{
	char tmp[256];
	char *p = NULL;
	size_t len;

	snprintf(tmp, sizeof(tmp), "%s", path);
	len = strlen(tmp);

	if (tmp[len - 1] == '/')
		tmp[len - 1] = 0;

	for (p = tmp + 1; *p; p++)
	{
		if (*p == '/') 
		{
			*p = 0;
			FS_MakeDir(tmp);
			*p = '/';
		}
		FS_MakeDir(tmp);
	}
}


SceInt FS_RemoveFile(const char *path)
{
	SceInt ret = 0;

	if (R_FAILED(ret = sceIoRemove(path)))
		return ret;

	return 0;
}

SceInt FS_RemoveDir(const char *path)
{
	SceInt ret = 0;

	if (R_FAILED(ret = sceIoRmdir(path)))
		return ret;

	return 0;
}

SceOff FS_GetFileSize(const char *path)
{
	SceIoStat stat;
	SceInt ret = 0;

	if (R_FAILED(ret = sceIoGetstat(path, &stat)))
		return ret;

	return stat.st_size;
}

SceDateTime FS_GetFileModifiedTime(const char *path)
{
	SceIoStat stat;
	sceIoGetstat(path, &stat);
	
	return stat.st_mtime;
}

SceOff FS_GetPartitionInfo(SceBool max, const char *partition)
{
	SceInt ret = 0;

	SceOff maxSize = 0;
	SceOff freeSize = 0;

	SceIoStat stat;
	memset(&stat, 0, sizeof(SceIoStat));
	
	if (R_FAILED(ret = sceIoGetstat(partition, &stat)))
		return ret;

	SceIoDevInfo info;
	memset(&info, 0, sizeof(SceIoDevInfo));

	if (R_FAILED(ret = sceIoDevctl(partition, 0x3001, 0, 0, &info, sizeof(SceIoDevInfo))))
		return ret;
	
	freeSize = info.free_size;
	maxSize = info.max_size;
	
	if (!max)
		return freeSize;
	
	return maxSize;
}

SceInt FS_CopyFile(const char *src, const char *dst) 
{
	SceInt fd = 0, wfd = 0, ret = 0;

	char buffer[0x1000];
	SceInt off;
	SceIoStat stat;

	if (R_FAILED(fd = sceIoOpen(src, SCE_O_RDONLY, 0)))
		return fd;

	if (R_FAILED(wfd = sceIoOpen(dst, SCE_O_WRONLY | SCE_O_TRUNC | SCE_O_CREAT, 0777)))
	{
		sceIoClose(fd);
		return wfd;
	}
	
	if (R_FAILED(ret = sceIoGetstatByFd(fd, &stat)))
		goto error;

	if (R_FAILED(ret = sceIoChstatByFd(wfd, &stat, SCE_CST_CT | SCE_CST_AT | SCE_CST_MT)))
		goto error;

	size_t rd, wr, total = 0, write;
	
	while ((rd = sceIoRead(fd, buffer, sizeof(buffer))) > 0)
	{
		off = 0;
		while ((off += (wr = sceIoWrite(wfd, buffer+off, rd-off))) < rd)
		{
			if (wr < 0)	
				goto error;
		}
		total += rd;
		ProgressBar_DisplayProgress("Backing up data...", Utils_Basename(src), total, stat.st_size);
	}
	
	if (R_FAILED(rd)) 
		goto error;

	sceIoClose(fd);
	sceIoClose(wfd);

	return 0;

error:
	sceIoClose(fd);
	sceIoClose(wfd);
	return ret;
}