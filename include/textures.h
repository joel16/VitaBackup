#ifndef _VITABACKUP_TEXTURES_H_
#define _VITABACKUP_TEXTURES_H_

#include <vita2d.h>

#define vita2d_end_frame() {		\
	vita2d_end_drawing();			\
	vita2d_common_dialog_update();	\
	vita2d_swap_buffers();			\
	sceDisplayWaitVblankStart();	\
}

#define COLOUR_TEXT          RGBA8(0, 213, 140, 255)
#define COLOUR_TEXT_SELECTED RGBA8(255, 255, 255, 255)

vita2d_texture *background, *logo;
vita2d_texture *ico_main_backup, *ico_main_restore, *ico_main_options;
vita2d_texture *ico_main_backup_selected, *ico_main_restore_selected, *ico_main_options_selected;
vita2d_texture *checkbox_full, *checkbox_empty;
vita2d_texture *checkbox_full_selected, *checkbox_empty_selected;
vita2d_texture *scroll_bg, *scroll_pointer;
vita2d_texture *ico_container_zip, *ico_container_zip_selected;

vita2d_pvf *font;

SceVoid Textures_Load(SceVoid);
SceVoid Textures_Free(SceVoid);

#endif
