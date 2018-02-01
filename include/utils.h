#ifndef _VITABACKUP_UTILS_H_
#define _VITABACKUP_UTILS_H_

#include <vitasdk.h>

#include <vita2d.h>

/// Checks whether a result code indicates success.
#define R_SUCCEEDED(res)   ((res)>=0)
/// Checks whether a result code indicates failure.
#define R_FAILED(res)      ((res)<0)
/// Returns the level of a result code.

#define vita2d_end_frame() {		\
	vita2d_end_drawing();			\
	vita2d_common_dialog_update();	\
	vita2d_swap_buffers();			\
	sceDisplayWaitVblankStart();	\
}

vita2d_texture *Utils_LoadPNG(const SceVoid * buffer);
SceVoid Utils_GetSizeString(char *string, SceULong64 size);
char *Utils_Basename(const char * filename);
char *Utils_RemoveExt(char *filename);

#endif