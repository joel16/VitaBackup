#ifndef _VITABACKUP_TEXTURES_H_
#define _VITABACKUP_TEXTURES_H_

#include <vita2d.h>

#define COLOUR_TEXT          theme == 0? RGBA8(0, 213, 140, 255) : RGBA8(12, 149, 255, 255)
#define COLOUR_TEXT_SELECTED theme == 0? RGBA8(255, 255, 255, 255) : RGBA8(144, 144, 144, 255)

vita2d_texture *background[2], *logo[2];
vita2d_texture *ico_main_backup[2], *ico_main_restore[2], *ico_main_options[2];
vita2d_texture *ico_main_backup_selected[2], *ico_main_restore_selected[2], *ico_main_options_selected[2];
vita2d_texture *checkbox_full[2], *checkbox_empty[2];
vita2d_texture *checkbox_full_selected[2], *checkbox_empty_selected[2];
vita2d_texture *scroll_bg[2], *scroll_pointer[2];
vita2d_texture *ico_container_zip[2], *ico_container_zip_selected[2];

vita2d_pvf *font;

SceVoid Textures_Load(SceVoid);
SceVoid Textures_Free(SceVoid);

#endif