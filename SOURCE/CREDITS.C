#ifndef _M_CRED_
#define _M_CRED_

#include "decl.h"

static int creditTick;

void creditsInput()
{
    if (gs.ticks == 0) {
        clearKeys();
        creditTick = 0;
    }
    if (EDGE(KEY_ESC)) { setGameState(STATE_MAIN_MENU); return; }
}

void creditsUpdate()
{
    creditTick++;
    if (creditTick > NUM_CREDIT_TICKS) creditTick = 0;
}

void creditsRender()
{
    int lineIndex, displayComplete;
    int calculatedX, calculatedY, usedX;
    int grayValue, textColor;
    clearScreen();
    updateStarfield();
    drawStarfield();
    drawSprite(logo, getMidX() - (logo->w / 2), logo->h / 4);
    drawCenteredTextNoAA(MENU_Y_OFFSET - (FONT_SIZE + VERTICAL_SPACING), activeColor, "%s", menu[CREDITS_MENU_ITEM]);
    displayComplete = 0;
    usedX = sinTable[creditTick % SIN_TABLE_SIZE] * CREDIT_X_MULTIPLIER / SIN_SCALE;
    for (lineIndex = 0; lineIndex < NUM_CREDIT_LINES && !displayComplete; lineIndex++) {
        grayValue = (rand() % ((MAX_COLOR_VALUE / 2) + 1)) + (MAX_COLOR_VALUE / 2);
        textColor = makecol(grayValue, grayValue, grayValue);
        calculatedY = getMidY() - (creditTick * CREDIT_SPEED) + (lineIndex * (FONT_SIZE + CREDIT_LINE_SPACING));
        calculatedX = usedX;
        if (calculatedY >= getMinY() && calculatedY <= getMaxY()) {
            alfont_textout_centre_aa(drawingBuffer, creditsFont, (char*)names[lineIndex], getMidX() - calculatedX, MENU_Y_OFFSET - VERTICAL_SPACING + calculatedY, textColor);
            usedX += sinTable[(lineIndex + creditTick) % SIN_TABLE_SIZE] * CREDIT_X_MULTIPLIER / SIN_SCALE;
        } else if (calculatedY > getMaxY()) {
            displayComplete = 1;
        }
    }
}

#endif //_M_CRED_


