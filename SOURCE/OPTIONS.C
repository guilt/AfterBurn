#ifndef _M_OPT_
#define _M_OPT_

#include "decl.h"
#include "stat.c"
#include "sounds.c"

void drawOptionsMenu()
{
    clearScreen();
    updateStarfield();
    drawStarfield();
    drawSprite(logo, getMidX() - (logo->w / 2), logo->h / 4);
    if (optionMenuSelection == 2) {
        drawCenteredText(MENU_Y_OFFSET + (2 * (FONT_SIZE + VERTICAL_SPACING)), activeColor, optionsMenu[2], optionStates[showStats]);
    } else {
        drawCenteredText(MENU_Y_OFFSET + (2 * (FONT_SIZE + VERTICAL_SPACING)), passiveColor, optionsMenu[2], optionStates[showStats]);
    }
    if (!soundError) {
        if (optionMenuSelection == 0) {
            drawCenteredText(MENU_Y_OFFSET, activeColor, optionsMenu[0], optionStates[isSoundEnabled]);
        } else {
            drawCenteredText(MENU_Y_OFFSET, passiveColor, optionsMenu[0], optionStates[isSoundEnabled]);
        }
        if (optionMenuSelection == 1) {
            drawCenteredText(MENU_Y_OFFSET + FONT_SIZE + VERTICAL_SPACING, activeColor, optionsMenu[1], optionStates[isMusicEnabled]);
        } else {
            drawCenteredText(MENU_Y_OFFSET + FONT_SIZE + VERTICAL_SPACING, passiveColor, optionsMenu[1], optionStates[isMusicEnabled]);
        }
        if (optionMenuSelection == 3) {
            drawCenteredText(MENU_Y_OFFSET + (3 * (FONT_SIZE + VERTICAL_SPACING)), activeColor, optionsMenu[3], DVOL);
        } else {
            drawCenteredText(MENU_Y_OFFSET + (3 * (FONT_SIZE + VERTICAL_SPACING)), passiveColor, optionsMenu[3], DVOL);
        }
    }
}

void optionsInput()
{
    if (EDGE(KEY_ESC)) { setGameState(gs.prevState); return; }
    if (EDGE(KEY_UP)) { optionMenuSelection += NUM_OPTIONS_MENU_ITEMS - 1; optionMenuSelection %= NUM_OPTIONS_MENU_ITEMS; }
    if (EDGE(KEY_DOWN)) { optionMenuSelection++; optionMenuSelection %= NUM_OPTIONS_MENU_ITEMS; }
    if (EDGE(KEY_ENTER)) {
        switch (optionMenuSelection) {
        case 0: toggleSound(); break;
        case 1: toggleMusic(); break;
        case 2: toggleStats(); break;
        }
    }
    if (key[KEY_RIGHT]) {
        switch (optionMenuSelection) {
        case 3:
            if (DVOL < MAX_VOLUME) DVOL++;
            DVOL %= (MAX_VOLUME + 1);
            applyVolume();
            break;
        }
    }
    if (key[KEY_LEFT]) {
        switch (optionMenuSelection) {
        case 3:
            if (DVOL > 0) DVOL--;
            DVOL = (DVOL + MAX_VOLUME + 1) % (MAX_VOLUME + 1);
            applyVolume();
            break;
        }
    }
}

void optionsRender()
{
    drawOptionsMenu();
}

#endif //_M_OPT_

