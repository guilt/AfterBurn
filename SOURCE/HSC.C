//High Scores Loop

#ifndef _M_HSC_
#define _M_HSC_

#include "decl.h"
#include "fhsc.c"

static int entryLen;
static char entryKeyChar, entryName[NAMELEN + 1];

void highScoresInput()
{
    if (gs.ticks == 0) {
        if (numHighScores <= 0) { setGameState(STATE_MAIN_MENU); return; }
        sortHighScores();
        clearKeys();
    }
    if (EDGE(KEY_ESC)) { setGameState(STATE_MAIN_MENU); return; }
}

void highScoresRender()
{
    int i;
    clearScreen();
    updateStarfield();
    drawStarfield();
    drawSprite(logo, getMidX() - (logo->w / 2), logo->h / 4);
    drawCenteredText(MENU_Y_OFFSET - (FONT_SIZE + VERTICAL_SPACING), activeColor, "%s", menu[HIGH_SCORES_MENU_ITEM]);
    drawText(HIGH_SCORES_NAME_X, MENU_Y_OFFSET, activeColor, "%s", hsNameStr);
    drawTextCenteredAt(HIGH_SCORES_LEVEL_X, MENU_Y_OFFSET, activeColor, "%s", hsLevelStr);
    drawText(HIGH_SCORES_SCORE_X, MENU_Y_OFFSET, activeColor, "%s", hsScoreStr);
    alfont_set_font_size(highScoresValuesFont, FONT_SIZE);
    int yStep = FONT_SIZE + 4;
    for (i = numHighScores - 1; i >= 0; i--) {
        alfont_textout_aa(drawingBuffer, highScoresValuesFont, Players[i].name, HIGH_SCORES_NAME_X, MENU_Y_OFFSET + ((i + 1) * yStep), passiveColor);
        sprintf(textBuffer, "%u", Players[i].level);
        alfont_textout_centre_aa(drawingBuffer, highScoresValuesFont, textBuffer, HIGH_SCORES_LEVEL_X, MENU_Y_OFFSET + ((i + 1) * yStep), passiveColor);
        sprintf(textBuffer, scoreFormat, Players[i].score);
        alfont_textout_aa(drawingBuffer, highScoresValuesFont, textBuffer, HIGH_SCORES_SCORE_X, MENU_Y_OFFSET + ((i + 1) * yStep), passiveColor);
    }
}

void highScoreEntryUpdate()
{
    if (gs.ticks == 1) {
        if (!isHighScore(gs.level, gs.lives, gs.score, playerHealth)) { setGameState(STATE_MAIN_MENU); return; }
        {
            const char* user = getUserName();
            int k; for (k = 0; k < NAMELEN && user[k]; k++) { entryName[k] = user[k]; }
            entryName[k] = 0; entryLen = k;
        }
        clearKeys();
    }
    gs.frameCount++;
}

void highScoreEntryInput()
{
    if (!isKeyPressed()) return;
    entryKeyChar = readKeyChar();
    if (entryKeyChar == NEWLINE_CHAR) {
        entryName[entryLen] = 0;
        if (!*entryName) { strcpy(entryName, anonymousStr); }
        addHighScore(entryName);
        setGameState(STATE_HIGH_SCORES);
        return;
    }
    if (entryKeyChar == BACKSPACE_CHAR) {
        if (entryLen > 0) { entryLen--; entryName[entryLen] = 0; }
    } else if (entryLen < NAMELEN) {
        entryName[entryLen++] = entryKeyChar;
        entryName[entryLen] = 0;
    }
    clearKeys();
}

void highScoreEntryRender()
{
    clearScreen();
    updateStarfield();
    drawStarfield();
    drawSprite(logo, getMidX() - (logo->w / 2), logo->h / 4);
    drawCenteredText(MENU_Y_OFFSET - (FONT_SIZE + VERTICAL_SPACING), activeColor, "%s", hsPromptStr);
    {
        int i, txtW;
        for (i = 0; i < entryLen; i++) { textBuffer[i] = entryName[i]; }
        textBuffer[i] = 0;
        alfont_set_font_size(highScoresValuesFont, FONT_SIZE);
        alfont_textout_centre_aa(drawingBuffer, highScoresValuesFont, textBuffer, getMidX(), MENU_Y_OFFSET, passiveColor);
        txtW = alfont_text_length(highScoresValuesFont, textBuffer);
        if ((gs.frameCount / 15) % 2) {
            alfont_textout_aa(drawingBuffer, highScoresValuesFont, "_", getMidX() + txtW / 2 + 1, MENU_Y_OFFSET, activeColor);
        }
    }
}

#endif //_M_HSC_
