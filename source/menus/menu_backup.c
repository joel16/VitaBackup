#include "fs.h"
#include "menu_backup.h"
#include "menu_main.h"
#include "microtar.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

#define MAX_MENU_ITEMS 8
#define LIST_PER_PAGE  5
#define DISTANCE_Y     80

SceVoid Menu_Backup(SceVoid)
{
	SceInt selection = 0;

	SceCtrlData pad, old_pad;
	old_pad.buttons = 0;
	SceUInt32 pressed;

	int title_width = vita2d_pvf_text_width(font, 1.5f, "Select backup data");

	const char * items[] = 
	{
		"Encrypted savedata",
		"Decrypted savedata",
		"Trophies",
		"System Settings and User Information",
		"User and Hardware IDs",
		"Licenses",
		"Database",
		"Activation"
	};

	const char * items_desc[] = 
	{
		"/user/00/savedata/",
		"ux0:/data/savegames/",
		"/user/00/trophy/",
		"/registry/system.*",
		"/id.dat",
		"/license/app/",
		"ur0:shell/db/app.db",
		"tm0:/npdrm/act.da"
	};

	SceBool enable[MAX_MENU_ITEMS + 1];

	double scroll_length = (428.0 / (double)MAX_MENU_ITEMS);
	
	while (1)
	{
		memset(&pad, 0, sizeof(SceCtrlData));
		sceCtrlPeekBufferPositive(0, &pad, 1);

		pressed = pad.buttons & ~old_pad.buttons;
		old_pad = pad;

		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(background, 0, 0);

		vita2d_draw_texture(scroll_bg, 922, 56);

		vita2d_pvf_draw_text(font, (960 - title_width) / 2, 50, COLOUR_TEXT, 1.5f, "Select backup data");

		SceInt printed = 0; // Print counter

		for (int i = 0; i < MAX_MENU_ITEMS + 1; i++)
		{
			if (printed == LIST_PER_PAGE)
				break;

			vita2d_draw_texture(scroll_pointer, 922, 56 + (scroll_length * i)); // can't go more than y = 428 or it will be out of bounds

			if (selection < LIST_PER_PAGE || i > (selection - LIST_PER_PAGE))
			{
				if (i == selection)
					vita2d_draw_texture(enable[i] == SCE_TRUE? checkbox_full_selected : checkbox_empty_selected, 50, (110 + (DISTANCE_Y * printed)) - 10);
				else
					vita2d_draw_texture(enable[i] == SCE_TRUE? checkbox_full : checkbox_empty, 50, (110 + (DISTANCE_Y * printed)) - 10);

				vita2d_pvf_draw_text(font, 125, 110 + (DISTANCE_Y * printed), i == selection? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, items[i]);
				vita2d_pvf_draw_text(font, 125, (110 + (DISTANCE_Y * printed)) + 35, i == selection? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, items_desc[i]);

				printed++;
			}
		}

		// Do this in frame
		if (pressed & SCE_CTRL_CROSS)
		{
			if (!enable[selection])
				enable[selection] = SCE_TRUE;
			else
				enable[selection] = SCE_FALSE; 
		}

		vita2d_end_frame();

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

		if (pressed & SCE_CTRL_CIRCLE)
			Menu_Main();

		// Tests so far
		if (pressed & SCE_CTRL_START)
		{
			if (enable[3] == SCE_TRUE)
			{
				FS_CopyFile("vd0:/registry/system.dreg", "ux0:/data/VitaBackup/backups/registry/system.dreg");
				FS_CopyFile("vd0:/registry/system.ireg", "ux0:/data/VitaBackup/backups/registry/system.ireg");
			}
			if (enable[4] == SCE_TRUE)
				FS_CopyFile("ux0:/id.dat", "ux0:/data/VitaBackup/backups/id.dat");
			if (enable[6] == SCE_TRUE)
				FS_CopyFile("ur0:shell/db/app.db", "ux0:/data/VitaBackup/backups/shell/db/app.db");
		}
	}
}