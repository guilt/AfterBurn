# Changelog

## 2026-05-26

- Build system: full‑path tool vars (`%GCC%`, `%GDB%`, `%UPXE%`), `-O1`/`-Wall -Wextra`, all `.BAT` use `%~dp0`
- Refactored 11 game‑state globals into `struct GS`; removed dead globals `i`, `j`
- Simplified `BUILD.BAT`: uses `EXTDEV` env var, errors if unset
- Simplified launch scripts: `DEBUG.BAT` auto‑runs GDB with theme shorthand
- Each state machine state has its own input/update/render triplet; removed shared `nullUpdate`
- High score entry: blinking cursor, backspace, `getUserName()` pre‑fill, ENTER submits, only qualifies when beating real entry
- Removed all `float`/`double` from codebase (pure integer math throughout)
- Converted blocking `keypressed()` loops to non-blocking state machine (function pointer dispatch tables) — game over, pause, restart, level clear, options, high scores, credits
- Added `volatile long speedCounter` timer at 60 Hz for fixed-rate logic loop; decoupled input from frame rate
- Added aspect-ratio `stretch_blit` to video playback
- Added variadic drawing helpers (`drawText`, `drawCenteredText`, etc.) and file I/O helpers with auto-uppercase paths
- Added `pauseMusic()`/`resumeMusic()` with `set_display_switch_callback` to mute on alt-tab
- Fixed `lastKeys` type mismatch (`int → volatile char`); `memcpy` copy after every `stateInput()` call
- Added progress bar inline with level indicator (10% width, color-coded by percentage)

---

## 2026-05-25

- Code cleanup and modernization pass
- Renamed ~35 cryptic variables across 12 source files
- Converted all global loop variables (`i`, `j`) to local scope
- Normalized all source files to K&R brace style with 4-space indentation
- Replaced `sprintf(textBuffer) + alfont_textout` anti-pattern with variadic drawing helpers (`drawText`, `drawCenteredText`, `drawCenteredTextNoAA`, `drawTextCenteredAt`)
- Added file I/O helpers (`fileOpen`, `loadBitmap`, `saveBitmap`, `loadSample`, `loadFont`, `loadDatafile`, `dumbLoadS3m/It/Xm/Mod/Ogg`) that auto-uppercase file paths for case-insensitive filesystem compatibility
- Removed all explicit `strToUpper()` calls from LUNLOAD.C, SHOTS.C, VID.C
- Fixed theme credits rendering — always uses original font
- Removed `.git` from source control tracking

---

## 2004-03-13

- Added crude timer for `render()` which makes things run pretty uniformly

---

## 2004-03-10

- Things work just fine. Thanks to Ben Davis.

---

## 2004-03-09

- Converted documentation into HTML

---

## 2004-03-08

- Completed the Credits. What to do for languages?
- Made DUMBOGG optional

---

## 2003-11-28

- Removed most of the BATs.
- Added a `make.sh` for GNU/Linux.
- Revamped Code to adjust Delimiters for GNU/Linux.
- Fixed Bug in Video code to suit platform stuff.

---

## 2003-11-17

- Revamped the code, added support for Ogg Vorbis (only for DUMB).
- The Old Cluttered Batch Files have been moved to a BATS directory.
- Discovered Need to Patch `ogg/os_types.h` for new GCC thingy.

---

## 2003-06-21

- Checked out the non-compatibility of ModPlug with DUMB.
- Fixed the DUMB MOD Load functions, with thanks to Ben Davis.
- Checked for BUGS in the High Scores thingy.

---

## 2003-06-07

- Got to play with DUMB. With Thanks to Ben Davis. Yay!
- Tweaked the code to make it uniform with JGMOD.
- Added more Batch Files.
- Unified DUMB and JGMOD Interface.
- Fixed some stuff up in DUMB and JGMOD.
- Can't understand it ... Why is there some sorta' Buzzing sound in between Looping in DUMB?
- Fixed the Menu Thing with DUMB.

