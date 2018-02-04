#ifndef _VITABACKUP_LOG_H_
#define _VITABACKUP_LOG_H_

#include <vitasdk.h>

#define DEBUG

SceInt Log_Print(const char* format, ...);

#ifdef DEBUG
    #define DEBUG_PRINT(...) Log_Print(__VA_ARGS__)
#else
    #define DEBUG_PRINT(...)
#endif


#endif