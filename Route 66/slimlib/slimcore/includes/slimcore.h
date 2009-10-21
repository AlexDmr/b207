#ifndef _SLIMCORE_
#define _SLIMCORE_
	#include "slimsocket.h"
    #include "slimfile.h"
    #include "slimthread.h"
	#include "slimvector.h"
	#include "slimregexp.h"

	#ifdef _WIN32_WCE
        char *strdup(const char *str);
    #endif

    char *generate_uid(const char *string);

#endif

