#ifndef _M_FINI_
#define _M_FINI_

#include <stdarg.h>
#include <string.h>
#include "decl.h"
#include "files.h"

static int readIntTruncate(FILE* f)
{
    char buf[64];
    if (fscanf(f, "%63s", buf) != 1) return 0;
    char* dot = strchr(buf, '.');
    if (dot) *dot = '\0';
    return atoi(buf);
}

static int readFixedPoint(FILE* f)
{
    char buf[64];
    if (fscanf(f, "%63s", buf) != 1) return 0;
    if (strcmp(buf, "-1") == 0) return -1;
    char* dot = strchr(buf, '.');
    if (!dot) return atoi(buf) * SIN_SCALE;
    int whole = atoi(buf);
    int frac = 0, digits = 0;
    char* p = dot + 1;
    while (*p >= '0' && *p <= '9' && digits < 4) { frac = frac * 10 + (*p - '0'); digits++; p++; }
    int divisor = 1;
    { int k; for (k = 0; k < digits; k++) divisor *= 10; }
    if (whole < 0) {
        whole = -whole;
        return -(whole * SIN_SCALE + frac * SIN_SCALE / divisor);
    }
    return whole * SIN_SCALE + frac * SIN_SCALE / divisor;
}

static void normalizePath(char* p);

