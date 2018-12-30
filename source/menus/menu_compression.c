#include <stdio.h>
#include <stdlib.h>

#include "fs.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

#define COLOUR_COMPRESSION_BAR RGBA8(48, 48, 48, 255)

SceInt Menu_SelectCompression(SceVoid) {
	SceInt selection = 0;

	SceInt title_width = vita2d_pvf_text_width(font, 1.5f, "Choose compression level:");
	char *dialog_1 = (char *)malloc(256);
	char *dialog_2 = (char *)malloc(256);

	snprintf(dialog_1, 256, "Higher compression means lower speed but smaller file. Lower");
	snprintf(dialog_2, 256, "compression means higher speed but larger file. Recommended value is 0.");
	
	SceInt dialog1_width = vita2d_pvf_text_width(font, 1.5f, dialog_1);
	SceInt dialog2_width = vita2d_pvf_text_width(font, 1.5f, dialog_2);

	while (SCE_TRUE) {
		vita2d_start_drawing();
		vita2d_clear_screen();

		vita2d_draw_texture(background, 0, 0);

		vita2d_pvf_draw_text(font, (960 - title_width) / 2, 50, COLOUR_TEXT, 1.5f, "Choose compression level:");
		vita2d_pvf_draw_text(font, (960 - dialog1_width) / 2, 220, COLOUR_TEXT, 1.5f, dialog_1);
		vita2d_pvf_draw_text(font, (960 - dialog2_width) / 2, 260, COLOUR_TEXT, 1.5f, dialog_2);

		// 880 - 11
		vita2d_draw_rectangle(40, 365, 840, 10, COLOUR_COMPRESSION_BAR);
		vita2d_draw_rectangle(40, 365, (selection * 80), 10, COLOUR_TEXT);
		vita2d_draw_texture(compression_pointer, 40 + (selection * 80), 346);

		vita2d_pvf_draw_text(font, 50, 430, COLOUR_TEXT, 1.5f, "0");
		vita2d_pvf_draw_text(font, 130, 430, COLOUR_TEXT, 1.5f, "1");
		vita2d_pvf_draw_text(font, 210, 430, COLOUR_TEXT, 1.5f, "2");
		vita2d_pvf_draw_text(font, 290, 430, COLOUR_TEXT, 1.5f, "3");
		vita2d_pvf_draw_text(font, 370, 430, COLOUR_TEXT, 1.5f, "4");
		vita2d_pvf_draw_text(font, 450, 430, COLOUR_TEXT, 1.5f, "5");
		vita2d_pvf_draw_text(font, 530, 430, COLOUR_TEXT, 1.5f, "6");
		vita2d_pvf_draw_text(font, 610, 430, COLOUR_TEXT, 1.5f, "7");
		vita2d_pvf_draw_text(font, 690, 430, COLOUR_TEXT, 1.5f, "8");
		vita2d_pvf_draw_text(font, 770, 430, COLOUR_TEXT, 1.5f, "9");
		vita2d_pvf_draw_text(font, 840, 430, COLOUR_TEXT, 1.5f, "10");

		vita2d_end_frame();

		Utils_HandleControls();
		Touch_Update();

		if (pressed & SCE_CTRL_RIGHT) {
			if (selection < 10)
				selection++;
			else 
				selection = 0;
		}
		else if (pressed & SCE_CTRL_LEFT) {
			if (selection > 0)
				selection--;
			else 
				selection = 10;
		}

		if (pressed & SCE_CTRL_ENTER)
			break;
		else if (pressed & SCE_CTRL_CANCEL) {
			free(dialog_1);
			free(dialog_2);
			return -1;
		}
	}

	free(dialog_1);
	free(dialog_2);
	return selection;
}

