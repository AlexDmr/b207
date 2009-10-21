#include <Slimlib/Core/Misc.h>
#include <Slimlib/Core/Socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

clock_t Misc::getTimeStamp(int precision)
{

	double clockDivisor = 0;

#ifndef _WIN32_WCE
	switch(precision)
	{
		case TIMESTAMP_MINUTES:
			clockDivisor = (double)(CLOCKS_PER_SEC * 60);
			break;
		case TIMESTAMP_SECONDS:
			clockDivisor = (double)(CLOCKS_PER_SEC);
			break;
		case TIMESTAMP_MILLISECONDS:
			clockDivisor = (double)(CLOCKS_PER_SEC) / (double)(1000);
			break;
	}

	return (clock_t)((double)(clock()) / clockDivisor);
#endif
#ifdef _WIN32_WCE
	switch(precision)
	{
		case TIMESTAMP_MINUTES:
			clockDivisor = 60000;
			break;
		case TIMESTAMP_SECONDS:
			clockDivisor = 1000;
			break;
		case TIMESTAMP_MILLISECONDS:
			clockDivisor = 1;
			break;
	}
	return (clock_t) (GetTickCount()/clockDivisor);
#endif
}



#define IP_LEN 15
#define TIME_LEN 10
#define NULL_LEN 6

char *Misc::generateUid(const char *string)
{
	char *uid;
	long string_len;

	if((string == NULL) || (strlen(string) > 50))
	{
		string_len = NULL_LEN;
	}
	else
	{
		string_len = strlen(string);
	}

	if((uid = (char *) malloc(IP_LEN + TIME_LEN + string_len + 2)) == NULL)
	{
		return NULL;
	}

	#ifdef _WIN32_WCE
	sprintf(uid, "%s:%x:%s", GetLocalIp(), GetTickCount(), string);
	#else
	sprintf(uid, "%s:%x:%s", GetLocalIp(), (unsigned int) time(NULL), string);
	#endif

	return uid;
}
