#ifndef _VITABACKUP_PROGRESS_BAR_H_
#define _VITABACKUP_PROGRESS_BAR_H_

#include <vitasdk.h>

SceVoid ProgressBar_DisplayProgress(char *msg, char *src, SceInt offset, SceInt size);

#endif