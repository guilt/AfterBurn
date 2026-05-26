//File Names Header .

#ifndef _M_FILES_
#define _M_FILES_

#include "sysdefs.h"
#ifdef _WIN32
#include "direct.h"
#endif

//Files .
char configPath[STRING_BUFFER_SIZE]={"DATA\\GLOBAL\\ALLEGRO.CFG"};
char iniPath[STRING_BUFFER_SIZE]={"DATA\\DEFAULT.INI"};
char prfPath[STRING_BUFFER_SIZE]={""};
char dataDir[STRING_BUFFER_SIZE]={""};
char themeDataDir[STRING_BUFFER_SIZE]={""};
char userDataDir[STRING_BUFFER_SIZE]={""};
char logPath[STRING_BUFFER_SIZE]={"GAMELOG.TXT"};
char highScorePath[STRING_BUFFER_SIZE]={"DATA\\GLOBAL\\HSCORES.TXT"};
char userHighScorePath[STRING_BUFFER_SIZE]={"DEFAULT.HSC"};
char logoPath[STRING_BUFFER_SIZE]={"DATA\\GLOBAL\\LOGO.BMP"};
char fontPath[STRING_BUFFER_SIZE]={"DATA\\GLOBAL\\GAMEFONT.TTF"};
char shipPath[STRING_BUFFER_SIZE]={"DATA\\HERO\\SHIP%02d.BMP"};
char shipBlinkPath[STRING_BUFFER_SIZE]={"DATA\\HERO\\SHIPB%02d.BMP"};
char shipBlankPath[STRING_BUFFER_SIZE]={"DATA\\HERO\\SHBLANK.BMP"};
char bulletPath[STRING_BUFFER_SIZE]={"DATA\\BULLETS\\BULL%02d%02d.BMP"};
char enemyBulletPath[STRING_BUFFER_SIZE]={"DATA\\EBULLETS\\EBUL%02d%02d.BMP"};
char ammoPath[STRING_BUFFER_SIZE]={"DATA\\COLLEC\\BULLETAMMO\\AM%02d%02d.BMP"};
char collectiblePath[STRING_BUFFER_SIZE]={"DATA\\COLLEC\\OTHERS\\COL%02d%02d.BMP"};
char enemyPath[STRING_BUFFER_SIZE]={"DATA\\ENEMIES\\ENEM%02d%02d.BMP"};
char explosionPath[STRING_BUFFER_SIZE]={"DATA\\EXPLODE\\EXPLO%02d.BMP"};
char statIconPath[STRING_BUFFER_SIZE]={"DATA\\STAT\\ICON%02d.BMP"};
char levelMusicPath[STRING_BUFFER_SIZE]={"DATA\\LEVELMUSIC\\LEVELMUSIC%02d.MUS"};
char menuMusicPath[STRING_BUFFER_SIZE]={"DATA\\LEVELMUSIC\\MENUMUSIC.MUS"};
char collectSoundPath[STRING_BUFFER_SIZE]={"DATA\\SFX\\COLLECTSAMPLE.WAV"};
char winSoundPath[STRING_BUFFER_SIZE]={"DATA\\SFX\\WONSAMPLE.WAV"};
char shootSoundPath[STRING_BUFFER_SIZE]={"DATA\\SFX\\SHOOTSAMPLES%02d.WAV"};
char enemyShootSoundPath[STRING_BUFFER_SIZE]={"DATA\\SFX\\ENEMYSHOOTSAMPLES%02d.WAV"};
char explodeSoundPath[STRING_BUFFER_SIZE]={"DATA\\SFX\\EXPLODE.WAV"};
char playerDieSoundPath[STRING_BUFFER_SIZE]={"DATA\\SFX\\GEXPLODE.WAV"};
char screenshotPath[STRING_BUFFER_SIZE]={"SCR%05d.PCX"};

static void strToUpper(char* str)
{
    int i;
    for (i = 0; str[i]; i++) {
        if (str[i] == '%' && str[i + 1] && str[i + 1] != '%') {
            i++;
            while (str[i] && !((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))) {
                i++;
            }
            continue;
        }
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
    }
}

//File I/O helpers — auto-uppercase paths transparently
static FILE* fileOpen(const char* path, const char* mode)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return fopen(buf, mode);
}

static BITMAP* loadBitmap(const char* path, RGB* pal)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return load_bitmap(buf, pal);
}

static int saveBitmap(const char* path, BITMAP* bmp, RGB* pal)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return save_bitmap(buf, bmp, pal);
}

