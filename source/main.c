#include "menu_main.h"
#include "textures.h"
#include "touch.h"

static SceVoid Init_Services(SceVoid)
{
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0, 0, 0, 255));
	Textures_Load();
	touchInit();
}

static SceVoid Term_Services(SceVoid)
{
	Textures_Free();
	vita2d_fini();
}

int main(int argc, char *argv[]) 
{
	Init_Services();

	while(1)
		Menu_Main();

	Term_Services();
}