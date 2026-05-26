#ifndef _M_FLOG_
#define _M_FLOG_

#include <stdarg.h>
#include "decl.h"
#include "files.h"

void logInit()
{
    logFile = fileOpen(getUserDataPath("GAMELOG.TXT"), "w");
}

void logWrite(const char* format, ...)
{
    if (!logFile) { return; }
    va_list args;
    va_start(args, format);
    vfprintf(logFile, format, args);
    va_end(args);
    fputc('\n', logFile);
    fflush(logFile);
}

void logClose()
{
    if (logFile) { fclose(logFile); logFile = NULL; }
}

#endif
