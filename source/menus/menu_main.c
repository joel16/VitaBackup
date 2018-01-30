#include "menu_backup.h"
#include "menu_main.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

#define MAX_MENU_ITEMS 2

SceVoid Menu_Main(SceVoid)
{
	unsigned int logo_width = vita2d_texture_get_width(logo);
	unsigned int icon_width = vita2d_texture_get_width(ico_main_backup); // 112

	SceInt selection = 0;

	SceCtrlData pad, old_pad;
	old_pad.buttons = 0;
	SceUInt32 pressed;

	while (1) 
	{
		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(background, 0, 0);

		vita2d_draw_texture(logo, ((960 - logo_width) / 2), 60);

		vita2d_draw_texture(selection == 0? ico_main_backup_selected : ico_main_backup, (((960 - icon_width) / 2) - (icon_width + 100)), 290);
		vita2d_draw_texture(selection == 1? ico_main_restore_selected : ico_main_restore, ((960 - icon_width) / 2), 290);
		vita2d_draw_texture(selection == 2? ico_main_options_selected : ico_main_options, (((960 - icon_width) / 2) + (icon_width + 100)), 290);

		vita2d_pvf_draw_text(font, (((960 - icon_width) / 2) - (icon_width + 100)) + 20, 442, selection == 0? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 
			1.2f, "Backup");
		vita2d_pvf_draw_text(font, ((960 - icon_width) / 2) + 20, 442, selection == 1? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.2f, "Restore");
		vita2d_pvf_draw_text(font, (((960 - icon_width) / 2) + (icon_width + 100)) + 20, 442, selection == 2? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 
			1.2f, "Options");

		vita2d_end_frame();

		memset(&pad, 0, sizeof(SceCtrlData));
		sceCtrlPeekBufferPositive(0, &pad, 1);

		pressed = pad.buttons & ~old_pad.buttons;
		old_pad = pad;

		if (pressed & SCE_CTRL_RIGHT)
			selection++;
		else if (pressed & SCE_CTRL_LEFT)
			selection--;

		if (selection > MAX_MENU_ITEMS)
			selection = 0;
		if (selection < 0)
			selection = MAX_MENU_ITEMS;

		if (pressed & SCE_CTRL_CROSS)
		{
			if (selection == 0)
				Menu_Backup();
		}
	}
}