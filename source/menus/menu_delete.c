#include <stdio.h>
#include <stdlib.h>

#include "fs.h"
#include "log.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

SceInt Menu_Delete(char *path) {
	SceInt selection = 0, ret = 0;

	SceInt title_width = vita2d_pvf_text_width(font, 1.5f, "Delete backup");
	char *dialog = (char *)malloc(256);

	snprintf(dialog, 256, "Are you sure you want to delete %s", Utils_Basename(path));
	
	SceInt dialog_width = vita2d_pvf_text_width(font, 1.5f, dialog);

	while (SCE_TRUE) {
		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(background, 0, 0);

		vita2d_pvf_draw_text(font, (960 - title_width) / 2, 50, COLOUR_TEXT, 1.5f, "Delete backup");
		vita2d_pvf_draw_text(font, (960 - dialog_width) / 2, 220, COLOUR_TEXT, 1.5f, dialog);

		if (selection == 0)
			vita2d_draw_rectangle(240, 390, vita2d_pvf_text_width(font, 1.5f, "YES") + 20, 
				vita2d_pvf_text_height(font, 1.5f, "YES") + 30, COLOUR_TEXT);
		else
			vita2d_draw_rectangle((710 - vita2d_pvf_text_width(font, 1.5f, "NO")) - 10, 390, 
			vita2d_pvf_text_width(font, 1.5f, "NO") + 20, vita2d_pvf_text_height(font, 1.5f, "NO") + 30, COLOUR_TEXT);

		vita2d_pvf_draw_text(font, 250, 420, selection == 0? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, "YES");
		vita2d_pvf_draw_text(font, 710 - vita2d_pvf_text_width(font, 1.5f, "NO"), 420, selection == 1? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 
			1.5f, "NO");

		vita2d_end_frame();

		Utils_HandleControls();
		Touch_Update();

		if (pressed & SCE_CTRL_RIGHT) {
			if (selection < 1)
				selection++;
			else 
				selection = 0;
		}
		else if (pressed & SCE_CTRL_LEFT) {
			if (selection > 0)
				selection--;
			else 
				selection = 1;
		}

		if (pressed & SCE_CTRL_ENTER) {
			if (selection == 0) {
				if (R_FAILED(ret = FS_RemoveFile(path))) {
					DEBUG_PRINT("FS_RemoveFile %s failed 0x%lx\n", path, ret);
					free(dialog);
					return -1;
				}
				break;
			}
			else if (selection == 1)
				break;
		}
		else if (pressed & SCE_CTRL_CANCEL)
			break;
	}

	free(dialog);
	return 0;
}
