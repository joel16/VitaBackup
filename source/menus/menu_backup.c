#include <stdio.h>
#include <string.h>

#include "fs.h"
#include "log.h"
#include "menu_backup.h"
#include "menu_options.h"
#include "microtar_write.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

#define LIST_PER_PAGE  5
#define DISTANCE_Y     80

SceInt Menu_Backup(SceVoid)
{
	SceInt selection = 0;

	SceInt title_width = vita2d_pvf_text_width(font, 1.5f, "Select backup data");

	char line[256];
	char items[256][51];
	char items_path[256][101];
	SceInt count = 0;

	FILE *file = NULL;

	if (FS_FileExists("ur0:/data/VitaBackup/path.txt"))
		file = fopen("ur0:/data/VitaBackup/path.txt", "r");
	else
	{
		file = fopen("ur0:/data/VitaBackup/path.txt", "w"); // Create default path file:
		fprintf(file, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", 
			"Encrypted savedata~ux0:/user/00/savedata",
			"Decrypted savedata~ux0:/data/savegames",
			"Trophies~ux0:/user/00/trophy",
			"System Settings and User Information~vd0:/registry",
			"User and Hardware IDs~ux0:/id.dat",
			"Licenses~ux0:/license",
			"Database~ur0:shell/db/app.db",
			"Activation~tm0:/npdrm/act.dat");
		fclose(file);

		file = fopen("ur0:/data/VitaBackup/path.txt", "r"); // Now re-open in r mode.
	}

	while (fgets(line, sizeof line, file) != NULL) 
	{
		sscanf(line, "%[^~]~%[^~]", items[count], items_path[count]);
		items_path[count][strcspn(items_path[count], "\r\n")] = 0; // Remove trailing new line from fgets.
		count++;
	}

	fclose(file);

	SceBool enable[count + 1];
	memset(enable, 0, sizeof(enable)); // Reset all enabled data

	double scroll_length = (372.0 / ((double)count - 1.0));

	while (1)
	{
		Utils_HandleControls();

		if (pressed & SCE_CTRL_ENTER)
		{
			if (!enable[selection])
				enable[selection] = SCE_TRUE;
			else
				enable[selection] = SCE_FALSE;
		}
		
		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(background[theme], 0, 0);

		vita2d_draw_texture(scroll_bg[theme], 922, 56);

		vita2d_pvf_draw_text(font, (960 - title_width) / 2, 50, COLOUR_TEXT, 1.5f, "Select backup data");

		SceInt printed = 0; // Print counter

		for (int i = 0; i < count + 1; i++)
		{
			if (printed == LIST_PER_PAGE)
				break;

			if (count > 5) 
				vita2d_draw_texture(scroll_pointer[theme], 922, 56 + (scroll_length * selection)); // can't go more than y = 428 or it will be out of bounds

			if (selection < LIST_PER_PAGE || i > (selection - LIST_PER_PAGE))
			{
				if (i == selection)
					vita2d_draw_texture(enable[i] == SCE_TRUE? checkbox_full_selected[theme] : checkbox_empty_selected[theme], 50, (110 + (DISTANCE_Y * printed)) - 10);
				else
					vita2d_draw_texture(enable[i] == SCE_TRUE? checkbox_full[theme] : checkbox_empty[theme], 50, (110 + (DISTANCE_Y * printed)) - 10);

				vita2d_pvf_draw_text(font, 125, 110 + (DISTANCE_Y * printed), i == selection? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, items[i]);
				vita2d_pvf_draw_text(font, 125, (110 + (DISTANCE_Y * printed)) + 35, i == selection? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, items_path[i]);

				printed++;
			}
		}

		vita2d_end_frame();

		if (pressed & SCE_CTRL_DOWN)
		{
			if (selection < (count - 1))
				selection++;
			else 
				selection = 0;
		}
		else if (pressed & SCE_CTRL_UP)
		{
			if (selection > 0)
				selection--;
			else 
				selection = (count - 1);
		}

		if (pressed & SCE_CTRL_CANCEL)
			break;

		// Tests so far
		if (pressed & SCE_CTRL_START)
		{
			for (SceInt i = 0; i < count + 1; i++)
			{
				if (enable[i] == SCE_TRUE)
				{
					Utils_LockPower();
					MicrotarWrite_AddToTar(items_path[i]);
					Utils_UnlockPower();
				}		
			}
		}
	}

	return 0;
}