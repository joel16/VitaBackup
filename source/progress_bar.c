#include "menu_options.h"
#include "textures.h"
#include "utils.h"

SceVoid ProgressBar_DisplayProgress(char *msg, char *src, SceULong64 offset, SceULong64 size)
{
	vita2d_start_drawing();
	vita2d_clear_screen();
	
	vita2d_draw_texture(background[theme], 0, 0);

	int title_width = vita2d_pvf_text_width(font, 1.5f, msg);
	vita2d_pvf_draw_text(font, (960 - title_width) / 2, 50, COLOUR_TEXT, 1.5f, msg);

	int name_width = vita2d_pvf_text_width(font, 1.5f, src);
	vita2d_pvf_draw_text(font, (960 - name_width) / 2, 270, COLOUR_TEXT, 1.5f, src);

	vita2d_draw_rectangle(200, 300, 560, 30, COLOUR_TEXT_SELECTED);
	vita2d_draw_rectangle(200, 300, offset / size * 560.0, 30, COLOUR_TEXT);

	vita2d_end_frame();
}