static SAMPLE* loadSample(const char* path)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return load_sample(buf);
}

static ALFONT_FONT* loadFont(const char* path)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return alfont_load_font(buf);
}

static DATAFILE* loadDatafile(const char* path)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return load_datafile(buf);
}

static DUH* dumbLoadS3m(const char* path)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return dumb_load_s3m(buf);
}

static DUH* dumbLoadIt(const char* path)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return dumb_load_it(buf);
}

static DUH* dumbLoadXm(const char* path)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return dumb_load_xm(buf);
}

static DUH* dumbLoadMod(const char* path)
{
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return dumb_load_mod(buf);
}

void replaceChar(char str[], char from, char to)
{
    int i = 0;
    for (; str[i]; i++) {
        if (str[i] == from) {
            str[i] = to;
        }
    }
}

void convertToLocalPath(char str[])
{
    replaceChar(str, ALT_DIR_DELIMITER, DIR_DELIMITER);
    replaceChar(str, ARROW_SEPARATOR, SPACE_CHAR);
}

void convertToWindowsPath(char str[])
{
    replaceChar(str, DIR_DELIMITER, ALT_DIR_DELIMITER);
    replaceChar(str, SPACE_CHAR, ARROW_SEPARATOR);
}

void initUserDataDir()
{
    char* env;
    env = getenv("XDG_DATA_HOME");
    if (env && *env) {
        sprintf(userDataDir, "%s%cAfterBurn%c", env, DIR_DELIMITER, DIR_DELIMITER);
        mkdir(userDataDir);
        return;
    }
    env = getenv("APPDATA");
    if (env && *env) {
        sprintf(userDataDir, "%s%cAfterBurn%c", env, DIR_DELIMITER, DIR_DELIMITER);
        mkdir(userDataDir);
        return;
    }
    env = getenv("HOME");
    if (env && *env) {
        sprintf(userDataDir, "%s%c.AfterBurn%c", env, DIR_DELIMITER, DIR_DELIMITER);
        mkdir(userDataDir);
        return;
    }
    env = getenv("USERPROFILE");
    if (env && *env) {
        sprintf(userDataDir, "%s%c.AfterBurn%c", env, DIR_DELIMITER, DIR_DELIMITER);
        mkdir(userDataDir);
        return;
    }
    userDataDir[0] = '\0';
}

int fileExists(const char* filename)
{
    FILE* fp = fileOpen(filename, "rb");
    if (fp) {
        fclose(fp);
        return 1;
    }
    return 0;
}

#define getAppDataDir() dataDir
#define getUserDataDir() userDataDir
#define getThemeDataDir() themeDataDir

#define getAppDataPath(p) (getAppDataBuf[0] = '\0', sprintf(getAppDataBuf, "%s%s", getAppDataDir(), p), getAppDataBuf)
#define getUserDataPath(p) (getUserDataBuf[0] = '\0', sprintf(getUserDataBuf, "%s%s", getUserDataDir(), p), getUserDataBuf)
static char getAppDataBuf[STRING_BUFFER_SIZE];
static char getUserDataBuf[STRING_BUFFER_SIZE];

static void prependDir(char* path, const char* dir)
{
    char buf[STRING_BUFFER_SIZE];
    if (dir[0] && strncmp(path, dir, strlen(dir)) != 0) {
        sprintf(buf, "%s%s", dir, path); strcpy(path, buf);
    }
}