---

## 2003-05-29

- Accelerated the Video Player Code.
- Tweaked the Video Player UI again.

---

## 2003-05-28

- Remodelled the Video Player with Optimized Double Buffering.
- Fixed the Centering Problem.
- Added an Allegro Intro as well. Effect Works well :)
- Patched up the Video Player UI.

---

## 2003-05-27

- Added Non-Double Buffered Video Support.
- Renamed Batch Files according to INI name.
- Removed TEMPL out of distro. for small file size.

---

## 2003-05-26

- Tried out another language TTF support: TAMIL.INI.

---

## 2003-05-25

- Added a Win and updated the Scoring.
- Fixed another Scoring Bug.
- Added and Fixed Random Score Generation.

---

## 2003-05-24

- Added Batch Files for Debug compilation as well.
- Made the INI writer Neat.
- Fixed the Scores UI Bug.
- Checked out the "Funky Tracking" thingy!

---

## 2003-04-04

- Added Gen. level support and fixed some of it.
- Fixed INI support.

---

## 2003-04-01

- Fixed another Score bug.
- Added some Options support.
- Fixed some miscellaneous bugs.
- Removed display of MIDI Volume, though it is internally available to the system.
- Fixed a miscellaneous High Scores bug.
- Added Options in midst of Game too.

---

## 2003-03-30

- Added complete High Score support. All credits to Nikhil.
- Added a dummy loop (Yay!)
- Fixed a Menu Bug.
- Fixed a miscellaneous Score bug.

---

## 2003-03-29

- Changed appearance of lives.
- Checked compatibility of DUMB, for IT files. Has great effects and features, except for one thing: doesn't play music in the background. Requires manual polling, just like almp3 :( Hence DUMB is dropped in this game. Could have added Automatic poll support for non-DOS like users in the lib itself ....
- Added limited High Score display support. A little more needs to be done.
- Fixed the pause bug.

---

## 2003-03-28

- Improvised color support.
- Improved Weaponry Support, hero, ammo, collectibles, stars, explosions, enemy bullets and enemies support.
- Added mINI support.
- Fixed INI support.
- Made support for loading multiple INIs.
- Checked out a new GREEN INI and extended the code.
- Started work on a BITS INI :)

---

## 2003-03-21

- Thrashed the Starfield about ... FX.
- Randomized the Starfield color.
- Gave some time for "Press ...." .
- Added some debugging support for debug versions.

---

## 2003-02-21

- Renamed Music files to MUS to have non-conversion stuff.
- Made Music Code more Robust.

---

## 2003-01-13

- Added MOD support thru' JGMOD.
- Temporarily changed PCX to BMP shots.

---

## 2003-01-01

- Made the Code more Robust.
- Made a sound for collectibles — now you know if you have really picked it up!
- Fixed a big bug in the game loop in the re-generation of life in the hero.
- Fixed one more statistics doodle.
- Optimised the overall game loop.
- Edited the Ionic Rotator bullet sprite.
- Changed Version Info.
- Added GNU/Linux correctness too. Now things are going pretty well for that platform as well.

---

## 2002-12-31

- Fixed a miscellaneous bullet blitting bug.
- Fixed an Enemy Bullet Bug: What's an Hero without _any_ Enemies?
- Fixed the Enemy Bullet generation Speed Bug!
- Fixed a Huge Enemy Bullets Array Bug.
- Changed Orion to suit bullet sprites.
- Optimised the Game Loop.
- Made the hero Blink-Blank when damaged.
- Fixed a Loop Bug up.
- Fixed a miscellaneous Enemy Bullet generation bug.
- Added Sound logging support too.
- Fixed some explosion sprites up.
- Added Enemy Shoot Sounds as well! (much to the annoyance of Mom ....)

---

## 2002-12-30

