#include <stdio.h>
#include <stdlib.h>
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

static SceVoid Utils_ConvertUtcToLocalTime(SceDateTime *time_local, SceDateTime *time_utc) 
{
	SceRtcTick tick;
	sceRtcGetTick(time_utc, &tick);
	sceRtcConvertUtcToLocalTime(&tick, &tick);
	sceRtcSetTick(time_local, &tick);	
}

SceVoid Utils_GetTimeString(char string[16], SceInt time_format, SceDateTime time) 
{
	SceDateTime time_local;
	Utils_ConvertUtcToLocalTime(&time_local, &time);

	switch(time_format) 
	{
		case SCE_SYSTEM_PARAM_TIME_FORMAT_12HR:
			snprintf(string, 16, "%02d:%02d %s", (time_local.hour > 12) ? (time_local.hour-12) : ((time_local.hour == 0) ? 12 : time_local.hour), time_local.minute, time_local.hour >= 12 ? "PM" : "AM");
			break;

		case SCE_SYSTEM_PARAM_TIME_FORMAT_24HR:
			snprintf(string, 16, "%02d:%02d", time_local.hour, time_local.minute);
			break;
	}
}

SceVoid Utils_GetDateString(char string[24], SceInt date_format, SceDateTime time) 
{
	SceDateTime time_local;
	Utils_ConvertUtcToLocalTime(&time_local, &time);

	switch (date_format) 
	{
		case SCE_SYSTEM_PARAM_DATE_FORMAT_YYYYMMDD:
			snprintf(string, 24, "%04d/%02d/%02d", time_local.year, time_local.month, time_local.day);
			break;

		case SCE_SYSTEM_PARAM_DATE_FORMAT_DDMMYYYY:
			snprintf(string, 24, "%02d/%02d/%04d", time_local.day, time_local.month, time_local.year);
			break;

		case SCE_SYSTEM_PARAM_DATE_FORMAT_MMDDYYYY:
			snprintf(string, 24, "%02d/%02d/%04d", time_local.month, time_local.day, time_local.year);
			break;
	}
}

char *Utils_Basename(const char * filename)
{
	char *p = strrchr (filename, '/');
	return p ? p + 1 : (char *) filename;
}

char *Utils_RemoveExt(char *filename) 
{
	char *ret, *lastdot;

   	if (filename == NULL)
   		return NULL;
   	if ((ret = malloc(strlen(filename) + 1)) == NULL)
   		return NULL;

   	strcpy(ret, filename);
   	lastdot = strrchr(ret, '.');

   	if (lastdot != NULL)
   		*lastdot = '\0';

   	return ret;
}