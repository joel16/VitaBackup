#include "fs.h"
#include "menu_main.h"
#include "menu_options.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

static SceVoid Init_Services(SceVoid)
{
	vita2d_init();
	vita2d_set_clear_color(RGBA8(0, 0, 0, 255));
	Textures_Load();
	touchInit();

	if (!FS_DirExists("ux0:/data/VitaBackup/backups"))
		FS_RecursiveMakeDir("ux0:/data/VitaBackup/backups");
	if (!FS_DirExists("ur0:/data/VitaBackup/backups"))
		FS_RecursiveMakeDir("ur0:/data/VitaBackup/backups");

	if (FS_FileExists("ux0:data/VitaBackup/log.txt")) // Start a new log file on boot.
		FS_RemoveFile("ux0:data/VitaBackup/log.txt");

	Options_LoadConfig();

	sceShellUtilInitEvents(0);	// Initialize sceShellEvents to lock power button and auto suspend during backup/restore.
	Utils_InitAppUtil();
	Utils_GetEnterButton();
}

static SceVoid Term_Services(SceVoid)
{
	Utils_TermAppUtil();
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