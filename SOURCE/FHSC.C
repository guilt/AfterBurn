#ifndef _M_FHSC_
#define _M_FHSC_

#include <stdarg.h>
#include "decl.h"
#include "pal.c"
#include "files.h"

void hscWrite(const char* format, ...)
{
    if (!hscFile) { return; }
    va_list args;
    va_start(args, format);
    vfprintf(hscFile, format, args);
    va_end(args);
    fputc('\n', hscFile);
}

void initRandomHighScores()
{
    if (numHighScores <= 0) { return; }
    int i;
    seedRandom();
    for (i = 0; i < numHighScores; i++) {
        Players[i].win = (rand() % 2);
        Players[i].level = (numLevels - 1);
        if (!Players[i].win) {
            Players[i].level = (rand() % numLevels);
        }
        Players[i].lives = (rand() % maxPossibleLives);
        Players[i].score = (rand() % LIFE_GIVING_SCORE);
        Players[i].score /= SCORE_MULTIPLIER;
        Players[i].score *= SCORE_MULTIPLIER;
        Players[i].life = ((rand() % maxHealth) + 1);
    }
}

// Returns negative, 0, positive based on (win, level, lives, score, life, name) tuple ordering.
// Higher rank = better. So a > b means a should sort first.
static int comparePlayerRank(const struct Player* playerA, const struct Player* playerB)
{
    if (playerA->win   != playerB->win)   { return (playerA->win   > playerB->win)   ? 1 : -1; }
    if (playerA->level != playerB->level) { return (playerA->level > playerB->level) ? 1 : -1; }
    if (playerA->lives != playerB->lives) { return (playerA->lives > playerB->lives) ? 1 : -1; }
    if (playerA->score != playerB->score) { return (playerA->score > playerB->score) ? 1 : -1; }
    if (playerA->life  != playerB->life)  { return (playerA->life  > playerB->life)  ? 1 : -1; }
    if (!playerA->name[0]) return -1;
    if (!playerB->name[0]) return 1;
    return -strcmp(playerA->name, playerB->name);
}

void sortHighScores()
{
    int i, j;
    if (numHighScores <= 0) { return; }
    for (i = 0; i < numHighScores - 1; i++) {
        for (j = i + 1; j < numHighScores; j++) {
            if (comparePlayerRank(&Players[i], &Players[j]) < 0) {
                tempPlayer = Players[i];
                Players[i] = Players[j];
                Players[j] = tempPlayer;
            }
        }
    }
}

void readHighScores()
{
    int i;
    hscFile = fileOpen(getUserDataPath("DEFAULT.HSC"), "r");
    if (!hscFile) {
        hscFile = fileOpen(getAppDataPath("DATA\\GLOBAL\\HSCORES.TXT"), "r");
        if (!hscFile) {
            logWrite("Couldn't read scores file: %s", getAppDataPath("DATA\\GLOBAL\\HSCORES.TXT"));
            hscWriteNeeded = 1;
            return;
        }
        logWrite("Reading theme scores file: %s", getAppDataPath("DATA\\GLOBAL\\HSCORES.TXT"));
    } else {
        logWrite("Reading user scores file: %s", getUserDataPath("DEFAULT.HSC"));
    }
    fscanf(hscFile, " %d", &numHighScores);
    if (numHighScores < 0) { numHighScores = 0; }
    if (numHighScores > MAX_HIGH_SCORES) { numHighScores = MAX_HIGH_SCORES; }
    for (i = 0; i < numHighScores; i++) {
        fscanf(hscFile, "%s", Players[i].name);
        replaceChar(Players[i].name, ARROW_SEPARATOR, SPACE_CHAR);
        fscanf(hscFile, " %d %d %d %d %d",
               &Players[i].win, &Players[i].level, &Players[i].lives,
               &Players[i].score, &Players[i].life);
        Players[i].name[NAMELEN] = 0;
    }
    fclose(hscFile);
    logWrite("Read scores file done");
    sortHighScores();
}

int isHighScore(unsigned int level, unsigned int lives, unsigned int score, unsigned int life)
{
    int i;
    if (!hasScored) { return 0; }
    if (numHighScores < 0) { numHighScores = 0; }
    struct Player candidate;
    candidate.win = gs.win;
    candidate.level = gs.level;
    candidate.lives = gs.lives;
    candidate.score = gs.score;
    candidate.life = life;
    candidate.name[0] = 0;
    for (i = 0; i < numHighScores; i++) {
        if (comparePlayerRank(&candidate, &Players[i]) > 0) { return 1; }
    }
    return numHighScores < MAX_HIGH_SCORES;
}

void addHighScore(char name[])
{
    if (numHighScores < 0) { return; }
    if (numHighScores <= MAX_HIGH_SCORES) {
        name[NAMELEN] = 0;
        strcpy(Players[numHighScores].name, name);
        Players[numHighScores].win = gs.win;
        Players[numHighScores].level = gs.level;
        Players[numHighScores].lives = gs.lives;
        Players[numHighScores].score = gs.score;
        Players[numHighScores].life = (playerHealth > 0) ? playerHealth : 0;
        numHighScores++;
        hscWriteNeeded = 1;
    }
    sortHighScores();
    if (numHighScores > MAX_HIGH_SCORES) { numHighScores = MAX_HIGH_SCORES; }
}

void writeHighScores()
{
    int i;
    if (!hscWriteNeeded || !numHighScores) {
        logWrite("Needn't write scores file: %s", getUserDataPath("DEFAULT.HSC"));
        return;
    }
    hscFile = fileOpen(getUserDataPath("DEFAULT.HSC"), "w");
    if (!hscFile) {
        logWrite("Couldn't write scores file: %s", getUserDataPath("DEFAULT.HSC"));
        return;
    }
    logWrite("Writing scores file: %s", getUserDataPath("DEFAULT.HSC"));
    hscWrite("%d", numHighScores);
    for (i = 0; i < numHighScores; i++) {
        Players[i].name[NAMELEN] = 0;
        replaceChar(Players[i].name, SPACE_CHAR, ARROW_SEPARATOR);
        hscWrite("%s %d %d %d %d %d",
                 Players[i].name, Players[i].win, Players[i].level,
                 Players[i].lives, Players[i].score, Players[i].life);
    }
    logWrite("Wrote scores file: %s", getUserDataPath("DEFAULT.HSC"));
    fclose(hscFile);
}

#endif //_M_FHSC_