void iniOpenFile()
{
    int i, j;
    iniFile = fileOpen(iniPath, "r");
    if (!iniFile) {
        logWrite("Couldn't read INI file: %s", iniPath);
        return;
    }
    logWrite("Reading INI file: %s", iniPath);

    // Logo & gameFont
    fscanf(iniFile, "%s", logoPath);
    fscanf(iniFile, "%s", fontPath);
    fscanf(iniFile, " %d %d %d", &activeColorR, &activeColorG, &activeColorB);
    fscanf(iniFile, " %d %d %d", &passiveColorR, &passiveColorG, &passiveColorB);

    // Ship
    fscanf(iniFile, "%s", shipPath);
    fscanf(iniFile, " %d %d %d %d %d", &shipFrames, &shipAnimSpeed, &maxHealth, &maxLives, &maxPossibleLives);
    fscanf(iniFile, "%s", shipBlinkPath);
    fscanf(iniFile, " %d", &shipBlinkFrames);
    fscanf(iniFile, "%s", shipBlankPath);
    fscanf(iniFile, " %d", &maxBlinkActiveFrames);

    // Levels
    fscanf(iniFile, " %d", &numLevels);
    for (i = 0; i < numLevels; i++) {
        fscanf(iniFile, " %d", &killsToAdvanceLevel[i]);
    }

    // Bullets
    fscanf(iniFile, "%s", bulletPath);
    fscanf(iniFile, " %d", &numBulletTypes);
    for (i = 0; i < numBulletTypes; i++) {
        fscanf(iniFile, "%s", bulletNames[i]);
        replaceChar(bulletNames[i], ARROW_SEPARATOR, SPACE_CHAR);
        fscanf(iniFile, " %d %d %d %d %d %d %d %d %d",
               &bulletFrames[i], &bulletFrameInterval[i], &bulletsPerType[i],
               &bulletXSpeed[i], &bulletYSpeed[i], &bulletSpawnInterval[i], &bulletDirection[i],
               &defaultBulletAmmo[i], &maxBulletAmmo[i]);
        defaultBulletDamage[i] = readIntTruncate(iniFile);
        maxBulletDamage[i] = readIntTruncate(iniFile);
    }

    // Enemy bullets
    fscanf(iniFile, "%s", enemyBulletPath);
    fscanf(iniFile, " %d", &numEnemyBulletTypes);
    for (i = 0; i < numEnemyBulletTypes; i++) {
        fscanf(iniFile, " %d %d %d %d",
               &enemyBulletFrames[i], &enemyBulletFrameInterval[i], &enemyBulletXSpeed[i], &enemyBulletSpawnInterval[i]);
        enemyBulletDamage[i] = readIntTruncate(iniFile);
    }

    // bulletAmmo
    fscanf(iniFile, "%s", ammoPath);
    for (i = 0; i < numBulletTypes; i++) {
        fscanf(iniFile, " %d %d %d %d %d",
               &ammoCollectFrames[i], &ammoCollectFrameInterval[i], &ammoCollectDuration[i], &ammoCollectQuantity[i], &ammoCollectScore[i]);
    }

    // Other collectibles
    fscanf(iniFile, "%s", collectiblePath);
    fscanf(iniFile, " %d", &numCollectibles);
    for (i = 0; i < numCollectibles; i++) {
        fscanf(iniFile, " %d %d %d %d %d %d",
               &collectFrames[i], &collectFrameInterval[i], &collectHealthBonus[i], &collectLivesBonus[i], &collectScore[i], &collectibleDuration[i]);
    }

    // Enemies
    fscanf(iniFile, "%s", enemyPath);
    fscanf(iniFile, " %d", &numEnemyTypes);
    for (i = 0; i < numEnemyTypes; i++) {
        fscanf(iniFile, " %d %d %d %d %d %d",
               &enemyFrames[i], &enemyFrameInterval[i], &enemyHealth[i], &enemyDamage[i],
               &enemyHasMovement[i], &enemyBulletTypeByEnemy[i]);
        enemyXSpeed[i] = readIntTruncate(iniFile);
        enemyYSpeed[i] = readIntTruncate(iniFile);
    }

    // Enemy map per level
    for (i = 0; i < numLevels; i++) {
        for (j = 0; j < numEnemyTypes; j++) {
            fscanf(iniFile, " %d", &enemyLevelMap[i][j]);
        }
    }

    // Effects
    fscanf(iniFile, " %d", &numStars);
    fscanf(iniFile, "%s", explosionPath);
    fscanf(iniFile, " %d", &numExplodeFrames);

    // Statistics statIcons
    fscanf(iniFile, "%s", statIconPath);

    // Sounds & music
    fscanf(iniFile, "%s", levelMusicPath);
    fscanf(iniFile, "%s", menuMusicPath);
    fscanf(iniFile, "%s", shootSoundPath);
    fscanf(iniFile, "%s", enemyShootSoundPath);
    fscanf(iniFile, "%s", explodeSoundPath);
    fscanf(iniFile, "%s", playerDieSoundPath);
    fscanf(iniFile, "%s", collectSoundPath);
    fscanf(iniFile, "%s", winSoundPath);
    fscanf(iniFile, " %d %d %u %u", &isSoundEnabled, &isMusicEnabled, &DVOL, &MVOL);

    // Scores & stats
    fscanf(iniFile, "%s", highScorePath);
    fscanf(iniFile, "%d", &showStats);

    // Videos
    fscanf(iniFile, "%d", &numVideos);
    for (i = 0; i < numVideos; i++) {
        fscanf(iniFile, "%s %s %d %d",
               videos[i].filename, videos[i].password,
               &videos[i].animIndex, &videos[i].videoIndex);
        replaceChar(videos[i].filename, ALT_DIR_DELIMITER, DIR_DELIMITER);
        replaceChar(videos[i].filename, ARROW_SEPARATOR, SPACE_CHAR);
        strToUpper(videos[i].filename);
        replaceChar(videos[i].password, ARROW_SEPARATOR, SPACE_CHAR);
    }

    // Localised strings (encoded with ARROW_SEPARATOR instead of spaces in the INI)
    for (i = 0; i < NUM_MENU_ITEMS; i++) {
        fscanf(iniFile, "%s", menu[i]);
        replaceChar(menu[i], ARROW_SEPARATOR, SPACE_CHAR);
    }
    for (i = 0; i < NUM_OPTIONS_MENU_ITEMS; i++) {
        fscanf(iniFile, "%s", optionsMenu[i]);
        replaceChar(optionsMenu[i], ARROW_SEPARATOR, SPACE_CHAR);
    }
    for (i = 0; i < NUM_OPTION_STATES; i++) {
        fscanf(iniFile, "%s", optionStates[i]);
        replaceChar(optionStates[i], ARROW_SEPARATOR, SPACE_CHAR);
    }
    char* uiStrings[] = {
        gameOverStr, pauseStr, quitDialogStr, anyKeyStr, infiniteStr, unknownStr,
        hsNameStr, hsLevelStr, hsScoreStr, hsPromptStr, anonymousStr,
        unimplementedStr, levelClearStr, winStr
    };
    for (i = 0; i < (int)(sizeof(uiStrings) / sizeof(uiStrings[0])); i++) {
        fscanf(iniFile, "%s", uiStrings[i]);
        replaceChar(uiStrings[i], ARROW_SEPARATOR, SPACE_CHAR);
    }

    // Colour parameters (stored as decimal strings in INI)
    gameRScaleOverride = readFixedPoint(iniFile);
    gameGScaleOverride = readFixedPoint(iniFile);
    gameBScaleOverride = readFixedPoint(iniFile);
    starRScaleOverride = readFixedPoint(iniFile);
    starGScaleOverride = readFixedPoint(iniFile);
    starBScaleOverride = readFixedPoint(iniFile);
    govRScaleOverride = readFixedPoint(iniFile);
    govGScaleOverride = readFixedPoint(iniFile);
    govBScaleOverride = readFixedPoint(iniFile);

    { char endTag[STRING_BUFFER_SIZE]; fscanf(iniFile, "%s", endTag); }

    logWrite("Read INI file: %s", iniPath);
    fclose(iniFile);

    /* normalize INI-read paths to 8.3 format */
    {
        char* iniPaths[] = {
            logoPath, fontPath, shipPath, shipBlinkPath, shipBlankPath,
            bulletPath, enemyBulletPath, ammoPath, collectiblePath, enemyPath,
            explosionPath, statIconPath, levelMusicPath, menuMusicPath,
            shootSoundPath, enemyShootSoundPath, explodeSoundPath, playerDieSoundPath,
            collectSoundPath, winSoundPath, highScorePath
        };
        int i;
        for (i = 0; i < (int)(sizeof(iniPaths) / sizeof(iniPaths[0])); i++) {
            normalizePath(iniPaths[i]);
        }
    }

    /* overlay PRF (user preferences) over theme INI values */
    iniFile = fileOpen(prfPath, "r");
    if (iniFile) {
        logWrite("Overlaying PRF file: %s", prfPath);
        fscanf(iniFile, " %d %d %d", &activeColorR, &activeColorG, &activeColorB);
        fscanf(iniFile, " %d %d %d", &passiveColorR, &passiveColorG, &passiveColorB);
        fscanf(iniFile, " %d %d %u %u", &isSoundEnabled, &isMusicEnabled, &DVOL, &MVOL);
        fscanf(iniFile, " %d", &showStats);
        gameRScaleOverride = readIntTruncate(iniFile);
        gameGScaleOverride = readIntTruncate(iniFile);
        gameBScaleOverride = readIntTruncate(iniFile);
        starRScaleOverride = readIntTruncate(iniFile);
        starGScaleOverride = readIntTruncate(iniFile);
        starBScaleOverride = readIntTruncate(iniFile);
        govRScaleOverride = readIntTruncate(iniFile);
        govGScaleOverride = readIntTruncate(iniFile);
        govBScaleOverride = readIntTruncate(iniFile);
        fscanf(iniFile, " %d %d %d", &savedResW, &savedResH, &savedGfxMode);
        fclose(iniFile);
        logWrite("Overlaid PRF file: %s", prfPath);
    }
}

