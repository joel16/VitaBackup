#include "textures.h"
#include "utils.h"

extern SceUChar8 _binary_res_Phalaris_background_png_start;
extern SceUChar8 _binary_res_Phalaris_logo_png_start;
extern SceUChar8 _binary_res_Light_background_png_start;
extern SceUChar8 _binary_res_Light_logo_png_start;

extern SceUChar8 _binary_res_Phalaris_ico_main_backup_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_restore_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_options_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_backup_selected_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_restore_selected_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_main_options_selected_png_start;
extern SceUChar8 _binary_res_Light_ico_main_backup_png_start;
extern SceUChar8 _binary_res_Light_ico_main_restore_png_start;
extern SceUChar8 _binary_res_Light_ico_main_options_png_start;
extern SceUChar8 _binary_res_Light_ico_main_backup_selected_png_start;
extern SceUChar8 _binary_res_Light_ico_main_restore_selected_png_start;
extern SceUChar8 _binary_res_Light_ico_main_options_selected_png_start;

extern SceUChar8 _binary_res_Phalaris_checkbox_full_png_start;
extern SceUChar8 _binary_res_Phalaris_checkbox_full_selected_png_start;
extern SceUChar8 _binary_res_Phalaris_checkbox_empty_png_start;
extern SceUChar8 _binary_res_Phalaris_checkbox_empty_selected_png_start;
extern SceUChar8 _binary_res_Light_checkbox_full_png_start;
extern SceUChar8 _binary_res_Light_checkbox_full_selected_png_start;
extern SceUChar8 _binary_res_Light_checkbox_empty_png_start;
extern SceUChar8 _binary_res_Light_checkbox_empty_selected_png_start;

extern SceUChar8 _binary_res_Phalaris_scroll_bg_png_start;
extern SceUChar8 _binary_res_Phalaris_scroll_pointer_png_start;
extern SceUChar8 _binary_res_Light_scroll_bg_png_start;
extern SceUChar8 _binary_res_Light_scroll_pointer_png_start;

extern SceUChar8 _binary_res_Phalaris_ico_container_zip_png_start;
extern SceUChar8 _binary_res_Phalaris_ico_container_zip_selected_png_start;
extern SceUChar8 _binary_res_Light_ico_container_zip_png_start;
extern SceUChar8 _binary_res_Light_ico_container_zip_selected_png_start;

SceVoid Textures_Load(SceVoid)
{
	background[0] = Utils_LoadPNG(&_binary_res_Phalaris_background_png_start);
	logo[0] = Utils_LoadPNG(&_binary_res_Phalaris_logo_png_start);
	background[1] = Utils_LoadPNG(&_binary_res_Light_background_png_start);
	logo[1] = Utils_LoadPNG(&_binary_res_Light_logo_png_start);

	ico_main_backup[0] = Utils_LoadPNG(&_binary_res_Phalaris_ico_main_backup_png_start);
	ico_main_restore[0] = Utils_LoadPNG(&_binary_res_Phalaris_ico_main_restore_png_start);
	ico_main_options[0] = Utils_LoadPNG(&_binary_res_Phalaris_ico_main_options_png_start);
	ico_main_backup_selected[0] = Utils_LoadPNG(&_binary_res_Phalaris_ico_main_backup_selected_png_start);
	ico_main_restore_selected[0] = Utils_LoadPNG(&_binary_res_Phalaris_ico_main_restore_selected_png_start);
	ico_main_options_selected[0] = Utils_LoadPNG(&_binary_res_Phalaris_ico_main_options_selected_png_start);
	ico_main_backup[1] = Utils_LoadPNG(&_binary_res_Light_ico_main_backup_png_start);
	ico_main_restore[1] = Utils_LoadPNG(&_binary_res_Light_ico_main_restore_png_start);
	ico_main_options[1] = Utils_LoadPNG(&_binary_res_Light_ico_main_options_png_start);
	ico_main_backup_selected[1] = Utils_LoadPNG(&_binary_res_Light_ico_main_backup_selected_png_start);
	ico_main_restore_selected[1] = Utils_LoadPNG(&_binary_res_Light_ico_main_restore_selected_png_start);
	ico_main_options_selected[1] = Utils_LoadPNG(&_binary_res_Light_ico_main_options_selected_png_start);

	checkbox_full[0] = Utils_LoadPNG(&_binary_res_Phalaris_checkbox_full_png_start);
	checkbox_full_selected[0] = Utils_LoadPNG(&_binary_res_Phalaris_checkbox_full_selected_png_start);
	checkbox_empty[0] = Utils_LoadPNG(&_binary_res_Phalaris_checkbox_empty_png_start);
	checkbox_empty_selected[0] = Utils_LoadPNG(&_binary_res_Phalaris_checkbox_empty_selected_png_start);
	checkbox_full[1] = Utils_LoadPNG(&_binary_res_Light_checkbox_full_png_start);
	checkbox_full_selected[1] = Utils_LoadPNG(&_binary_res_Light_checkbox_full_selected_png_start);
	checkbox_empty[1] = Utils_LoadPNG(&_binary_res_Light_checkbox_empty_png_start);
	checkbox_empty_selected[1] = Utils_LoadPNG(&_binary_res_Light_checkbox_empty_selected_png_start);

	scroll_bg[0] = Utils_LoadPNG(&_binary_res_Phalaris_scroll_bg_png_start);
	scroll_pointer[0] = Utils_LoadPNG(&_binary_res_Phalaris_scroll_pointer_png_start);
	scroll_bg[1] = Utils_LoadPNG(&_binary_res_Light_scroll_bg_png_start);
	scroll_pointer[1] = Utils_LoadPNG(&_binary_res_Light_scroll_pointer_png_start);

	ico_container_zip[0] = Utils_LoadPNG(&_binary_res_Phalaris_ico_container_zip_png_start);
	ico_container_zip_selected[0] = Utils_LoadPNG(&_binary_res_Phalaris_ico_container_zip_selected_png_start);
	ico_container_zip[1] = Utils_LoadPNG(&_binary_res_Light_ico_container_zip_png_start);
	ico_container_zip_selected[1] = Utils_LoadPNG(&_binary_res_Light_ico_container_zip_selected_png_start);

	font = vita2d_load_default_pvf();
}

SceVoid Textures_Free(SceVoid)
{
	vita2d_free_pvf(font);

	for (SceInt i = 0; i < 2; i++)
	{
		vita2d_free_texture(ico_container_zip_selected[i]);
		vita2d_free_texture(ico_container_zip[i]);

		vita2d_free_texture(scroll_pointer[i]);
		vita2d_free_texture(scroll_bg[i]);

		vita2d_free_texture(checkbox_empty_selected[i]);
		vita2d_free_texture(checkbox_empty[i]);
		vita2d_free_texture(checkbox_full_selected[i]);
		vita2d_free_texture(checkbox_full[i]);

		vita2d_free_texture(ico_main_options_selected[i]);
		vita2d_free_texture(ico_main_restore_selected[i]);
		vita2d_free_texture(ico_main_backup_selected[i]);
		vita2d_free_texture(ico_main_options[i]);
		vita2d_free_texture(ico_main_restore[i]);
		vita2d_free_texture(ico_main_backup[i]);

		vita2d_free_texture(logo[i]);
		vita2d_free_texture(background[i]);
	}
}