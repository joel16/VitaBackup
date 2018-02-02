#ifndef _VITABACKUP_FS_H_
#define _VITABACKUP_FS_H_

#include <vitasdk.h>

SceBool FS_FileExists(const char *path);
SceBool FS_DirExists(const char *path);
SceInt FS_ReadFile(char *path, SceVoid *buf, SceInt size);
SceInt FS_WriteFile(char *path, SceVoid *buf, SceInt size);
SceInt FS_MakeDir(const char *path);
SceVoid FS_RecursiveMakeDir(const char *path);
SceInt FS_RemoveFile(const char *path);
SceInt FS_RemoveDir(const char *path);
SceOff FS_GetFileSize(const char *path);
SceOff FS_GetPartitionInfo(SceBool max, const char *partition);
SceInt FS_CopyFile(const char *src, const char *dst);

#endif