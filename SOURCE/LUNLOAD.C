#include "decl.h"
#include "files.h"
#include "flog.c"
#include "fhsc.c"
#include "fini.c"

void unloadAssets();

void destroyMod(MUSIC* musicHandle)
{
    unload_duh(musicHandle);
}

MUSIC* loadMod(char filePath[])
{
    MUSIC* musicHandle;
    if ((musicHandle = dumbLoadS3m(filePath))) { return musicHandle; }
    if ((musicHandle = dumbLoadIt(filePath)))  { return musicHandle; }
    if ((musicHandle = dumbLoadXm(filePath)))  { return musicHandle; }
    if ((musicHandle = dumbLoadMod(filePath))) { return musicHandle; }
    return NULL;
}

void writeStatus(void* pointer, const char* message)
{
    if (pointer) {
        logWrite("%s: OK", message);
        return;
    }
    logWrite("Error processing: %s", message);
    unloadAssets();
    writePreferences();
    logClose();
    alfont_exit();
    musicExit();
    allegro_exit();
    exit(RESULT_ERROR);
}

void unloadAssets()
{
    int i, j;
    logWrite("Unloading...");

    writeHighScores();

    if (!soundError) {
        destroy_sample(wonSample);
        destroy_sample(collectSample);
        destroy_sample(playerDieSample);
        destroy_sample(explodeSample);
        for (i = 0; i < numEnemyBulletTypes; i++) { destroy_sample(enemyShootSamples[i]); }
        for (i = 0; i < numBulletTypes; i++)  { destroy_sample(shootSamples[i]); }
        destroyMod(menuMusic);
        for (i = 0; i < numLevels; i++) { destroyMod(levelMusic[i]); }
    }

    for (i = 0; i < NUM_ICONS; i++)   { destroy_bitmap(statIcons[i]); }
    for (i = 0; i < numExplodeFrames; i++) { destroy_bitmap(explodeFrames[i]); }

    for (i = 0; i < numEnemyTypes; i++) {
        for (j = 0; j < enemyFrames[i]; j++) { destroy_bitmap(enemyAnimations[i][j]); }
    }
    for (i = 0; i < NUM_COLLECTIBLE_TYPES; i++) {
        for (j = 0; j < collectFrames[i]; j++) { destroy_bitmap(collectBitmaps[i][j]); }
    }
    for (i = 0; i < numBulletTypes; i++) {
        for (j = 0; j < ammoCollectFrames[i]; j++) { destroy_bitmap(ammoCollectBitmaps[i][j]); }
    }
    for (i = 0; i < numEnemyBulletTypes; i++) {
        for (j = 0; j < enemyBulletFrames[i]; j++) { destroy_bitmap(enemyBulletBitmaps[i][j]); }
    }
    for (i = 0; i < numBulletTypes; i++) {
        for (j = 0; j < bulletFrames[i]; j++) { destroy_bitmap(bulletBitmaps[i][j]); }
    }

    destroy_bitmap(playerBlankBitmap);
    for (i = 0; i < shipBlinkFrames; i++) { destroy_bitmap(playerBlinkBitmaps[i]); }
    for (i = 0; i < shipFrames; i++)   { destroy_bitmap(playerShipBitmaps[i]); }
    alfont_destroy_font(creditsFont);
    alfont_destroy_font(highScoresValuesFont);
    alfont_destroy_font(gameFont);
    destroy_bitmap(logo);
    destroy_bitmap(virtualBuffer); virtualBuffer = NULL;
    destroy_bitmap(backBuffer); backBuffer = NULL;

    logWrite("Unloading done.");
}

