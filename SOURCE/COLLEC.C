#ifndef _M_COLLEC_
#define _M_COLLEC_

#include "decl.h"
#include "effects.c"
#include "collisio.c"
#include "score.c"

void placeCollectible(int x, int y)
{
    int i, j, placed = 0;
    seedRandom();
    for (i = 0; i < MAX_COLLECTIBLES && !placed; i++) {
        if (colls[i].active) { continue; }

        colls[i].x = x;
        colls[i].y = y;
        colls[i].type = (rand() % NUM_COLLECTIBLE_TYPES);

        if (colls[i].type == COLLECTIBLE_AMMO) {
            colls[i].itemIndex = (rand() % numBulletTypes);
            colls[i].active = ammoCollectDuration[colls[i].itemIndex];
            colls[i].bitmap = ammoCollectBitmaps[colls[i].itemIndex]
                [getAnimFrameIndex(gs.frameCount, ammoCollectFrames[colls[i].itemIndex], ammoCollectFrameInterval[colls[i].itemIndex])];
        }
        else if (colls[i].type == COLLECTIBLE_OTHER) {
            colls[i].itemIndex = (rand() % numCollectibles);
            colls[i].active = collectibleDuration[colls[i].itemIndex];
            colls[i].bitmap = collectBitmaps[colls[i].itemIndex]
                [getAnimFrameIndex(gs.frameCount, collectFrames[colls[i].itemIndex], collectFrameInterval[colls[i].itemIndex])];
        }

        placed = 1;
        for (j = 0; j < MAX_COLLECTIBLES; j++) {
            if (j == i || !colls[j].active) { continue; }
            if (boxesCollide(colls[i].x, colls[i].y, cWidth(i), cHeight(i),
                        colls[j].x, colls[j].y, cWidth(j), cHeight(j))) {
                colls[i].active = 0;
                placed = 0;
            }
        }
        if (boxesCollide(colls[i].x, colls[i].y, cWidth(i), cHeight(i),
                    playerX, playerY, hWidth, hHeight)) {
            colls[i].active = 0;
            placed = 0;
        }
    }
}

void generateCollectible(int i, int x, int y)
{
    if (((gs.frameCount % COLLECTIBLE_SPAWN_INTERVAL) + enemyHealth[i]) > COLLECTIBLE_ENEMY_THRESHOLD) {
        placeCollectible(x, y);
    }
}

void updateCollectibles()
{
    int i;
    if (((gs.frameCount % COLLECTIBLE_SPAWN_INTERVAL) + (rand() % COLLECTIBLE_SPAWN_INTERVAL)) > COLLECTIBLE_RANDOM_THRESHOLD) {
        seedRandom();
        placeCollectible((rand() % SCREEN_WIDTH), (rand() % SCREEN_HEIGHT));
    }
    for (i = 0; i < MAX_COLLECTIBLES; i++) {
        if (colls[i].active) {
            if (colls[i].type == COLLECTIBLE_AMMO) {
                colls[i].bitmap = ammoCollectBitmaps[colls[i].itemIndex]
                    [getAnimFrameIndex(gs.frameCount, ammoCollectFrames[colls[i].itemIndex], ammoCollectFrameInterval[colls[i].itemIndex])];
            }
            else if (colls[i].type == COLLECTIBLE_OTHER) {
                colls[i].bitmap = collectBitmaps[colls[i].itemIndex]
                    [getAnimFrameIndex(gs.frameCount, collectFrames[colls[i].itemIndex], collectFrameInterval[colls[i].itemIndex])];
            }
            if (!boxesCollide(colls[i].x, colls[i].y, cWidth(i), cHeight(i),
                         getMinX(), getMinY(), getMaxX(), getMaxY())) {
                colls[i].active = 0;
            }
            else if (boxesCollide(colls[i].x, colls[i].y, cWidth(i), cHeight(i),
                             playerX, playerY, hWidth, hHeight)) {
                playCollectSound();
                addCollectibleScore(i);
            }
        }
        colls[i].active--;
        if (colls[i].active < 0) { colls[i].active = 0; }
    }
}

void drawCollectibles()
{
    int i;
    for (i = 0; i < MAX_COLLECTIBLES; i++) {
        if (colls[i].active) {
            drawSpriteSize(colls[i].bitmap, colls[i].x, colls[i].y, cWidth(i), cHeight(i));
        }
    }
}

#endif //_M_COLLEC_
