#ifndef _M_LEV_
#define _M_LEV_

#include "decl.h"
#include "fhsc.c"

void levelClearInput()
{
    if (gs.ticks == 0) {
        clearKeys();
        stopMusic();
    }
    if (isKeyPressed()) {
        gs.level++;
        if (gs.level >= numLevels) { gs.win = 1; gs.isDone = 1; }
        gs.enemiesKilled = 0;
        initGame();
        playMusic();
        if (gs.win) { setGameState(STATE_GAME_OVER); }
        else { setGameState(STATE_PLAYING); }
    }
}

void levelClearRender()
{
    clearScreen();
    updateStarfield();
    drawStarfield();
    drawCenteredTextNoAA(getMidY() - FONT_SIZE, makecol(0, 220, 0), "%s", levelClearStr);
    drawCenteredText(getMidY() + FONT_SIZE, activeColor, "%s", anyKeyStr);
}

#endif //_M_LEV_
