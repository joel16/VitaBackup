#include "textures.h"
#include "utils.h"

extern SceUChar8 _binary_res_background_png_start;
extern SceUChar8 _binary_res_logo_png_start;

extern SceUChar8 _binary_res_ico_main_backup_png_start;
extern SceUChar8 _binary_res_ico_main_restore_png_start;
extern SceUChar8 _binary_res_ico_main_options_png_start;
extern SceUChar8 _binary_res_ico_main_backup_selected_png_start;
extern SceUChar8 _binary_res_ico_main_restore_selected_png_start;
extern SceUChar8 _binary_res_ico_main_options_selected_png_start;

extern SceUChar8 _binary_res_checkbox_full_png_start;
extern SceUChar8 _binary_res_checkbox_full_selected_png_start;
extern SceUChar8 _binary_res_checkbox_empty_png_start;
extern SceUChar8 _binary_res_checkbox_empty_selected_png_start;

extern SceUChar8 _binary_res_scroll_bg_png_start;
extern SceUChar8 _binary_res_scroll_pointer_png_start;

extern SceUChar8 _binary_res_ico_container_zip_png_start;
extern SceUChar8 _binary_res_ico_container_zip_selected_png_start;

SceVoid Textures_Load(SceVoid)
{
	background = Utils_LoadPNG(&_binary_res_background_png_start);
	logo = Utils_LoadPNG(&_binary_res_logo_png_start);

	ico_main_backup = Utils_LoadPNG(&_binary_res_ico_main_backup_png_start);
	ico_main_restore = Utils_LoadPNG(&_binary_res_ico_main_restore_png_start);
	ico_main_options = Utils_LoadPNG(&_binary_res_ico_main_options_png_start);
	ico_main_backup_selected = Utils_LoadPNG(&_binary_res_ico_main_backup_selected_png_start);
	ico_main_restore_selected = Utils_LoadPNG(&_binary_res_ico_main_restore_selected_png_start);
	ico_main_options_selected = Utils_LoadPNG(&_binary_res_ico_main_options_selected_png_start);

	checkbox_full = Utils_LoadPNG(&_binary_res_checkbox_full_png_start);
	checkbox_full_selected = Utils_LoadPNG(&_binary_res_checkbox_full_selected_png_start);
	checkbox_empty = Utils_LoadPNG(&_binary_res_checkbox_empty_png_start);
	checkbox_empty_selected = Utils_LoadPNG(&_binary_res_checkbox_empty_selected_png_start);

	scroll_bg = Utils_LoadPNG(&_binary_res_scroll_bg_png_start);
	scroll_pointer = Utils_LoadPNG(&_binary_res_scroll_pointer_png_start);

	ico_container_zip = Utils_LoadPNG(&_binary_res_ico_container_zip_png_start);
	ico_container_zip_selected = Utils_LoadPNG(&_binary_res_ico_container_zip_selected_png_start);

	font = vita2d_load_default_pvf();
}

SceVoid Textures_Free(SceVoid)
{
	vita2d_free_pvf(font);

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