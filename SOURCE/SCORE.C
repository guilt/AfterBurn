#ifndef _M_SCOR_
#define _M_SCOR_

#include "decl.h"

void checkIncrementLife()
{
    if (gs.lives < maxPossibleLives && gs.score > LIFE_GIVING_SCORE) {
        gs.lives++;
        gs.score = 0;
    }
}

void addCollectibleScore(int i)
{
    if (!colls[i].active) { return; }

    if (colls[i].type == COLLECTIBLE_AMMO) {
        if (colls[i].itemIndex) {
            bulletAmmo[colls[i].itemIndex] += ammoCollectQuantity[colls[i].itemIndex];
        } else {
            bulletDamage[colls[i].itemIndex] += HEALTH_THRESHOLD;
        }
        gs.score += ammoCollectScore[colls[i].itemIndex];
    }
    else if (colls[i].type == COLLECTIBLE_OTHER) {
        playerHealth += collectHealthBonus[colls[i].itemIndex];
        if (playerHealth > maxHealth) { playerHealth = maxHealth; }
        gs.lives += collectLivesBonus[colls[i].itemIndex];
        if (gs.lives > maxPossibleLives) { gs.lives = maxPossibleLives; }
        gs.score += collectScore[colls[i].itemIndex];
    }
    colls[i].active = 0;
    hasScored = 1;
}

void updateScore(int type)
{
    gs.score += SCORE_MULTIPLIER * enemyDamage[type];
    hasScored = 1;
    if (gs.score > MAX_SCORE) { gs.score = MAX_SCORE; }
    checkIncrementLife();
}

#endif //_M_SCOR_