static void normalizePath(char* p)
{
    convertToLocalPath(p);
    strToUpper(p);
    prependDir(p, getAppDataDir());
}

static void prfWrite(const char* format, ...)
{
    if (!iniFile) { return; }
    va_list args;
    va_start(args, format);
    vfprintf(iniFile, format, args);
    va_end(args);
    fputc('\n', iniFile);
}

static void writeColorOrSentinel(int overrideParam, int red, int green, int blue)
{
    int R = -1, G = -1, B = -1;
    if (overrideParam >= 0) { R = red; G = green; B = blue; }
    prfWrite("%d %d %d", R, G, B);
}

void writePreferences()
{
    iniFile = fileOpen(prfPath, "w");
    if (!iniFile) {
        logWrite("Couldn't write PRF file: %s", prfPath);
        return;
    }
    logWrite("Writing PRF file: %s", prfPath);

    prfWrite("%d %d %d", activeColorR, activeColorG, activeColorB);
    prfWrite("%d %d %d", passiveColorR, passiveColorG, passiveColorB);
    prfWrite("%d %d %u %u", isSoundEnabled, isMusicEnabled, DVOL, MVOL);
    prfWrite("%d", showStats);
    writeColorOrSentinel(gameRScaleOverride, gameRScale, gameGScale, gameBScale);
    writeColorOrSentinel(starRScaleOverride, starRScale, starGScale, starBScale);
    writeColorOrSentinel(govRScaleOverride, govRScale, govGScale, govBScale);
    prfWrite("%d %d %d", screenWidth, screenHeight, savedGfxMode);

    fclose(iniFile);
    logWrite("Wrote PRF file: %s", prfPath);
}

#endif //_M_FINI
