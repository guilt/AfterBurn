//Game Loop .

#include "pal.c"
#include "sounds.c"
#include "enemy.c"
#include "bullets.c"
#include "ebullets.c"
#include "hero.c"
#include "effects.c"
#include "stat.c"
#include "collec.c"
#include "fini.c"
#include "flog.c"
#include "hsc.c"
#include "options.c"
#include "level.c"
#include "udf.c"
#include "credits.c"
#include "vid.c"

void initEnemyTable()
{
    int i;
    int lastSin, lastCos;
    sinTable[0] = 0;
    cosTable[0] = SIN_SCALE;
    for (i = 1; i < SIN_TABLE_SIZE; i++) {
        sinTable[i] = (int)(sin((i * PI * 2) / SIN_TABLE_SIZE) * SIN_SCALE);
        cosTable[i] = (int)(cos((i * PI * 2) / SIN_TABLE_SIZE) * SIN_SCALE);
    }
    lastSin = sinTable[SIN_TABLE_SIZE - 1];
    lastCos = cosTable[SIN_TABLE_SIZE - 1];
    for (i = (SIN_TABLE_SIZE - 1); i > 0; i--) {
        sinTable[i] -= sinTable[i - 1];
        cosTable[i] -= cosTable[i - 1];
    }
    /* close the cycle: the telescoping sum of all entries equals the last
       original value; subtract it so a full lap through the table sums to 0 */
    sinTable[SIN_TABLE_SIZE - 1] -= lastSin;
    cosTable[SIN_TABLE_SIZE - 1] -= lastCos;
}

void oneTimeGameInit()
{
    gs.score = 0;
    hasScored = 0;
    gs.level = 0;
    gs.win = 0;
    gs.isInGame = 1;
    gs.lives = maxLives;
    oneTimeBulletInit();
    playMusic();
}

void initGame()
{
    gs.frameCount = 1;
    playerHealth = maxHealth;
    playerY = SCREEN_HEIGHT / 2;
    playerX = 0;
    playerFrame = 0;
    playerBlinkFrame = 0;
    soundError = 0;
    gs.isDone = 0;
    isBlinking = 0;
    gs.enemiesKilled = 0;
    memset(colls, 0, sizeof(colls));
    clearKeys();
    initBullets();
    initEnemyBullets();
    initEnemies();
    generateEnemies();
}

void checkNewGameState()
{
    if (!playerHealth && gs.lives) {
        setGameState(STATE_RESTART);
        return;
    }
    if (gs.isDone) { setGameState(STATE_GAME_OVER); }
}

void checkLevelProgress()
{
    if (gs.enemiesKilled > killsToAdvanceLevel[gs.level]) {
        setGameState(STATE_LEVEL_CLEAR);
    }
}

void updateGameState()
{
    if (gs.state == STATE_PLAYING) {
        updateStarfield();
        updateBullets();
        updateEnemies();
        updateEnemyBullets();
        updateCollectibles();
        updateBulletsAndEnemies();
        updatePlayer();
        updateEnemyBulletsAndPlayer();
    }
}

void drawGame()
{
    clearScreen();
    drawStarfield();
    drawBullets();
    drawEnemies();
    drawEnemyBullets();
    drawPlayer();
    drawCollectibles();
    drawStatusBar();
}

void updateFrameCount()
{
    gs.frameCount++;
}

void playingUpdate()
{
    updateGameState();
    updateDecliningLife();
    checkNewGameState();
    if (gs.state != STATE_PLAYING) return;
    checkLevelProgress();
    if (gs.state != STATE_PLAYING) return;
    updateFrameCount();
    checkIfDead();
}

int errorHandler(const char msg[])
{
    logWrite("%s", msg);
    unloadAssets();
    writePreferences();
    logWrite("Quitting prematurely.");
    logClose();
    alfont_exit();
    musicExit();
    allegro_exit();
    return RESULT_ERROR;
}