void normalizePaths()
{
    char buf[STRING_BUFFER_SIZE];
    initUserDataDir();
    convertToLocalPath(configPath); strToUpper(configPath); prependDir(configPath, getAppDataDir());
    convertToLocalPath(iniPath); strToUpper(iniPath); prependDir(iniPath, getAppDataDir());
    {
        char* lastSlash = strrchr(iniPath, DIR_DELIMITER);
        char* baseName = lastSlash ? (lastSlash + 1) : iniPath;
        if (lastSlash) {
            int dirLen = (int)(lastSlash - iniPath + 1);
            memcpy(themeDataDir, iniPath, dirLen);
            themeDataDir[dirLen] = '\0';
        } else {
            themeDataDir[0] = '\0';
        }
        strcpy(prfPath, baseName);
        char* dot = strrchr(prfPath, '.');
        if (dot) { strcpy(dot, ".PRF"); }
        else { strcat(prfPath, ".PRF"); }
        if (getUserDataDir()[0]) { sprintf(buf, "%s%s", getUserDataDir(), prfPath); strcpy(prfPath, buf); }
    }
    convertToLocalPath(logPath); strToUpper(logPath);
    convertToLocalPath(highScorePath); strToUpper(highScorePath); prependDir(highScorePath, getAppDataDir());
    convertToLocalPath(userHighScorePath); strToUpper(userHighScorePath); prependDir(userHighScorePath, getUserDataDir());
    convertToLocalPath(logoPath); strToUpper(logoPath); prependDir(logoPath, getAppDataDir());
    convertToLocalPath(fontPath); strToUpper(fontPath); prependDir(fontPath, getAppDataDir());
    convertToLocalPath(shipPath); strToUpper(shipPath); prependDir(shipPath, getAppDataDir());
    convertToLocalPath(shipBlinkPath); strToUpper(shipBlinkPath); prependDir(shipBlinkPath, getAppDataDir());
    convertToLocalPath(shipBlankPath); strToUpper(shipBlankPath); prependDir(shipBlankPath, getAppDataDir());
    convertToLocalPath(bulletPath); strToUpper(bulletPath); prependDir(bulletPath, getAppDataDir());
    convertToLocalPath(enemyBulletPath); strToUpper(enemyBulletPath); prependDir(enemyBulletPath, getAppDataDir());
    convertToLocalPath(ammoPath); strToUpper(ammoPath); prependDir(ammoPath, getAppDataDir());
    convertToLocalPath(collectiblePath); strToUpper(collectiblePath); prependDir(collectiblePath, getAppDataDir());
    convertToLocalPath(enemyPath); strToUpper(enemyPath); prependDir(enemyPath, getAppDataDir());
    convertToLocalPath(explosionPath); strToUpper(explosionPath); prependDir(explosionPath, getAppDataDir());
    convertToLocalPath(statIconPath); strToUpper(statIconPath); prependDir(statIconPath, getAppDataDir());
    convertToLocalPath(levelMusicPath); strToUpper(levelMusicPath); prependDir(levelMusicPath, getAppDataDir());
    convertToLocalPath(menuMusicPath); strToUpper(menuMusicPath); prependDir(menuMusicPath, getAppDataDir());
    convertToLocalPath(collectSoundPath); strToUpper(collectSoundPath); prependDir(collectSoundPath, getAppDataDir());
    convertToLocalPath(winSoundPath); strToUpper(winSoundPath); prependDir(winSoundPath, getAppDataDir());
    convertToLocalPath(shootSoundPath); strToUpper(shootSoundPath); prependDir(shootSoundPath, getAppDataDir());
    convertToLocalPath(enemyShootSoundPath); strToUpper(enemyShootSoundPath); prependDir(enemyShootSoundPath, getAppDataDir());
    convertToLocalPath(explodeSoundPath); strToUpper(explodeSoundPath); prependDir(explodeSoundPath, getAppDataDir());
    convertToLocalPath(playerDieSoundPath); strToUpper(playerDieSoundPath); prependDir(playerDieSoundPath, getAppDataDir());
    convertToLocalPath(screenshotPath); strToUpper(screenshotPath); prependDir(screenshotPath, getUserDataDir());
}

void denormalizePaths()
{
    convertToWindowsPath(configPath);
    convertToWindowsPath(iniPath);
    convertToWindowsPath(logPath);
    convertToWindowsPath(highScorePath);
    convertToWindowsPath(userHighScorePath);
    convertToWindowsPath(logoPath);
    convertToWindowsPath(fontPath);
    convertToWindowsPath(shipPath);
    convertToWindowsPath(shipBlinkPath);
    convertToWindowsPath(shipBlankPath);
    convertToWindowsPath(bulletPath);
    convertToWindowsPath(enemyBulletPath);
    convertToWindowsPath(ammoPath);
    convertToWindowsPath(collectiblePath);
    convertToWindowsPath(enemyPath);
    convertToWindowsPath(explosionPath);
    convertToWindowsPath(statIconPath);
    convertToWindowsPath(levelMusicPath);
    convertToWindowsPath(menuMusicPath);
    convertToWindowsPath(collectSoundPath);
    convertToWindowsPath(winSoundPath);
    convertToWindowsPath(shootSoundPath);
    convertToWindowsPath(enemyShootSoundPath);
    convertToWindowsPath(explodeSoundPath);
    convertToWindowsPath(playerDieSoundPath);
    convertToWindowsPath(screenshotPath);
}

#endif //_M_FILES_

