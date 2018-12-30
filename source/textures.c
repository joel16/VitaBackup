#include "textures.h"

extern SceUChar8 _binary_res_Phalaris_background_png_start;
extern SceUChar8 _binary_res_Phalaris_logo_png_start;

extern SceUChar8 _binary_res_Phalaris_ico_main_backup_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_restore_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_options_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_backup_selected_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_restore_selected_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_options_selected_png_start;

extern SceUChar8 _binary_res_Phalaris_checkbox_full_png_start;
extern SceUChar8 _binary_res_Phalaris_checkbox_full_selected_png_start;
extern SceUChar8 _binary_res_Phalaris_checkbox_empty_png_start;
extern SceUChar8 _binary_res_Phalaris_checkbox_empty_selected_png_start;

extern SceUChar8 _binary_res_Phalaris_scroll_bg_png_start;
extern SceUChar8 _binary_res_Phalaris_scroll_pointer_png_start;

extern SceUChar8 _binary_res_Phalaris_ico_container_zip_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_container_zip_selected_png_start;

extern SceUChar8 _binary_res_Phalaris_compression_pointer_png_start;

static vita2d_texture *Texture_LoadPNG(const SceVoid *buffer) {
	vita2d_texture *texture = vita2d_load_PNG_buffer(buffer);
	vita2d_texture_set_filters(texture, SCE_GXM_TEXTURE_FILTER_LINEAR, SCE_GXM_TEXTURE_FILTER_LINEAR);
	
	return texture;
}

SceVoid Textures_Load(SceVoid) {
	background = Texture_LoadPNG(&_binary_res_Phalaris_background_png_start);
	logo = Texture_LoadPNG(&_binary_res_Phalaris_logo_png_start);

	ico_main_backup = Texture_LoadPNG(&_binary_res_Phalaris_ico_main_backup_png_start);
	ico_main_restore = Texture_LoadPNG(&_binary_res_Phalaris_ico_main_restore_png_start);
	ico_main_options = Texture_LoadPNG(&_binary_res_Phalaris_ico_main_options_png_start);
	ico_main_backup_selected = Texture_LoadPNG(&_binary_res_Phalaris_ico_main_backup_selected_png_start);
	ico_main_restore_selected = Texture_LoadPNG(&_binary_res_Phalaris_ico_main_restore_selected_png_start);
	ico_main_options_selected = Texture_LoadPNG(&_binary_res_Phalaris_ico_main_options_selected_png_start);

	checkbox_full = Texture_LoadPNG(&_binary_res_Phalaris_checkbox_full_png_start);
	checkbox_full_selected = Texture_LoadPNG(&_binary_res_Phalaris_checkbox_full_selected_png_start);
	checkbox_empty = Texture_LoadPNG(&_binary_res_Phalaris_checkbox_empty_png_start);
	checkbox_empty_selected = Texture_LoadPNG(&_binary_res_Phalaris_checkbox_empty_selected_png_start);

	scroll_bg = Texture_LoadPNG(&_binary_res_Phalaris_scroll_bg_png_start);
	scroll_pointer = Texture_LoadPNG(&_binary_res_Phalaris_scroll_pointer_png_start);

	ico_container_zip = Texture_LoadPNG(&_binary_res_Phalaris_ico_container_zip_png_start);
	ico_container_zip_selected = Texture_LoadPNG(&_binary_res_Phalaris_ico_container_zip_selected_png_start);

	compression_pointer = Texture_LoadPNG(&_binary_res_Phalaris_compression_pointer_png_start);

	font = vita2d_load_default_pvf();
}

SceVoid Textures_Free(SceVoid) {
	vita2d_free_pvf(font);

	vita2d_free_texture(compression_pointer);

	vita2d_free_texture(ico_container_zip_selected);
	vita2d_free_texture(ico_container_zip);

	vita2d_free_texture(scroll_pointer);
	vita2d_free_texture(scroll_bg);

	vita2d_free_texture(checkbox_empty_selected);
	vita2d_free_texture(checkbox_empty);
	vita2d_free_texture(checkbox_full_selected);
	vita2d_free_texture(checkbox_full);

	vita2d_free_texture(ico_main_options_selected);
	vita2d_free_texture(ico_main_restore_selected);
	vita2d_free_texture(ico_main_backup_selected);
	vita2d_free_texture(ico_main_options);
	vita2d_free_texture(ico_main_restore);
	vita2d_free_texture(ico_main_backup);

	vita2d_free_texture(logo);
	vita2d_free_texture(background);
}
