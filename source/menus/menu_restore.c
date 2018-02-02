#include <stdio.h>
#include <stdlib.h>

#include "fs.h"
#include "menu_options.h"
#include "menu_restore.h"
#include "textures.h"
#include "touch.h"
#include "utils.h"

#define MAX_MENU_ITEMS 255
#define LIST_PER_PAGE  5
#define DISTANCE_Y     80

#define wd ":/data/VitaBackup/backups/"

typedef struct File
{
	struct File * next; // Next file
	SceBool isDir; // Folder flag
	char name[256]; // File name
} File;

SceInt selection = 0;
SceInt fileCount = 0;
File * files = NULL;
char cwd[128];

// Free Heap Memory
static SceVoid Restore_RecursiveFree(File * node)
{
	if (node == NULL) 
		return;

	Restore_RecursiveFree(node->next);
	free(node);
}

static SceInt Restore_PopulateBackups(SceBool refresh)
{
	SceInt dir = 0;

	Restore_RecursiveFree(files);
	files = NULL;
	fileCount = 0;

	snprintf(cwd, 50, "%s%s", storage_location == SCE_FALSE? "ux0" : "ur0", wd);

	if (R_SUCCEEDED(dir = sceIoDopen(cwd)))
	{
		SceInt ret = 1;

		// Iterate Files
		while (ret > 0)
		{
			// File entry
			SceIoDirent entry;
			memset(&entry, 0, sizeof(entry));

			if (R_SUCCEEDED(ret = sceIoDread(dir, &entry)))
			{
				// Ingore null filename
				if (entry.d_name[0] == '\0') 
					continue;

				// Ignore "." in all directories
				if (strcmp(entry.d_name, ".") == 0) 
					continue;

				// Ignore ".." in all directories
				if (strcmp(entry.d_name, "..") == 0) 
					continue;

				// Allocate Memory
				File *item = (File *)malloc(sizeof(File));
				memset(item, 0, sizeof(File));

				// Copy File Name
				strcpy(item->name, entry.d_name);

				// Ignore anything that is not a tar file
				char * ext = strrchr(item->name, '.');
				if (strcmp(ext, ".tar") != 0) 
					continue;

				// Set Folder Flag
				item->isDir = SCE_S_ISDIR(entry.d_stat.st_mode);

				if (item->isDir)
					continue;

				// New List
				if (files == NULL) 
					files = item;

				// Existing List
				else
				{
					File * list = files;
					
					while(list->next != NULL) 
						list = list->next;
					
					list->next = item;
				}

				fileCount++;
			}
			else
				return ret;
		}

		if (R_FAILED(ret = sceIoDclose(dir)))
			return ret;
	}
	else
		return dir;

	if (!refresh)	
	{
		if (selection >= fileCount) 
			selection = fileCount - 1; // Keep index
	}
	else 
		selection = 0; // Refresh selection
}

SceBool enable[MAX_MENU_ITEMS + 1];

static SceInt Restore_DisplayFiles(SceVoid)
{
	double scroll_length = (428.0 / (double)fileCount);
	int title_width = vita2d_pvf_text_width(font, 1.5f, "Select restore data");

	vita2d_start_drawing();
	vita2d_clear_screen();
	
	vita2d_draw_texture(background, 0, 0);
	vita2d_draw_texture(scroll_bg, 922, 56);

	vita2d_pvf_draw_text(font, (960 - title_width) / 2, 50, COLOUR_TEXT, 1.5f, "Select restore data");

	// File Iterator
	SceInt i = 0, printed = 0;

	// Draw File List
	File * file = files;
	
	for (; file != NULL; file = file->next)
	{
		// Limit the files per page
		if (printed == LIST_PER_PAGE) 
			break;

		vita2d_draw_texture(scroll_pointer, 922, 56 + (scroll_length * i)); // can't go more than y = 428 or it will be out of bounds

		if (selection < LIST_PER_PAGE || i > (selection - LIST_PER_PAGE))
		{
			if (i == selection)
				vita2d_draw_texture(enable[i] == SCE_TRUE? checkbox_full_selected : checkbox_empty_selected, 50, (110 + (DISTANCE_Y * printed)) - 10);
			else
				vita2d_draw_texture(enable[i] == SCE_TRUE? checkbox_full : checkbox_empty, 50, (110 + (DISTANCE_Y * printed)) - 10);

			char * ext = strrchr(file->name, '.');

			//if (strncasecmp(ext ,".tar") == 0)
			//	vita2d_draw_texture(dbIcon, 80, 130 + (86 * printed));
			
			char buf[64], path[500], size[16];;

			strncpy(buf, file->name, sizeof(buf));
			buf[sizeof(buf) - 1] = '\0';
			int len = strlen(buf);
			len = 40 - len;
			
			while (len -- > 0)
				strcat(buf, " ");

			vita2d_pvf_draw_text(font, 125, 110 + (DISTANCE_Y * printed), i == selection? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, buf); // printf file name

			strcpy(path, cwd);
			strcpy(path + strlen(path), file->name);
			char dateStr[16], timeStr[24];
			Utils_GetDateString(dateStr, 0, FS_GetFileModifiedTime(path)); // Get modified date
			Utils_GetTimeString(timeStr, 0, FS_GetFileModifiedTime(path)); // Get modified time
			
			if (!file->isDir)
			{
				Utils_GetSizeString(size, FS_GetFileSize(path)); // Get size for files only
				vita2d_pvf_draw_textf(font, 125, (110 + (DISTANCE_Y * printed)) + 35, i == selection? COLOUR_TEXT_SELECTED : COLOUR_TEXT, 1.5f, "%s %s - %s", dateStr, timeStr, size);
			}

			printed++; // Increase printed counter
		}

		i++;
	}

	vita2d_end_frame();
}

SceInt Menu_Restore(SceVoid)
{
	SceCtrlData pad, old_pad;
	old_pad.buttons = 0;
	SceUInt32 pressed;

	Restore_PopulateBackups(SCE_TRUE);
	Restore_DisplayFiles();

	while (1)
	{
		memset(&pad, 0, sizeof(SceCtrlData));
		sceCtrlPeekBufferPositive(0, &pad, 1);

		pressed = pad.buttons & ~old_pad.buttons;
		old_pad = pad;

		if (fileCount > 0)
		{
			if (pressed & SCE_CTRL_UP)
			{
				if (selection > 0) 
					selection--;
				else 
					selection = fileCount - 1;

				Restore_DisplayFiles();
			}
			else if (pressed & SCE_CTRL_DOWN)
			{
				if (selection < (fileCount - 1)) 
					selection++;
				else 
					selection = 0;

				Restore_DisplayFiles();
			}
		}

		if (pressed & SCE_CTRL_CROSS)
		{
			if (!enable[selection])
				enable[selection] = SCE_TRUE;
			else
				enable[selection] = SCE_FALSE; 
			
			Restore_DisplayFiles();
		}

		if (pressed & SCE_CTRL_CIRCLE)
			break;
	}

	return 1;
}