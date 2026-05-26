#include "loop.c"

void initFont()
{
    alfont_text_mode(-1);
    alfont_set_font_size(gameFont, FONT_SIZE);
}

void doTitle()
{
    set_window_title(windowTitle);
}

void beginLog()
{
    sprintf(windowTitle, "%s - v%s", VProductName, VProductVersion);
    logWrite("%s Log", windowTitle);
}

void drawMenu()
{
    int menuIndex;
    clearScreen();
    updateStarfield();
    drawStarfield();
    drawSprite(logo, getMidX() - (logo->w / 2), logo->h / 4);
    for (menuIndex = 0; menuIndex < NUM_MENU_ITEMS; menuIndex++) {
        drawCenteredText(MENU_Y_OFFSET + (menuIndex * (FONT_SIZE + VERTICAL_SPACING)), passiveColor, "%s", menu[menuIndex]);
    }
    drawCenteredText(MENU_Y_OFFSET + (menuSelection * (FONT_SIZE + VERTICAL_SPACING)), activeColor, "%s", menu[menuSelection]);
}

void menuInput()
{
    if (EDGE(KEY_ESC)) { setGameState(STATE_EXIT); return; }
    if (EDGE(KEY_UP)) { menuSelection += NUM_MENU_ITEMS - 1; menuSelection %= NUM_MENU_ITEMS; }
    if (EDGE(KEY_DOWN)) { menuSelection++; menuSelection %= NUM_MENU_ITEMS; }
    if (EDGE(KEY_ENTER)) {
        switch (menuSelection) {
        case 0: oneTimeGameInit(); initGame(); setGameState(STATE_PLAYING); break;
        case 1: gs.prevState = STATE_MAIN_MENU; setGameState(STATE_OPTIONS); break;
        case 2: setGameState(STATE_HIGH_SCORES); break;
        case 3: setGameState(STATE_CREDITS); break;
        case 4: setGameState(STATE_EXIT); return;
        }
    }
}

void menuRender()
{
    drawMenu();
}

void showIntroVideo()
{
    playCinema(allegroLogoIndex);
}

void nullInput() {}
void nullRender() {}
void unimplementedUpdate() {}
void menuUpdate() {}
void gameOverUpdate() { gs.frameCount++; }
void pauseUpdate() {}
void restartUpdate() { gs.frameCount++; }
void levelClearUpdate() {}
void optionsUpdate() {}
void highScoresUpdate() {}

void initStateMachine()
{
    int i;
    for (i = 0; i < NUM_STATES; i++) {
        stateInput[i] = nullInput;
        stateUpdate[i] = nullInput;
        stateRender[i] = nullRender;
    }
    stateInput[STATE_MAIN_MENU] = menuInput;
    stateUpdate[STATE_MAIN_MENU] = menuUpdate;
    stateRender[STATE_MAIN_MENU] = menuRender;
    stateInput[STATE_PLAYING] = playingInput;
    stateUpdate[STATE_PLAYING] = playingUpdate;
    stateRender[STATE_PLAYING] = drawGame;
    stateInput[STATE_GAME_OVER] = gameOverInput;
    stateUpdate[STATE_GAME_OVER] = gameOverUpdate;
    stateRender[STATE_GAME_OVER] = gameOverRender;
    stateInput[STATE_PAUSED] = pauseInput;
    stateUpdate[STATE_PAUSED] = pauseUpdate;
    stateRender[STATE_PAUSED] = pauseRender;
    stateInput[STATE_QUIT_DIALOG] = pauseInput;
    stateUpdate[STATE_QUIT_DIALOG] = pauseUpdate;
    stateRender[STATE_QUIT_DIALOG] = pauseRender;
    stateInput[STATE_RESTART] = restartInput;
    stateUpdate[STATE_RESTART] = restartUpdate;
    stateRender[STATE_RESTART] = restartRender;
    stateInput[STATE_LEVEL_CLEAR] = levelClearInput;
    stateUpdate[STATE_LEVEL_CLEAR] = levelClearUpdate;
    stateRender[STATE_LEVEL_CLEAR] = levelClearRender;
    stateInput[STATE_OPTIONS] = optionsInput;
    stateUpdate[STATE_OPTIONS] = optionsUpdate;
    stateRender[STATE_OPTIONS] = optionsRender;
    stateInput[STATE_HIGH_SCORES] = highScoresInput;
    stateUpdate[STATE_HIGH_SCORES] = highScoresUpdate;
    stateRender[STATE_HIGH_SCORES] = highScoresRender;
    stateInput[STATE_HIGH_SCORE_ENTRY] = highScoreEntryInput;
    stateUpdate[STATE_HIGH_SCORE_ENTRY] = highScoreEntryUpdate;
    stateRender[STATE_HIGH_SCORE_ENTRY] = highScoreEntryRender;
    stateInput[STATE_CREDITS] = creditsInput;
    stateUpdate[STATE_CREDITS] = creditsUpdate;
    stateRender[STATE_CREDITS] = creditsRender;
    stateInput[STATE_EXIT] = nullInput;
    stateRender[STATE_EXIT] = nullRender;
}

void globalInput()
{
    if (EDGE(KEY_M)) { toggleMusic(); }
    if (EDGE(KEY_S)) { toggleSound(); }
    if (EDGE(KEY_T)) { toggleStats(); }
}

void mainMenu()
{
    showIntroVideo();
    logWrite("-- Menu initialising --");
    initEnemyTable();
    initFont();
    initGenColors();
    initStarfield();
    initStarColors();
    setPalette();
    clearScreen();
    clearKeys();
    playMusic();
    initStateMachine();
    setGameState(STATE_MAIN_MENU);
    logWrite("-- Menu loop --");
    while (gs.state != STATE_EXIT) {
        poll_keyboard();
        globalInput();
        stateInput[gs.state]();
        memcpy((void*)lastKeys, (void*)key, sizeof(lastKeys));
        while (speedCounter <= 0) { rest(1); }
        do {
            gs.ticks++;
            stateUpdate[gs.state]();
            poll_keyboard();
            stateInput[gs.state]();
            memcpy((void*)lastKeys, (void*)key, sizeof(lastKeys));
            speedCounter--;
        } while (speedCounter > 0);
        stateRender[gs.state]();
        render();
        rest(0);
    }
    stopMusic();
    logWrite("-- Menu loop done --");
}

