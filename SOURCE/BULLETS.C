#ifndef _M_BULLETS_
#define _M_BULLETS_

#include "decl.h"
#include "collec.c"
#include "collisio.c"
#include "effects.c"
#include "sounds.c"
#include "score.c"

void oneTimeBulletInit()
{
    currentBullet = 0;
    for (int i = 0; i < numBulletTypes; i++) {
        bulletDamage[i] = defaultBulletDamage[i];
        bulletAmmo[i] = defaultBulletAmmo[i];
    }
}

void initBullets()
{
    memset(bullets, 0, sizeof(bullets));
}

void clipBulletCounts()
{
    for (int i = 0; i < numBulletTypes; i++) {
        if (i && bulletAmmo[i] > maxBulletAmmo[i]) { bulletAmmo[i] = maxBulletAmmo[i]; }
        if (bulletDamage[i] > maxBulletDamage[i]) { bulletDamage[i] = maxBulletDamage[i]; }
    }
}

void changeBulletType()
{
    if (key[KEY_0] || key[KEY_0_PAD]) { currentBullet = 9; }
    if (key[KEY_9] || key[KEY_9_PAD]) { currentBullet = 8; }
    if (key[KEY_8] || key[KEY_8_PAD]) { currentBullet = 7; }
    if (key[KEY_7] || key[KEY_7_PAD]) { currentBullet = 6; }
    if (key[KEY_6] || key[KEY_6_PAD]) { currentBullet = 5; }
    if (key[KEY_5] || key[KEY_5_PAD]) { currentBullet = 4; }
    if (key[KEY_4] || key[KEY_4_PAD]) { currentBullet = 3; }
    if (key[KEY_3] || key[KEY_3_PAD]) { currentBullet = 2; }
    if (key[KEY_2] || key[KEY_2_PAD]) { currentBullet = 1; }
    if (key[KEY_1] || key[KEY_1_PAD]) { currentBullet = 0; }
    if (key[KEY_Q]) { currentBullet += numBulletTypes - 1; currentBullet %= numBulletTypes; }
    if (key[KEY_W]) { currentBullet++; currentBullet %= numBulletTypes; }
    if (currentBullet >= MAX_BULLET_TYPES || currentBullet >= numBulletTypes) { currentBullet = 0; }
}

void setBulletSpec(int i)
{
    seedRandom();
    bullets[i].directionMode = bulletDirection[bullets[i].type];
    if (bullets[i].directionMode) {
        bullets[i].movementMode = ((gs.frameCount % (DIRECTION_ALTERNATOR * bulletSpawnInterval[bullets[i].type])) / bulletSpawnInterval[bullets[i].type]);
    }
}

void displaceBullet(int i)
{
    int bulletType = bullets[i].type;
    int directionMode = bullets[i].directionMode;
    int movementMode = bullets[i].movementMode;

    if (directionMode == DIR_HORIZONTAL) {
        bullets[i].x += bulletXSpeed[bulletType];
    }
    else if (directionMode == DIR_VERTICAL) {
        bullets[i].x += bulletXSpeed[bulletType];
        if (movementMode == DIR_DOWN) { bullets[i].y += bulletYSpeed[bulletType]; }
        else if (movementMode == DIR_UP) { bullets[i].y -= bulletYSpeed[bulletType]; }
    }
    else if (directionMode == DIR_Y) {
        if (movementMode == DIR_DOWN) { bullets[i].y += bulletYSpeed[bulletType]; }
        else if (movementMode == DIR_UP) { bullets[i].y -= bulletYSpeed[bulletType]; }
    }
    else if (directionMode == DIR_JITTER) {
        bullets[i].x += bulletXSpeed[bulletType];
        if (movementMode == DIR_DOWN) { bullets[i].y += (flickerOffset() * bulletYSpeed[bulletType]); }
        else if (movementMode == DIR_UP) { bullets[i].y -= (flickerOffset() * bulletYSpeed[bulletType]); }
    }
}

void generateBullet()
{
    int i, spawnInterval = bulletSpawnInterval[currentBullet];
    if (!spawnInterval) { return; }
    if ((gs.frameCount + 1) % spawnInterval) { return; }
    if (currentBullet && !bulletAmmo[currentBullet]) { return; }
    if (!bulletFrames[currentBullet] || !bulletFrameInterval[currentBullet]) { return; }
    for (i = 0; i < bulletsPerType[currentBullet]; i++) {
        if (bullets[i].active != 0) { continue; }

        if (currentBullet) {
            bulletAmmo[currentBullet]--;
            if (bulletAmmo[currentBullet] < 0) {
                bulletAmmo[currentBullet] = 0;
                currentBullet = 0;
            }
        }
        bullets[i].y = (playerY + (hHeight / 2));
        bullets[i].x = playerX + hWidth;
        bullets[i].type = currentBullet;
        bullets[i].active = 1;
        setBulletSpec(i);
        seedRandom();
        bullets[i].bitmap = bulletBitmaps[bullets[i].type]
            [getAnimFrameIndexRand(gs.frameCount, bulletFrames[bullets[i].type], bulletFrameInterval[bullets[i].type])];
        playShootSound();
        break;
    }
}

void updateBullets()
{
    int i;
    for (i = 0; i < bulletsPerType[currentBullet]; i++) {
        if (!bullets[i].active) { continue; }
        bullets[i].bitmap = bulletBitmaps[bullets[i].type]
            [getAnimFrameIndex(gs.frameCount, bulletFrames[bullets[i].type], bulletFrameInterval[bullets[i].type])];
        if (!boxesCollide(bullets[i].x, bullets[i].y, bWidth(i), bHeight(i),
                     getMinX(), getMinY(), getMaxX(), getMaxY())) {
            bullets[i].active = 0;
        }
    }
    clipBulletCounts();
    if (!bulletAmmo[currentBullet]) { currentBullet = 0; }
}

void updateBulletsAndEnemies()
{
    int i, j;
    for (i = 0; i < bulletsPerType[currentBullet]; i++) {
        if (!bullets[i].active) { continue; }
        for (j = 0; j < MAX_ENEMIES; j++) {
            if (!enemies[j].active) { continue; }
            if (!boxesCollide(bullets[i].x, bullets[i].y, bWidth(i), bHeight(i),
                         enemies[j].x, enemies[j].y / SIN_SCALE, eWidth(j), eHeight(j))) { continue; }
            bullets[i].active = 0;
            enemies[j].active -= bulletDamage[bullets[i].type];
            if (enemies[j].active <= 0) {
                gs.enemiesKilled += enemyHealth[enemies[j].type];
                enemies[j].active = 0;
                enemies[j].explosionFrame = 0;
                updateScore(enemies[j].type);
                generateCollectible(enemies[j].type,
                                    enemies[j].x + (eWidth(j) / 2),
                                    enemies[j].y / SIN_SCALE + (eHeight(j) / 2));
            }
        }
    }
}

void drawBullets()
{
    int i;
    for (i = 0; i < bulletsPerType[currentBullet]; i++) {
        if (!bullets[i].active) { continue; }
        drawSpriteSize(bullets[i].bitmap, bullets[i].x, bullets[i].y, bWidth(i), bHeight(i));
        displaceBullet(i);
    }
}

#endif //_M_BULLETS_
