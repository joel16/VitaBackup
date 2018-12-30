#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"
#include "log.h"
#include "menu_backup.h"
#include "menu_compression.h"
#include "microtar_write.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

#define LIST_PER_PAGE  5
#define DISTANCE_Y     80

SceInt Menu_Backup(SceVoid) {
	SceInt selection = 0;

	SceInt title_width = vita2d_pvf_text_width(font, 1.5f, "Select backup data:");
	SceInt instr_width = vita2d_pvf_text_width(font, 1.5f, "Press START to begin backup process.");

	char line[256];
	char items[256][51];
	char items_path[256][101];
	SceInt count = 0;

	SceUID file = 0;

	if (FS_FileExists("ur0:/data/VitaBackup/path.txt"))
		file = sceIoOpen("ur0:/data/VitaBackup/path.txt", SCE_O_RDONLY, 0);
	else {
		file = sceIoOpen("ur0:/data/VitaBackup/path.txt", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777); // Create default path file:
		char *buf = (char *)malloc(1024);
		
		SceInt len = snprintf(buf, 1024, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", 
			"Icon layout~ux0:/iconlayout.ini",
			"Registry~vd0:/registry/system.dreg",
			"Registry~vd0:/registry/system.ireg",
			"User IDs~ux0:/id.dat",
			"User profile~ur0:/user/00/np/myprofile.dat",
			"Cloud data~tm0:/clouddata/mcid.dat",
			"Activation~tm0:/npdrm/act.dat",
			"Licenses~ux0:/license",
			"Database~ur0:/shell/db/app.db",
			"Encrypted savedata~ux0:/user/00/savedata",
			"Encrypted savedata_backup~ux0:/user/00/savedata_backup",
			"Encrypted savedata_plus~ux0:/user/00/savedata_plus",
			"Decrypted savegames~ux0:/data/savegames",
			"Trophies~ux0:/user/00/trophy",
			"Trophies~ur0:/user/00/trophy");
		
		sceIoWrite(file, buf, len);
		
		free(buf);
		sceIoClose(file);

		file = sceIoOpen("ur0:/data/VitaBackup/path.txt", SCE_O_RDONLY, 0); // Now re-open in read mode.
	}

	while (FS_Gets(line, sizeof line, file) != NULL) {
		sscanf(line, "%[^~]~%[^~]", items[count], items_path[count]);
		items_path[count][strcspn(items_path[count], "\r\n")] = 0; // Remove trailing new line from fgets.
		count++;
	}

	sceIoClose(file);

	SceBool enable[count + 1];
	memset(enable, 0, sizeof(enable)); // Reset all enabled data

	double scroll_length = (372.0 / ((double)count - 1.0));

	SceInt compression = -1;

	while (SCE_TRUE) {
		Utils_HandleControls();

		if (pressed & SCE_CTRL_ENTER) {
			if (!enable[selection])
				enable[selection] = SCE_TRUE;
			else
				enable[selection] = SCE_FALSE;
		}
		
		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(background, 0, 0);

		vita2d_draw_texture(scroll_bg, 922, 56);

		vita2d_pvf_draw_text(font, (960 - title_width) / 2, 50, COLOUR_TEXT, 1.5f, "Select backup data:");
		vita2d_pvf_draw_text(font, (960 - instr_width) / 2, 524, COLOUR_TEXT, 1.5f, "Press START to begin backup process.");

		SceInt printed = 0; // Print counter

		for (int i = 0; i < count + 1; i++) {
			if (printed == LIST_PER_PAGE)
				break;

			if (count > 5) 
				vita2d_draw_texture(scroll_pointer, 922, 56 + (scroll_length * selection)); // can't go more than y = 428 or it will be out of bounds

			if (selection < LIST_PER_PAGE || i > (selection - LIST_PER_PAGE)) {
				if (i == selection)
					vita2d_draw_texture(enable[i] == SCE_TRUE? checkbox_full_selected : checkbox_empty_selected, 50, (110 + (DISTANCE_Y * printed)) - 10);
				else
					vita2d_draw_texture(enable[i] == SCE_TRUE? checkbox_full : checkbox_empty, 50, (110 + (DISTANCE_Y * printed)) - 10);

				vita2d_pvf_draw_text(font, 125, 110 + (DISTANCE_Y * printed), i == selection? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, items[i]);
				vita2d_pvf_draw_text(font, 125, (110 + (DISTANCE_Y * printed)) + 35, i == selection? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, items_path[i]);

				printed++;
			}
		}

		vita2d_end_frame();

		if (pressed & SCE_CTRL_DOWN) {
			if (selection < (count - 1))
				selection++;
			else 
				selection = 0;
		}
		else if (pressed & SCE_CTRL_UP) {
			if (selection > 0)
				selection--;
			else 
				selection = (count - 1);
		}

		if (pressed & SCE_CTRL_CANCEL)
			break;

		if (pressed & SCE_CTRL_START) {
			if (compression < 0)
				compression = Menu_SelectCompression();
			if (compression >= 0) {
				for (SceInt i = 0; i < count + 1; i++) {
					if (enable[i] == SCE_TRUE) {
						Utils_LockPower();
						MicrotarWrite_AddToTar(items_path[i], compression);
						Utils_UnlockPower();
					}		
				}
			}

			compression = -1;
		}
	}

	return 0;
}
