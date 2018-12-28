/*
		
		Taken from VitaComix by Arkanite.
		A manga/comic book reader for the Playstation Vita
		
		File: touch.c
		
*/
#ifndef _VITABACKUP_TOUCH_H_
#define _VITABACKUP_TOUCH_H_

#include <vitasdk.h>

#define touchInRect(x1, y1, x2, y2)  ((Touch_CheckPressed()) && (Touch_GetX() >= (x1) && Touch_GetX() <= (x2) && Touch_GetY() >= (y1) && Touch_GetY() <= (y2)))

SceTouchData touch;

SceVoid Touch_Reset(SceVoid);
SceInt touchInit(SceVoid);
SceInt Touch_GetX(SceVoid);
SceInt Touch_GetY(SceVoid);
SceInt Touch_CheckPressed(SceVoid);
SceInt Touch_CheckReleased(SceVoid);
SceInt Touch_CheckHeld(SceVoid);
SceVoid Touch_Update(SceVoid);

#endif
