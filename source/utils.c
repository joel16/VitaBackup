#include <stdio.h>
#include <string.h>

#include "utils.h"

vita2d_texture *Utils_LoadPNG(const SceVoid *buffer)
{
	vita2d_texture * texture = vita2d_load_PNG_buffer(buffer);
	vita2d_texture_set_filters(texture, SCE_GXM_TEXTURE_FILTER_LINEAR, SCE_GXM_TEXTURE_FILTER_LINEAR);
	
	return texture;
}

SceVoid Utils_GetSizeString(char * string, SceULong64 size) //Thanks TheOfficialFloW
{
	double double_size = (double)size;

	SceInt i = 0;
	static char * units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
	
	while (double_size >= 1024.0f) 
	{
		double_size /= 1024.0f;
		i++;
	}

	sprintf(string, "%.*f %s", (i == 0) ? 0 : 2, double_size, units[i]);
}

char *Utils_Basename(const char * filename)
{
	char *p = strrchr (filename, '/');
	return p ? p + 1 : (char *) filename;
}