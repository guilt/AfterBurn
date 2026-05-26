extern volatile long speedCounter;
extern void incrementSpeedCounter();

#include "files.h"
#include "time.h"
#include "lunload.c"
#include "flog.c"
#include "menu.c"

volatile long speedCounter = 0;
void incrementSpeedCounter() { speedCounter++; }
END_OF_FUNCTION(incrementSpeedCounter);

static void shutdownAndExit(int code, const char* reason)
{
    if (reason) { logWrite("%s", reason); }
    writePreferences();
    logClose();
    alfont_exit();
    musicExit();
    allegro_exit();
    exit(code);
}

int main(int argc, char* argv[])
{
    if (getcwd(dataDir, STRING_BUFFER_SIZE - 1) != NULL) {
        int len = strlen(dataDir);
        if (len > 0 && dataDir[len - 1] != DIR_DELIMITER && len < STRING_BUFFER_SIZE - 2) {
            dataDir[len] = DIR_DELIMITER;
            dataDir[len + 1] = '\0';
        }
        strToUpper(dataDir);
        /* prefer executable directory over CWD */
        {
            char* sep = strrchr(argv[0], DIR_DELIMITER);
            if (!sep) sep = strrchr(argv[0], ALT_DIR_DELIMITER);
            if (sep) {
                int dlen = (int)(sep - argv[0] + 1);
                if (dlen < STRING_BUFFER_SIZE) {
                    memcpy(dataDir, argv[0], dlen);
                    dataDir[dlen] = '\0';
                    strToUpper(dataDir);
                }
            }
        }
    }
    if (argc > 1) {
        if (argc > 2) {
            logWrite("Argument error: format is %s <file>!", argv[0]);
            logClose();
            return RESULT_ERROR;
        }
        strcpy(iniPath, argv[1]);
    }
    normalizePaths();
    logInit();
    beginLog();

    iniOpenFile();
    allegro_init();
    register_trace_handler(errorHandler);
    register_assert_handler(errorHandler);
    doTitle();
    install_timer();
    LOCK_VARIABLE(speedCounter);
    LOCK_FUNCTION(incrementSpeedCounter);
    install_int_ex(incrementSpeedCounter, BPS_TO_TIMER(LPS));
    install_keyboard();
    install_mouse();
    scare_mouse();

    if (alfont_init() != ALFONT_OK) {
        shutdownAndExit(RESULT_ERROR, "gameFont error!");
    }
    preSoundInit();
    if (install_sound(DIGITAL_MODE, MIDI_MODE, configPath)) { soundError = 1; }
    postSoundInit();
    if (soundError) { logWrite("Sound initialisation error!"); }
    if (musicError) { logWrite("Music initialisation error!"); }

    set_color_depth(BITS_PER_PIXEL);

    // Cascade: try saved resolution+mode first, then 720p, then 640 — save working res
    int gfxOk = 0, tryW = 640, tryH = 480, gfxMode = GFX_AUTODETECT;
    if (savedResW > 0 && savedResH > 0) {
        gfxMode = savedGfxMode; tryW = savedResW; tryH = savedResH;
        gfxOk = (set_gfx_mode(gfxMode, tryW, tryH, 0, 0) == 0);
    }
    if (!gfxOk) { gfxMode = GFX_AUTODETECT; tryW = 1280; tryH = 720; gfxOk = (set_gfx_mode(gfxMode, tryW, tryH, 0, 0) == 0); }
    if (!gfxOk) { gfxMode = GFX_AUTODETECT; tryW = 640;  tryH = 480; gfxOk = (set_gfx_mode(gfxMode, tryW, tryH, 0, 0) == 0); }
    if (!gfxOk) shutdownAndExit(RESULT_ERROR, "Graphics initialisation error!");
    savedGfxMode = gfxMode;
    screenWidth = SCREEN_W;
    screenHeight = SCREEN_H;

    // Calculate letterboxed destination rect (integer math, 4:3 aspect)
    if (screenWidth * 480 <= screenHeight * 640) {
        scaleDestW = screenWidth;
        scaleDestH = screenWidth * 480 / 640;
    } else {
        scaleDestW = screenHeight * 640 / 480;
        scaleDestH = screenHeight;
    }
    scaleDestX = (screenWidth - scaleDestW) / 2;
    scaleDestY = (screenHeight - scaleDestH) / 2;

    set_display_switch_callback(SWITCH_OUT, onSwitchOut);
    set_display_switch_callback(SWITCH_IN, onSwitchIn);

    loadAssets();
    mainMenu();
    unloadAssets();
    shutdownAndExit(RESULT_OK, "Quitting gracefully.");
    return RESULT_OK;
}

END_OF_MAIN();
