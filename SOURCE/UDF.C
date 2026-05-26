#ifndef _M_UDF_
#define _M_UDF_

#include "decl.h"

void unimplementedInput()
{
    if (gs.ticks == 0) {
        clearKeys();
    }
    if (EDGE(KEY_ESC)) { setGameState(STATE_MAIN_MENU); return; }
}

void unimplementedRender()
{
    clearScreen();
    updateStarfield();
    drawStarfield();
    drawSprite(logo, getMidX() - (logo->w / 2), logo->h / 4);
    drawCenteredText(MENU_Y_OFFSET - (FONT_SIZE + VERTICAL_SPACING), activeColor, "%s", unimplementedStr);
}

#endif //_M_UDF_


