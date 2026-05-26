# AfterBurn — Developer Guide

## Build System

- **`EXTDEV`** must be set in your environment — points to your SDK root (e.g. `C:\Users\Karthik\EXTDEV`). `BUILD.BAT` exits with an error if it's undefined.
- **`MINGDIR`** is derived as `%EXTDEV%\MinGW32` (the 32-bit MinGW compiler directory).
- **Compiler:** `%MINGDIR%\bin\gcc` (invoked via `%GCC%` variable — no `PATH` modification needed)
- **Script:** `BUILD.BAT` (run from repo root; all paths use `%~dp0` so it works from any working directory)
- **Output:** `AB.EXE` (statically linked, UPX-compressed by default)
- **Defines (required):** `-DDEL -DADEL -DDUMB`
- **Optimisation:** `-O1 -Wall -Wextra` (release); `-O0 -g -DDEBUG=1` (debug, set via `DEBUG=1` env var)
- **Debugging:** `DEBUG.BAT` builds with debug symbols and launches GDB with auto-run (`-ex "run"`). Accepts theme name as shorthand: `DEBUG` = GREEN, `DEBUG TAMIL` = TAMIL.

```
BUILD.BAT      → AB.EXE + alleg42.dll for runtime
GREEN.BAT      → runs AB.EXE with THEME/GREEN/GREEN.INI (Green Theme)
TAMIL.BAT      → runs AB.EXE with the THEME/TAMIL/TAMIL.INI (Tamil-language)
```

### External Libraries

| Library     | Version        | Headers                     | Key API                     |
|-------------|----------------|-----------------------------|-----------------------------|
| **Allegro** | 4.x (WIP)      | `<allegro.h>`               | `key[]`, `BITMAP`, timer, sound, file I/O |
| **DUMB**    | 0.9.3+         | `<aldumb.h>`                | Module loading & playback   |
| **AllegroFont** | 2.x        | `<alfont.h>`                | TrueType font rendering     |

DUMB is always used for music playback (JGMOD removed).

### Library Layout (under `%EXTDEV%`)

```
%EXTDEV%\MinGW32\                         → Compiler (bin\gcc, bin\windres)
%EXTDEV%\Allegro-MSVC8-MinGW-BCC\         → Allegro 4 (Include\, Lib\MinGW\x86\)
%EXTDEV%\AlFont-MSVC-MinGW\              → AllegroFont (Include\, Lib\MinGW\x86\)
%EXTDEV%\Dumb-MSVC9-MinGW\               → DUMB (Include\, Lib\MinGW\x86\)
```

## Code Conventions

- **Language:** C89 with GNU extensions (declaration-after-statement, `//` comments allowed)
- **Brace style:** K&R — opening brace on same line as control flow
- **Indentation:** 4 spaces (no tabs)
- **Naming:** camelCase for variables and functions
- **Comments:** avoid unless explaining a non-obvious design decision
- **No `while(1)` blocking loops** — every function returns each frame (state machine pattern)

## Architecture — State Machine

The game uses a function-pointer-dispatched state machine instead of switch/case:

```c
void (*stateInput[NUM_STATES])();   // per-state input
void (*stateUpdate[NUM_STATES])();  // per-state logic
void (*stateRender[NUM_STATES])();  // per-state rendering
```

Initialised by `initStateMachine()` in `SOURCE/MENU.C`.  
States defined in `enum GameState` in `SOURCE/DECL.H`.

### Game State Struct

Core game state is grouped into `struct GS` in `SOURCE/DECL.H`:

```c
struct GS {
    int state;              // current game state
    int ticks;              // frames since state entered
    int prevState;          // previous state (for return)
    int isDone;             // game-over flag
    unsigned int frameCount; // frame counter (for animation, blinking cursor)
    int score, lives, level, win, isInGame;
    unsigned int enemiesKilled;
} gs;  // single global instance
```

This replaces 11 separate globals, making initialization explicit and preventing
cross-file linkage-order issues.

### Master Loop (in `mainMenu()`)

Every state has its own `input`, `update`, and `render` function wired up in
`initStateMachine()`. States that don't need per-tick logic have explicit
empty updates (not a shared stub).

```
while (gameState != STATE_EXIT):
    poll_keyboard()
    globalInput()            // M/S/T toggles from any state
    stateInput[gameState]()
    while (speedCounter <= 0):
        rest(1)              // wait for timer
    do:
        gameStateTicks++
        stateUpdate[gameState]()
        poll_keyboard()
        stateInput[gameState]()
        speedCounter--
    while (speedCounter > 0)
    stateRender[gameState]()
    render()                 // triple-buffer flip
    lastKeys = key[]         // update edge-detection state
    rest(0)
```

### Adding a New State

1. Add entry to `enum GameState` in `DECL.H` (before `STATE_EXIT`).
2. Create `myStateInput()`, `myStateUpdate()`, `myStateRender()` functions.
3. Register them in `initStateMachine()` in `MENU.C`.
4. Add function declarations in `DECL.H`.

## Display Pipeline

The game renders everything to a **virtual canvas** (`virtualBuffer`) at a fixed **640×480** resolution, then stretches it to the physical display with aspect‑correct letterboxing. This makes all game logic resolution‑independent.

