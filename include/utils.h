#ifndef _VITABACKUP_UTILS_H_
#define _VITABACKUP_UTILS_H_

#include <vitasdk.h>

/// Checks whether a result code indicates success.
#define R_SUCCEEDED(res)   ((res)>=0)
/// Checks whether a result code indicates failure.
#define R_FAILED(res)      ((res)<0)
/// Returns the level of a result code.

SceUInt32 pressed;

int SCE_CTRL_ENTER;
int SCE_CTRL_CANCEL;

SceInt Utils_HandleControls(SceVoid);
SceVoid Utils_GetSizeString(char *string, SceULong64 size);
SceVoid Utils_GetTimeString(char string[16], SceInt time_format, SceDateTime time);
SceVoid Utils_GetDateString(char string[24], SceInt date_format, SceDateTime time, SceBool slash);
char *Utils_Basename(const char *filename);
char *Utils_RemoveExt(char *filename);
SceInt Utils_LockPower(SceVoid);
SceInt Utils_UnlockPower(SceVoid);
SceInt Utils_InitAppUtil(SceVoid);
SceInt Utils_TermAppUtil(SceVoid);
SceVoid Utils_GetEnterButton(SceVoid);

#endif
