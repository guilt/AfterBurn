#include "decl.h"
#include "effects.c"
#include "collisio.c"
#include "ebullets.c"

void initEnemies()
{
    int i;
    memset(enemies, 0, sizeof(enemies));
    for (i = 0; i < MAX_ENEMIES; i++) { enemies[i].explosionFrame = -1; }
    seedRandom();
}

void generateEnemies()
{
    int i, j;
    seedRandom();
    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].type = rand() % numEnemyTypes;
        enemies[i].active = enemyHealth[enemies[i].type];
        enemies[i].movementType = ENEMY_HORIZONTAL;
        enemies[i].oscillationTimer = (rand() % SIN_TABLE_SIZE);
        if (enemyHasMovement[enemies[i].type]) {
            enemies[i].movementType = ((rand() + gs.frameCount) % ENEMY_MOVEMENT_TYPES);
        }
        enemies[i].bitmap = enemyAnimations[enemies[i].type]
            [getAnimFrameIndex(gs.frameCount, enemyFrames[enemies[i].type], enemyFrameInterval[enemies[i].type])];
        enemies[i].x = SCREEN_WIDTH + (rand() % ENEMY_X_LIMIT);
        enemies[i].y = (rand() % (SCREEN_HEIGHT - 1 - eHeight(i))) * SIN_SCALE;
        enemies[i].explosionFrame = -1;

        for (j = 0; j < i; j++) {
            if (enemies[j].active &&
                boxesCollide(enemies[i].x, enemies[i].y / SIN_SCALE, eWidth(i), eHeight(i),
                        enemies[j].x, enemies[j].y / SIN_SCALE, eWidth(j), eHeight(j))) {
                enemies[j].active = 0;
            }
            if (!enemyLevelMap[gs.level][enemies[j].type] && enemies[j].active) {
                enemies[j].active = 0;
            }
        }
    }
}

void horizontalDisplace(int i)
{
    enemies[i].x -= enemyXSpeed[enemies[i].type];
}

void displaceEnemy(int i)
{
    if (enemies[i].movementType == ENEMY_HORIZONTAL) {
        horizontalDisplace(i);
    }
    if (enemies[i].movementType == ENEMY_LINEAR) {
        horizontalDisplace(i);
        enemies[i].x -= enemyXSpeed[enemies[i].type] * cosTable[(gs.frameCount + enemies[i].oscillationTimer) % SIN_TABLE_SIZE] / SIN_SCALE;
        enemies[i].y -= sinTable[(gs.frameCount + enemies[i].oscillationTimer) % SIN_TABLE_SIZE] * enemyYSpeed[enemies[i].type];
    }
    if (enemies[i].movementType == ENEMY_SINE) {
        horizontalDisplace(i);
        enemies[i].y -= sinTable[(gs.frameCount + enemies[i].oscillationTimer) % SIN_TABLE_SIZE] * enemyYSpeed[enemies[i].type];
    }
}

void updateEnemies()
{
    int i, inactiveCount = 0;
    for (i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].bitmap = enemyAnimations[enemies[i].type]
            [getAnimFrameIndex(gs.frameCount, enemyFrames[enemies[i].type], enemyFrameInterval[enemies[i].type])];
        if (enemies[i].active) {
            displaceEnemy(i);
        }
        else {
            inactiveCount++;
            if (enemies[i].explosionFrame >= 0 && enemies[i].explosionFrame < (EXPLOSION_MULTIPLIER * numExplodeFrames)) {
                horizontalDisplace(i);
            }
        }
        if (enemies[i].x < 0) {
            enemies[i].active = 0;
            enemies[i].explosionFrame = -1;
        }
    }
    if (inactiveCount == MAX_ENEMIES) {
        generateEnemies();
    }
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            generateEnemyBullet(i);
        }
    }
}

void drawEnemies()
{
    int i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            drawSprite(enemies[i].bitmap, enemies[i].x, enemies[i].y / SIN_SCALE);
        }
        else {
            checkExplode(i);
        }
    }
}