```c
BITMAP *virtualBuffer = NULL;              // 640×480 memory bitmap (drawing target)
BITMAP *backBuffer = NULL;                 // screen‑sized memory bitmap (page flip)
int scaleDestX, scaleDestY, scaleDestW, scaleDestH; // letterbox rect

#define drawingBuffer virtualBuffer

#define render() { \
    clear_bitmap(backBuffer); \
    stretch_blit(drawingBuffer, backBuffer, 0,0, 640,480, scaleDestX,scaleDestY,scaleDestW,scaleDestH); \
    blit(backBuffer, screen, 0,0, 0,0, screenWidth, screenHeight); \
    vsync(); \
}
```

**Resolution cascade** (in `main()`):
1. Try saved resolution/mode from PRF (skips on first run)
2. `GFX_AUTODETECT` at 1280×720 → 640×480 (tries fullscreen first, windowed fallback per res)
3. If all fail, exit with error
4. Working resolution saved to PRF on shutdown for fast startup next time

The letterbox rect is computed once at startup with pure integer math
(`scaleDestW/scaleDestH` derived from `screenWidth × 480 ⇔ screenHeight × 640`).
FLI video playback uses the same stretch‑blit pipeline for consistent appearance.

## Logic Timer

A 60 Hz timer drives the logic loop:

```c
volatile long speedCounter = 0;
void incrementSpeedCounter() { speedCounter++; }
END_OF_FUNCTION(incrementSpeedCounter);

install_int_ex(incrementSpeedCounter, BPS_TO_TIMER(60));  // in main()
```

`LPS = 60` in `DECL.H`. The do-while loop drains `speedCounter` so the game
runs catch-up logic ticks if it falls behind, then renders once.

## Input Handling

**No `keypressed()` / `readkey()` during gameplay.** All input uses direct
`key[]` array polling:

```c
int lastKeys[KEY_MAX];
#define EDGE(k) (key[k] && !lastKeys[k])
```

- **Toggle/one-shot keys:** use `EDGE(k)` (e.g. `EDGE(KEY_ESC)`)
- **Continuous keys:** use `key[k]` directly (e.g. `key[KEY_SPACE]`)
- `lastKeys` is updated at the end of the master loop: `lastKeys[i] = key[i]`
- `poll_keyboard()` is called before every input dispatch (required on Allegro
  Windows where `key[]` is updated by the message pump, not a hardware interrupt)
- `clear_keybuf()` is called in each state's input function on first entry
  (`gameStateTicks == 0`) to drain the buffer for `keypressed()`-based states
  (game over, restart, level clear, high score entry)

## File I/O & Path Handling

### Path Resolution

All paths are resolved at startup in `normalizePaths()` (`FILES.H`). The game uses three directory globals:

- **`dataDir`**: Set from `getcwd()` at startup. The game root — all data assets are resolved relative to this path (e.g. `dataDir\DATA\HERO\SHIP01.BMP`).
- **`themeDataDir`**: Extracted from the `.INI` path after `dataDir` is prefixed. For `dataDir\THEME\GREEN\GREEN.INI`, this becomes `dataDir\THEME\GREEN\`.
- **`userDataDir`**: Set by `initUserDataDir()` from environment variables (`XDG_DATA_HOME` → `APPDATA` → `HOME` → `USERPROFILE`). Falls back to empty (CWD) on DOS/9x.

Paths that reference game assets (`DATA\*`, theme files) are prefixed with `dataDir`.  
Paths that are user-specific (high scores, preferences, game log) are prefixed with `userDataDir`.  
The `prependDir()` helper (`FILES.H`) handles idempotent prefixing — it checks `strncmp` to avoid double-prefixing across repeated `normalizePaths()` calls.

### Case Normalization

All file paths are **auto-uppercased** via `strToUpper()` in `FILES.H`:

```c
static BITMAP* loadBitmap(const char* path, RGB* pal) {
    char buf[STRING_BUFFER_SIZE];
    strcpy(buf, path); strToUpper(buf);
    return load_bitmap(buf, pal);
}
```

Same pattern for `loadSample()`, `loadFont()`, `loadDatafile()`, `dumbLoad*()`.
This ensures case-insensitive filesystem compatibility on Windows.

## INI Format

A `.INI` file controls all game data paths, enemy/bullet parameters, UI strings,
and color settings. The default is `THEME/GREEN/GREEN.INI`.  
INI parsing is in `SOURCE/FINI.C`; user preferences are written via `writePreferences()` to `userDataDir/<theme>.PRF`.

## Debugging

- **Game log:** written to `GAMELOG.TXT` at the game root
- **Assert/trace handler:** `errorHandler()` in `SOURCE/LOOP.C` captures Allegro
  assertions and traces, logs them, then exits cleanly
- **Screenshots:** F12 writes PCX files to `SCR*.PCX`

## Compatibility Notes

- Works on **Windows** (MinGW) and **GNU/Linux**
- `DIR_DELIMITER` / `ALT_DIR_DELIMITER` macros in `SYSDEFS.H` abstract path separators
- `convertToLocalPath()` / `convertToWindowsPath()` in `FILES.H` for cross-platform paths
- Video playback uses `play_memory_fli()` (Allegro FLI/FLC player)
- Font rendering via `alfont_textout_*` (anti-aliased) and `alfont_textout_centre_*`