void loadAssets()
{
    int i, j;
    char filePath[STRING_BUFFER_SIZE];
    logWrite("Loading...");

    virtualBuffer = create_bitmap(640, 480);
    writeStatus(virtualBuffer, "Virtual Canvas");
    backBuffer = create_bitmap(screenWidth, screenHeight);
    writeStatus(backBuffer, "Back Buffer");

    logo = loadBitmap(logoPath, palette);
    writeStatus(logo, logoPath);
    gameFont = loadFont(fontPath);
    writeStatus(gameFont, fontPath);
    creditsFont = loadFont("DATA\\GLOBAL\\FONT.TTF");
    writeStatus(creditsFont, "Credits Font");
    alfont_set_font_size(creditsFont, FONT_SIZE);
    highScoresValuesFont = loadFont("DATA\\GLOBAL\\FONT.TTF");
    writeStatus(highScoresValuesFont, "High Score Values Font");
    alfont_set_font_size(highScoresValuesFont, FONT_SIZE);

    for (i = 0; i < shipFrames; i++) {
        sprintf(filePath, shipPath, i + 1);
        playerShipBitmaps[i] = loadBitmap(filePath, palette);
        writeStatus(playerShipBitmaps[i], filePath);
    }
    for (i = 0; i < shipBlinkFrames; i++) {
        sprintf(filePath, shipBlinkPath, i + 1);
        playerBlinkBitmaps[i] = loadBitmap(filePath, palette);
        writeStatus(playerBlinkBitmaps[i], filePath);
    }
    playerBlankBitmap = loadBitmap(shipBlankPath, palette);
    writeStatus(playerBlankBitmap, shipBlankPath);

    for (i = 0; i < numBulletTypes; i++) {
        for (j = 0; j < bulletFrames[i]; j++) {
            sprintf(filePath, bulletPath, i + 1, j + 1);
            bulletBitmaps[i][j] = loadBitmap(filePath, palette);
            writeStatus(bulletBitmaps[i][j], filePath);
        }
    }
    for (i = 0; i < numEnemyBulletTypes; i++) {
        for (j = 0; j < enemyBulletFrames[i]; j++) {
            sprintf(filePath, enemyBulletPath, i + 1, j + 1);
            enemyBulletBitmaps[i][j] = loadBitmap(filePath, palette);
            writeStatus(enemyBulletBitmaps[i][j], filePath);
        }
    }
    for (i = 0; i < numBulletTypes; i++) {
        for (j = 0; j < ammoCollectFrames[i]; j++) {
            sprintf(filePath, ammoPath, i + 1, j + 1);
            ammoCollectBitmaps[i][j] = loadBitmap(filePath, palette);
            writeStatus(ammoCollectBitmaps[i][j], filePath);
        }
    }
    for (i = 0; i < numCollectibles; i++) {
        for (j = 0; j < collectFrames[i]; j++) {
            sprintf(filePath, collectiblePath, i + 1, j + 1);
            collectBitmaps[i][j] = loadBitmap(filePath, palette);
            writeStatus(collectBitmaps[i][j], filePath);
        }
    }
    for (i = 0; i < numEnemyTypes; i++) {
        for (j = 0; j < enemyFrames[i]; j++) {
            sprintf(filePath, enemyPath, i + 1, j + 1);
            enemyAnimations[i][j] = loadBitmap(filePath, palette);
            writeStatus(enemyAnimations[i][j], filePath);
        }
    }
    for (i = 0; i < numExplodeFrames; i++) {
        sprintf(filePath, explosionPath, i + 1);
        explodeFrames[i] = loadBitmap(filePath, palette);
        writeStatus(explodeFrames[i], filePath);
    }
    for (i = 0; i < NUM_ICONS; i++) {
        sprintf(filePath, statIconPath, i + 1);
        statIcons[i] = loadBitmap(filePath, palette);
        writeStatus(statIcons[i], filePath);
    }

    if (!soundError) {
        for (i = 0; i < numLevels; i++) {
            sprintf(filePath, levelMusicPath, i + 1);
            levelMusic[i] = loadMod(filePath);
            writeStatus(levelMusic[i], filePath);
        }
        menuMusic = loadMod(menuMusicPath);
        writeStatus(menuMusic, menuMusicPath);
        for (i = 0; i < numBulletTypes; i++) {
            sprintf(filePath, shootSoundPath, i + 1);
            shootSamples[i] = loadSample(filePath);
            writeStatus(shootSamples[i], filePath);
        }
        for (i = 0; i < numEnemyBulletTypes; i++) {
            sprintf(filePath, enemyShootSoundPath, i + 1);
            enemyShootSamples[i] = loadSample(filePath);
            writeStatus(enemyShootSamples[i], filePath);
        }
        explodeSample = loadSample(explodeSoundPath);
        writeStatus(explodeSample, explodeSoundPath);
        playerDieSample = loadSample(playerDieSoundPath);
        writeStatus(playerDieSample, playerDieSoundPath);
        collectSample = loadSample(collectSoundPath);
        writeStatus(collectSample, collectSoundPath);
        wonSample = loadSample(winSoundPath);
        writeStatus(wonSample, winSoundPath);
    }

    initRandomHighScores();
    readHighScores();
    logWrite("Loading done.");
}
