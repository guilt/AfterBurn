//Special Effects .

#ifndef _M_FX_
#define _M_FX_

#include "pal.c"
#include "decl.h"
#include "sounds.c"
#include "shots.c"

void updateStarfield();
void drawStarfield();

void gameOverInput()
{
    if (gs.ticks == 0) {
        clearKeys();
        gs.isInGame = 0;
        stopMusic();
        if (gs.win) { playWonSound(); } else { playPlayerExplodeSound(); }
    }
    if (isKeyPressed()) {
        if (gs.win) { stopWonSound(); } else { stopPlayerExplodeSound(); }
        clearKeys();
        playMusic();
        setGameState(STATE_HIGH_SCORE_ENTRY);
    }
}

void pauseInput()
{
    if (gs.ticks == 0) {
        clearKeys();
        stopMusic();
        stopCollectSound();
        stopShootSound();
        stopExplodeSound();
        stopPlayerExplodeSound();
        stopEnemyShootSound();
        stopWonSound();
    }
    clearKeys();
    if (gs.state == STATE_QUIT_DIALOG) {
        if (EDGE(KEY_Y)) { playMusic(); setGameState(STATE_GAME_OVER); return; }
        if (EDGE(KEY_N) || EDGE(KEY_ESC)) { playMusic(); setGameState(STATE_PLAYING); return; }
    } else {
        if (EDGE(KEY_P)) { playMusic(); setGameState(STATE_PLAYING); return; }
    }
}

void restartInput()
{
    if (gs.ticks == 0) {
        clearKeys();
        playPlayerExplodeSound();
    }
    if (isKeyPressed()) {
        stopPlayerExplodeSound();
        clearKeys();
        initGame();
        setGameState(STATE_PLAYING);
    }
}

void gameOverRender()
{
    clearScreen();
    updateStarfield();
    drawStarfield();
    drawCenteredTextNoAA(getMidY() - FONT_SIZE, gameOverColor, "%s", gs.win ? winStr : gameOverStr);
    updateGameOverColor();
}

void pauseRender()
{
    clearScreen();
    updateStarfield();
    drawStarfield();
    int isQuitPrompt = (gs.state == STATE_QUIT_DIALOG);
    if (isQuitPrompt) {
        drawCenteredText(getMidY() - FONT_SIZE, activeColor, "%s", quitDialogStr);
    } else {
        drawCenteredText(getMidY() - FONT_SIZE, activeColor, "%s", pauseStr);
    }
}

void restartRender()
{
    clearScreen();
    updateStarfield();
    drawStarfield();
    drawCenteredTextNoAA(getMidY() - (FONT_SIZE + VERTICAL_SPACING), gameOverColor, "%s", gameOverStr);
    updateGameOverColor();
    drawCenteredText(getMidY() + (FONT_SIZE + VERTICAL_SPACING), activeColor, "%s", anyKeyStr);
}

void initStarfield()
{
    int i;
    seedRandom();
    for (i = 0; i < numStars; i++) {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].state = rand() % STAR_LAYERS;
    }
}

void updateStarfield()
{
    int i;
    for (i = 0; i < numStars; i++) {
        stars[i].x += SCREEN_WIDTH - (STAR_SPEED * (stars[i].state + 1));
        stars[i].x %= SCREEN_WIDTH;
    }
}

void thrashStarfield(int deltaX, int deltaY)
{
    int i;
    for (i = 0; i < numStars; i++) {
        stars[i].x += SCREEN_WIDTH + (STAR_SPEED * deltaX);
        stars[i].x %= SCREEN_WIDTH;
        stars[i].y += SCREEN_HEIGHT + (STAR_SPEED * deltaY);
        stars[i].y %= SCREEN_HEIGHT;
    }
}

void drawStarfield()
{
    int i;
    seedRandom();
    for (i = 0; i < numStars; i++) {
        putpixel(drawingBuffer, stars[i].x, stars[i].y, starColors[rand() % STAR_LAYERS]);
    }
}

void checkExplode(int i)
{
    if (enemies[i].explosionFrame >= 0 && enemies[i].explosionFrame < (EXPLOSION_MULTIPLIER * numExplodeFrames)) {
        if (enemies[i].explosionFrame == 0) { playExplodeSound(); }
        enemies[i].bitmap = explodeFrames[((int)(enemies[i].explosionFrame / EXPLOSION_MULTIPLIER)) % numExplodeFrames];
        drawSprite(enemies[i].bitmap, enemies[i].x, enemies[i].y / SIN_SCALE);
    }
    if (enemies[i].explosionFrame >= 0) {
        enemies[i].explosionFrame++;
    }
}
 
#endif // _M_FX_

