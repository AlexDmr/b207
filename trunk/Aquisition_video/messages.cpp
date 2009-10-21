//#include <atlbase.h>
#include <windows.h>
//#include <dshow.h>
//#include <stdio.h>
//#include <qedit.h>
#include <stdio.h>
#include <stdarg.h>
#include "messages.h"

void Msg(char *szFormat, ...)
{
    TCHAR szBuffer[512];

    va_list pArgs;
    va_start(pArgs, szFormat);
    vsprintf(szBuffer, szFormat, pArgs);
    va_end(pArgs);

    MessageBox(NULL, szBuffer, "PlayCap Message", MB_OK | MB_ICONERROR);
}
