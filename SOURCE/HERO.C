//Hero .

#ifndef _M_HERO_
#define _M_HERO_

#include "decl.h"
#include "effects.c"
#include "collisio.c"
#include "bullets.c"
#include "stat.c"
#include "shots.c"
#include "sounds.c"

void checkIfDead()
{
    if (!playerHealth && !gs.lives) { gs.isDone = 1; }
}

void playingInput()
{
    clearKeys();
    if (EDGE(KEY_ESC)) { setGameState(STATE_QUIT_DIALOG); return; }
    if (EDGE(KEY_P)) { setGameState(STATE_PAUSED); return; }
    if (EDGE(KEY_O)) { gs.prevState = STATE_PLAYING; setGameState(STATE_OPTIONS); return; }
    if (key[KEY_SPACE]) { generateBullet(); }
    if (key[KEY_UP]) {
        if (playerY > 0) { playerY -= shipSpeed; }
        else { thrashStarfield(NODIR, YPDIR); }
    }
    if (key[KEY_DOWN]) {
        if (playerY < (SCREEN_HEIGHT - 1 - shipSpeed - hHeight)) { playerY += shipSpeed; }
        else { thrashStarfield(NODIR, YNDIR); }
    }
    if (key[KEY_LEFT]) {
        if (playerX > 0) { playerX -= shipSpeed; }
        else { thrashStarfield(XPDIR, NODIR); }
    }
    if (key[KEY_RIGHT]) {
        if (playerX < (SCREEN_WIDTH - 1 - shipSpeed - hWidth)) { playerX += shipSpeed; }
        else { thrashStarfield(XNDIR, NODIR); }
    }
    changeBulletType();
    playerX %= SCREEN_WIDTH;
    playerY %= SCREEN_HEIGHT;
}

void updateDecliningLife()
{
    if (playerHealth <= 0) {
        playerHealth = 0;
        if (gs.lives > 0) { gs.lives--; }
    }
}

void updatePlayer()
{
    int i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active && boxesCollide(enemies[i].x, enemies[i].y / SIN_SCALE, eWidth(i), eHeight(i), playerX, playerY, hWidth, hHeight)) {
            isBlinking = 1;
            enemies[i].active = 0;
            enemies[i].explosionFrame = 0;
            playerHealth -= enemyDamage[enemies[i].type];
            if (playerHealth < 0) { playerHealth = 0; }
        }
    }
}

void drawPlayer()
{
    if (isBlinking > 0) {
        if (playerHealth > 0) {
            currentBmp = playerBlankBitmap;
            isBlinking++;
            if (isBlinking == maxBlinkActiveFrames) {
                isBlinking = 0;
            }
        } else {
            currentBmp = playerShipBitmaps[playerFrame];
            playerFrame++;
            playerFrame %= shipFrames;
        }
    } else {
        if ((gs.frameCount % (unsigned)STAR_FREQUENCY) < (unsigned)(shipBlinkFrames * shipAnimSpeed)) {
            currentBmp = playerBlinkBitmaps[playerBlinkFrame / shipAnimSpeed];
            playerBlinkFrame++;
            playerBlinkFrame %= (shipBlinkFrames * shipAnimSpeed);
        } else {
            currentBmp = playerShipBitmaps[playerFrame];
            playerFrame++;
            playerFrame %= shipFrames;
        }
    }
    drawSprite(currentBmp, playerX, playerY);
}

#endif //_M_HERO_

