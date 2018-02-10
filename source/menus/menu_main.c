#include "menu_backup.h"
#include "menu_main.h"
#include "menu_options.h"
#include "menu_restore.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

#define MAX_MENU_ITEMS 2

SceVoid Menu_Main(SceVoid)
{
	unsigned int logo_width = vita2d_texture_get_width(logo[theme]);
	unsigned int icon_width = vita2d_texture_get_width(ico_main_backup[theme]); // 112

	SceInt selection = 0;

	while (1) 
	{
		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(background[theme], 0, 0);

		vita2d_draw_texture(logo[theme], ((960 - logo_width) / 2), 60);

		vita2d_draw_texture(selection == 0? ico_main_backup_selected[theme] : ico_main_backup[theme], (((960 - icon_width) / 2) - (icon_width + 100)), 290);
		vita2d_draw_texture(selection == 1? ico_main_restore_selected[theme] : ico_main_restore[theme], ((960 - icon_width) / 2), 290);
		vita2d_draw_texture(selection == 2? ico_main_options_selected[theme] : ico_main_options[theme], (((960 - icon_width) / 2) + (icon_width + 100)), 290);

		vita2d_pvf_draw_text(font, (((960 - icon_width) / 2) - (icon_width + 100)) + 10, 442, selection == 0? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 
			1.5f, "Backup");
		vita2d_pvf_draw_text(font, ((960 - icon_width) / 2) + 10, 442, selection == 1? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, "Restore");
		vita2d_pvf_draw_text(font, (((960 - icon_width) / 2) + (icon_width + 100)) + 10, 442, selection == 2? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 
			1.5f, "Options");

		vita2d_end_frame();

		Utils_HandleControls();
		Touch_Update();

		if (Touch_GetX() >= (((960 - icon_width) / 2) - (icon_width + 100)) && Touch_GetX() <= 324 && Touch_GetY() >= 290 && Touch_GetY() <= 402)
		{
			selection = 0;

			if (Touch_CheckPressed())
			{
				Touch_Reset();
				selection = Menu_Backup();
			}
		}
		else if (Touch_GetX() >= ((960 - icon_width) / 2) && Touch_GetX() <= 536 && Touch_GetY() >= 290 && Touch_GetY() <= 402)
		{
			selection = 1;

			if (Touch_CheckPressed())
			{
				Touch_Reset();
				selection = Menu_Restore();
			}
		}
		else if (Touch_GetX() >= (((960 - icon_width) / 2) + (icon_width + 100)) && Touch_GetX() <= 748 && Touch_GetY() >= 290 && Touch_GetY() <= 402)
		{
			selection = 2;

			if (Touch_CheckPressed())
			{
				Touch_Reset();
				selection = Menu_Options();
			}
		}

		if (pressed & SCE_CTRL_RIGHT)
			selection++;
		else if (pressed & SCE_CTRL_LEFT)
			selection--;

		if (selection > MAX_MENU_ITEMS)
			selection = 0;
		if (selection < 0)
			selection = MAX_MENU_ITEMS;

		if (pressed & SCE_CTRL_ENTER)
		{
			switch (selection)
			{
				case 0:
					selection = Menu_Backup();
					break;
				case 1:
					selection = Menu_Restore();
					break;
				case 2:
					selection = Menu_Options();
					break;
			}
		}
	}
}