#include <slimcore.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
      #include <time.h>
#endif


#ifdef _WIN32_WCE

char *strdup(const char *str)
{
	char *buffer;

	buffer = newchar[strlen(str) + 1];
	if(buffer != NULL)
	{
		memcpy(buffer, str, strlen(str) + 1);
	}

	return buffer;
}

#endif


#define IP_LEN 15
#define TIME_LEN 10
#define NULL_LEN 6

char *generate_uid(const char *string)
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

        uid = new char[IP_LEN + TIME_LEN + string_len + 2];
	if(uid == NULL)
	{
		return NULL;
	}

	sprintf(uid, "%s:%x:%s", slimGetLocalIp(), time(NULL), string);

	return uid;
}
