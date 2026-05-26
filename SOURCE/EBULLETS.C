#ifndef _M_EBULL_
#define _M_EBULL_

#include "decl.h"
#include "hero.c"
#include "collisio.c"
#include "effects.c"
#include "sounds.c"

void initEnemyBullets()
{
    memset(ebullets, 0, sizeof(ebullets));
    currentEnemyBullet = -1;
}

void generateEnemyBullet(int enemyIndex)
{
    int i, spawned = 0;
    currentEnemyBullet = -1;
    for (i = 0; i < MAX_ENEMY_BULLETS && !spawned; i++) {
        if (ebullets[i].active != 0) { continue; }
        if (enemyBulletTypeByEnemy[enemies[enemyIndex].type] < 0) { continue; }
        if ((gs.frameCount + 1) % enemyBulletSpawnInterval[enemyBulletTypeByEnemy[enemies[enemyIndex].type]]) { continue; }

        currentEnemyBullet = ebullets[i].type = enemyBulletTypeByEnemy[enemies[enemyIndex].type];
        ebullets[i].bitmap = enemyBulletBitmaps[ebullets[i].type]
            [getAnimFrameIndex(gs.frameCount, enemyBulletFrames[ebullets[i].type], enemyBulletFrameInterval[ebullets[i].type])];
        ebullets[i].x = (enemies[enemyIndex].x - ebWidth(i));
        ebullets[i].y = (enemies[enemyIndex].y / SIN_SCALE + (eHeight(enemyIndex) / 2));

        if (boxesCollide(ebullets[i].x, ebullets[i].y, ebWidth(i), ebHeight(i),
                    getMinX(), getMinY(), getMaxX(), getMaxY())) {
            spawned = 1;
            ebullets[i].active = 1;
            playEnemyShootSound();
        }
    }
}

void updateEnemyBullets()
{
    int i;
    for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!ebullets[i].active) { continue; }
        ebullets[i].bitmap = enemyBulletBitmaps[ebullets[i].type]
            [getAnimFrameIndex(gs.frameCount, enemyBulletFrames[ebullets[i].type], enemyBulletFrameInterval[ebullets[i].type])];
        if (!boxesCollide(ebullets[i].x, ebullets[i].y, ebWidth(i), ebHeight(i),
                     getMinX(), getMinY(), getMaxX(), getMaxY())) {
            ebullets[i].active = 0;
        }
    }
}

void displaceEnemyBullet(int i)
{
    ebullets[i].x -= enemyBulletXSpeed[ebullets[i].type];
}

void updateEnemyBulletsAndPlayer()
{
    int i;
    for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!ebullets[i].active) { continue; }
        if (!boxesCollide(ebullets[i].x, ebullets[i].y, ebWidth(i), ebHeight(i),
                     playerX, playerY, hWidth, hHeight)) { continue; }
        ebullets[i].active = 0;
        isBlinking = 1;
        playExplodeSound();
        playerHealth -= enemyBulletDamage[ebullets[i].type];
        if (playerHealth <= 0) { playerHealth = ENEMY_BULLET_FIRE_DELAY; }
    }
}

void drawEnemyBullets()
{
    int i;
    for (i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (!ebullets[i].active) { continue; }
        drawSpriteSize(ebullets[i].bitmap, ebullets[i].x, ebullets[i].y, ebWidth(i), ebHeight(i));
        displaceEnemyBullet(i);
    }
}

#endif //_M_EBULL_
