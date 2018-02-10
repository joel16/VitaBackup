#include <stdio.h>
#include <stdlib.h>

#include "fs.h"
#include "menu_options.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

#define MAX_MENU_ITEMS 2
#define MAX_MENU_ITEMS_ROW1 2
#define MAX_MENU_ITEMS_ROW2 2

const char * configFile =
	"storage_location = %d\n"
	"theme = %d\n";

static SceInt Options_SaveConfig(SceBool storage_location, SceInt theme)
{
	SceInt ret = 0;
	
	char *config_path = (char *)malloc(32);
	snprintf(config_path, 32, "ur0:/data/VitaBackup/config.cfg");

	char *buf = (char *)malloc(128);
	SceInt len = snprintf(buf, 128, configFile, storage_location, theme);
	
	if (R_FAILED(ret = FS_WriteFile(config_path, buf, len)))
	{
		free(config_path);
		free(buf);
		return ret;
	}
	
	free(config_path);
	free(buf);
	return 0;
}

SceInt Options_LoadConfig(SceVoid)
{
	SceInt ret = 0;	

	char *config_path = (char *)malloc(32);
	snprintf(config_path, 32, "ur0:/data/VitaBackup/config.cfg");

	if (!(FS_FileExists(config_path)))
	{
		storage_location = SCE_FALSE; // 0(FALSE) = ux0: and 1(TRUE) = ur0:
		theme = 0;
		Options_SaveConfig(storage_location, theme);
	}

	char *buf = (char *)malloc(128);

	if (R_FAILED(ret = FS_ReadFile(config_path, buf, 128)))
	{
		free(config_path);
		free(buf);
		return ret;
	}

	sscanf(buf, configFile, &storage_location, &theme);

	free(config_path);
	free(buf);
	return 0;
}

SceInt Menu_Options(SceVoid)
{
	SceInt selection = 0, row_1 = 0, row_2 = 0;

	SceInt title_width = vita2d_pvf_text_width(font, 1.5f, "Options");

	Options_LoadConfig(); // Reload config

	while (1)
	{
		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(background[theme], 0, 0);

		vita2d_pvf_draw_text(font, (960 - title_width) / 2, 50, COLOUR_TEXT, 1.5f, "Options");

		vita2d_pvf_draw_text(font, 125, 110, selection == 0? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, "Backup destination:"); 
		vita2d_pvf_draw_text(font, 125, 235, selection == 1? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, "Theme:"); 
		
		/* Row 1*/
		vita2d_pvf_draw_text(font, 125, 155, row_1 == 0? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, "ux0");
		if (row_1 == 0)
			vita2d_draw_texture(storage_location == SCE_FALSE? checkbox_full_selected[theme] : checkbox_empty_selected[theme], 190, 130);
		else
			vita2d_draw_texture(storage_location == SCE_FALSE? checkbox_full[theme] : checkbox_empty[theme], 190, 130);

		vita2d_pvf_draw_text(font, 525, 155, row_1 == 1? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, "ur0");
		if (row_1 == 1)
			vita2d_draw_texture(storage_location == SCE_TRUE? checkbox_full_selected[theme] : checkbox_empty_selected[theme], 590, 130);
		else
			vita2d_draw_texture(storage_location == SCE_TRUE? checkbox_full[theme] : checkbox_empty[theme], 590, 130);

		/* Row 2*/
		vita2d_pvf_draw_text(font, 125, 280, row_2 == 0? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, "Phalaris");
		if (row_2 == 0)
			vita2d_draw_texture(theme == 0? checkbox_full_selected[theme] : checkbox_empty_selected[theme], 240, 255);
		else
			vita2d_draw_texture(theme == 0? checkbox_full[theme] : checkbox_empty[theme], 240, 255);

		vita2d_pvf_draw_text(font, 525, 280, row_2 == 1? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, "Light");
		if (row_2 == 1)
			vita2d_draw_texture(theme == 1? checkbox_full_selected[theme] : checkbox_empty_selected[theme], 610, 255);
		else
			vita2d_draw_texture(theme == 1? checkbox_full[theme] : checkbox_empty[theme], 610, 255);

		vita2d_end_frame();

		Utils_HandleControls();
		Touch_Update();

		if (Touch_GetX() >= 180 && Touch_GetX() <= 232 && Touch_GetY() >= 120 && Touch_GetY() <= 172)
		{
			row_1 = 0;

			if (Touch_CheckPressed())
			{
				storage_location = SCE_FALSE;
				Options_SaveConfig(storage_location, theme);
			}
		}
		else if (Touch_GetX() >= 580 && Touch_GetX() <= 632 && Touch_GetY() >= 120 && Touch_GetY() <= 172)
		{
			row_1 = 1;

			if (Touch_CheckPressed())
			{
				storage_location = SCE_TRUE;
				Options_SaveConfig(storage_location, theme);
			}
		}
		else if (Touch_GetX() >= 230 && Touch_GetX() <= 282 && Touch_GetY() >= 245 && Touch_GetY() <= 297)
		{
			row_2 = 0;

			if (Touch_CheckPressed())
			{
				theme = 0;
				Options_SaveConfig(storage_location, theme);
			}
		}
		else if (Touch_GetX() >= 600 && Touch_GetX() <= 652 && Touch_GetY() >= 245 && Touch_GetY() <= 297)
		{
			row_2 = 1;

			if (Touch_CheckPressed())
			{
				theme = 1;
				Options_SaveConfig(storage_location, theme);
			}
		}

		if (pressed & SCE_CTRL_DOWN)
		{
			if (selection < (MAX_MENU_ITEMS - 1))
				selection++;
			else 
				selection = 0;
		}
		else if (pressed & SCE_CTRL_UP)
		{
			if (selection > 0)
				selection--;
			else 
				selection = (MAX_MENU_ITEMS - 1);
		}

		switch (selection)
		{
			case 0:
				if (pressed & SCE_CTRL_RIGHT)
				{
					if (row_1 < (MAX_MENU_ITEMS_ROW1 - 1))
						row_1++;
					else 
						row_1 = 0;
				}
				else if (pressed & SCE_CTRL_LEFT)
				{
					if (row_1 > 0)
						row_1--;
					else 
						row_1 = (MAX_MENU_ITEMS_ROW1 - 1);
				}

				if (pressed & SCE_CTRL_ENTER)
				{
					switch (row_1)
					{
						case 0:
							storage_location = SCE_FALSE;
							Options_SaveConfig(storage_location, theme);
							break;
						case 1:
							storage_location = SCE_TRUE;
							Options_SaveConfig(storage_location, theme);
							break;
					}
				}
				break;
			case 1:
				if (pressed & SCE_CTRL_RIGHT)
				{
					if (row_2 < (MAX_MENU_ITEMS_ROW1 - 1))
						row_2++;
					else 
						row_2 = 0;
				}
				else if (pressed & SCE_CTRL_LEFT)
				{
					if (row_2 > 0)
						row_2--;
					else 
						row_2 = (MAX_MENU_ITEMS_ROW1 - 1);
				}

				if (pressed & SCE_CTRL_ENTER)
				{
					switch (row_2)
					{
						case 0:
							theme = 0;
							Options_SaveConfig(storage_location, theme);
							break;
						case 1:
							theme = 1;
							Options_SaveConfig(storage_location, theme);
							break;
					}
				}
				break;
		}

		if (pressed & SCE_CTRL_CANCEL)
			break;
	}

	return 2;
}
