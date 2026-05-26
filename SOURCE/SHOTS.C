//Screenshots !

#ifndef _M_SHOTS_
#define _M_SHOTS_
 
#include "decl.h" 
#include "files.h"
#include "flog.c"

void takeScreenshot()
{
    int screenshotCount = 0, screenshotDone = 0;
    char fileName[STRING_BUFFER_SIZE];
    for (; !screenshotDone; screenshotCount++)
    {
        sprintf(fileName, screenshotPath, screenshotCount + 1);
        if (!fileExists(fileName))
        {
            if (!saveBitmap(fileName, drawingBuffer, palette))
            {
                screenshotDone = 1;
                logWrite("Screenshot written to: %s", fileName);
            }
        }
    }
}

#endif //_M_SHOTS_