- Made the Enemy move as well — can move horizontally / sinusoidally / cycloidally!
- Varied the enemy movement (randomized it) and the enemies' X speed as well!
- Added logging support. The game is more robust now, ensuring that all data files are loaded, and all routines are nicely done. This mechanism provides for debugging too.
- Added Exit Codes along with the game.
- Added logging for Screenshots and Menu.

---

## 2002-12-29

- Animated the Collectible Sprites (added some Boxes myself — doesn't look good!)
- Revamped the Data directory; Was getting Crowded.
- Fixed another general Data Directory bug.
- Fixed a Wonderful Frame Bug — Enemies were showing up as Collectibles!
- Changed Points for some Collectibles.
- Converted Output format of BMP to PCX. PCX files are much smaller than their equivalent BMP files. PCX is a lossless file format. You don't lose any palette information. Of course, if you want BMP, just change the values and Re-compile.
- Fixed a versioning Bug as well.

---

## 2002-12-27

- Implemented the mechanism of Screenshots (Yay! is that Professional?) in Menu and Game Loop only — verified that you take a Shot "after" a Draw. Go on, press F-12.
- ScreenShots are BMPs — for the welfare of all!
- Now you can Screenshot anywhere. PrintScreening is "Highly" discouraged: Why "Steal" when it's "Free"?
- Converted all bitmaps to 8-bit (most were). Things are faster.

---

## 2002-12-26

- Changed the Explosion Sprites — Are things a bit faster?

---

## 2002-12-25

- Added Generalised File Support.
- Added Better Sound Support.
- Star Colors can change too, with ColorScaling.
- Added a Sound and Music toggle (to the Immense Relief of the Guys at Home!).
- Changed Statistics toggle button to "T".
- Added Menu Music too!
- Revamped Sound Loop.
- Fixed a Music Loop Bug.
- Added Cycling in Menu.
- Optimised the Loop, and fixed one "Big" Bug.
- Changed the Sound for Vertical Ventura; Old one was "too" similiar to Fire Fury.
- Added More Enemies: More Pleasing to the Eyes!
- Revamped some DOCS.

---

## 2002-12-24

- Added proper wfixicon support.
- Added Allegro Static Linking Support, with another batch file created.
- Fixed Versioning Problems by using a Common Version Header File. Also fixed the title to be in touch with the current version.
- P for Pause is Back (In Addition to ESC)!
- Added Language Info.

---

## 2002-12-23

- Fixed another Loop Bug!

---

## 2002-12-22

- Animated the Hero's Bullets
- Simultaneously fixed a Bullet Programming Bug.
- Revamped the Game Loop.
- Compressed the executable with UPX.
- Lightning Lash is Horizontal again!
- Fixed a Big Frame Bullet Bug.
- Added another Bullet Class — Vertical Ventura: Mass Carnage!
- Changed the Icon to make it suit the Menu Logo.
- Clubbed the UI of Pause and Exit.

---

## 2002-12-21

- Fixed a Bullet Propulsion Bug.
- Made Lightning Lash Vertical!
- Renamed `ENEMY****.***` to `ENEM****.***` for MS-DOS 8.3 Compatibility (if people wanna play).
- Added a Funky Jagged Class of Bullets.
- Added the Version Info (Hurrah!) for the Executable.
- Added Collectible Support.
- Added a Statistics Toggle Button.
- Fixed the Music Thingy in the Game Loop.
- Fixed the Collectible Bug of lotsa' lives.
- Fixed the ammo limit and bullet damage limit bug.
- Increased the Number of Mines ('t was Difficult ...)
- Added a Pause after End Routine.
- Enhanced Ending Routines with GrayScale.
- Changed GrayScale into a Generic ColorScale!
- Extended it into a Rectangular region!

---

## 2002-12-20

- Fixed the Randomisation Bug: Everyone shall now experience "Blue Thunder"!
- Fixed the number of Enemy Frames in animation.
- Managed to propel the Bullets in Funky Directions.
