#ifndef __Class_Misc_
#define __Class_Misc_

/**
 * @file Misc.h
 */
/**
 * Misc is portable set of miscelaneous functions
 */

#include <time.h>

#if defined(unix)
    #ifndef CLOCKS_PER_SEC
        #define CLOCKS_PER_SEC CLK_TCK
    #endif
#endif

#ifdef _WIN32_WCE
//	#include <windows.h>
#endif

/**
 * Used to configure slimProcessTime() to get elapsed time in minutes
 */
#define TIMESTAMP_MINUTES				0

/**
 * Used to configure slimProcessTime() to get elapsed time in seconds
 */
#define TIMESTAMP_SECONDS				1

/**
 * Used to configure slimProcessTime() to get elapsed time in milliseconds
 */
#define TIMESTAMP_MILLISECONDS			2

class Misc
{
	public:

		/**
		 * Get the elapsed time since the start of the process in <i>precision</i> basetime.
		 * @param precision any values among SLIMTIME_MINUTES, SLIMTIME_SECONDS, SLIMTIME_MILLISECONDS
		 * @return a clock_t integer of elapsed time since the start of the prosses
		 */
		static clock_t getTimeStamp(int precision);
		static char *generateUid(const char *string);
};

#endif
