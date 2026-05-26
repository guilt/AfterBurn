# AfterBurn © Documentation

by **Karthik Kumar Viswanathan** & **Nikhil Karnad**

## Contents
- [Welcome](#welcome)
- [Disclaimer](#disclaimer)
- [Copyrights](#copyrights)
- [Game Notes](#game-notes)
- [Guns](#guns)
- [Collectibles](#collectibles)
- [Points](#points)
- [Enemies](#enemies)
- [User Experience](#user-experience)
- [Game Debugging](#game-debugging)
- [Themes](#themes)
- [Source Code](#source-code)
- [Contact Us](#contact-us)
- [Links](#links)

---

## Welcome
Thank you for procuring this game! Hope you have as much fun playing it as we had making it.

---

## Disclaimer
```
#include <std_disclaimer.h>
```
> "We do not accept responsibility for any effects, adverse or otherwise, that this source code and software may have on you, your computer, your sanity, your dog, and anything else that you can think of. Use it at your own risk."

---

## Copyrights
Permission is granted to anyone to use and redistribute the source code for any purpose including commercial applications, and to alter it and redistribute it freely **as long as** you don't claim it as yours or misrepresent its origin. Altered source code must be clearly marked as such, and must not be misrepresented as being the original source code.

---

## Game Notes
You are a Space Ship, in a Space Shooter: press **Space** to shoot ;)
Shoot anything that **isn't you**. And don't hit 'em or their bullets.

---

## Guns
*"Guns. Lots of 'em."*

You can use different types of bullets to fire. A short description of what's available:

### Gun 1
- **Bullet:** Fire Fury
- **Description:** Infinite ammo.

### Gun 2
- **Bullet:** Poison Pint
- **Description:** Full of Green Venom, Jagged Fire ...

### Gun 3
- **Bullet:** Lightning Lash
- **Description:** Phew! That's fast!

### Gun 4
- **Bullet:** Icy Impulse
- **Description:** Brrr... Freezing cold, fired in a V pattern.

### Gun 5
- **Bullet:** Vertical Ventura
- **Description:** Diamond, Vertical fire: Clean Sweep the screen with it.

### Gun 6
- **Bullet:** Mega Mine
- **Description:** A blast. Place 'em and go away.

---

## Collectibles
Collectibles are of two types: **Ammo** and **Others**.

### Ammo
Ammo is for guns. They add on for Guns other than that for Fire Fury, in which case the bullet power is boosted.

### Others
Others give health, lives, and points.

All collectibles disappear after some time.

---

## Points
Points can be accumulated by killing enemies or picking up collectibles. Once you cross a number of points, a life gets added. Once a new life gets added, your points drop down to zero. This is done till you have the maximum number of lives, after which you can accumulate as many points as you like, as long as you don't lose your life.

---

## Enemies
Enemies aren't spawned as such. Only certain types of enemies are allowed in a given level. One may usually expect the difficulty level to increase with level.

Do not stand in their way if they can shoot. Use appropriate guns instead.

---

## User Experience
Running it is no big deal. It plays _only_ with a keyboard as of now.

These are the controls:

### Generic Menus
| Keys  | Function                   |
|-------|----------------------------|
| Up    | Move Up                    |
| Down  | Move Down                  |
| Left  | Cycle Through Selection - Previous |
| Right | Cycle Through Selection - Next     |
| Enter | Activate/Toggle Selection  |
| M     | Toggle Music On/Off        |
| S     | Toggle SFX On/Off          |
| F12   | Take a PCX snapshot        |
| Esc   | Quit/To Previous Menu      |

### Game
| Keys  | Function                   |
|-------|----------------------------|
| Up    | Move Up                    |
| Down  | Move Down                  |
| Left  | Move Left                  |
| Right | Move Right                 |
| Space | Shoot                      |
| T     | Toggle Stats On/Off        |
| M     | Toggle Music On/Off        |
| S     | Toggle SFX On/Off          |
| P     | Pause                      |
| F12   | Take a PCX snapshot        |
| Esc   | Esc Menu                   |
| Q     | Cycle Through Guns - Previous |
| W     | Cycle Through Guns - Next  |
| 1     | Gun 1                      |
| 2     | Gun 2                      |
| 3     | Gun 3                      |
| 4     | Gun 4                      |
| 5     | Gun 5                      |
| 6     | Gun 6                      |
| 7     | Gun 7*                     |
| 8     | Gun 8*                     |
| 9     | Gun 9*                     |
| 0     | Gun 10*                    |

\* - subject to availability

---

## Game Debugging
Look at the **Game Log** (`GAMELOG.TXT`) if the game does not run properly. It is written to your **user data directory** (see [Themes](#themes)). In case you aren't able to see anything in it, report it to [us](#contact-us).

---

## Themes
Themes are to be written in a file having `.INI` extension. A batch file/shell script is to be provided, passing the `.INI` filename as a command line argument to the executable. The game files are to be placed in the same directory as the executable. A sample theme, **GREEN**, has been given. If you are interested in making themes, contact [us](#contact-us) for the **specs**.

### Data Directory Layout

The game uses three data directory concepts:

- **`dataDir`**: The game's root directory (where `AB.EXE` is located). All game data assets (`DATA\`, `THEME\`) are resolved relative to this directory.
- **`themeDataDir`**: The active theme directory, derived from the `.INI` path — e.g., `dataDir\THEME\GREEN\`.
- **`userDataDir`**: The user-specific data directory, located at:
  - Windows: `%APPDATA%\AfterBurn\`
  - Linux: `$XDG_DATA_HOME/AfterBurn/` or `~/.AfterBurn/`
  - Fallback: same directory as the executable (DOS/9x)

The following files are written to `userDataDir`:
- **`GAMELOG.TXT`** — game log (for debugging)
- **`DEFAULT.HSC`** — high scores
- **`<theme>.PRF`** — user preferences (e.g. `GREEN.PRF`)

Theme files (`.INI`, graphics, sounds, music) are always read-only and never overwritten.

---

## Source Code
The source code (in **C**) for this game has been given. We believe in writing useful (?) free software. We hope you appreciate our efforts and respect [our feelings](#welcome) and [copyrights](#copyrights) for this game.

- You should have these external libraries to make the program: **Allegro**, **DUMB**, and **Allegrofont**.
- Ogg Vorbis playback support thanks to **Sven** (dumbogg).
- Has been tested on **Windows** and **GNU/Linux** platforms.
- You should have a compliant compiler such as **GCC**.
- **MIDI** isn't used in this distro. If you are keen on it, you can see for yourself that the source is **MIDI** compatible.
- For a list of what changes/fixes/improvements have been done to the code, please read [CHANGES.MD](CHANGES.MD).
- For developer documentation (build, architecture, code conventions), see [DEVELOP.MD](DEVELOP.MD).

---

## Contact Us
1. **Karthik Kumar Viswanathan** ([e-mail](mailto:karthikkumar@gmail.com))
2. **Nikhil Karnad** ([e-mail](mailto:nikhil.karnad@gmail.com))

---
# Links
- **License:** [LICENSE.MD](LICENSE.MD) (MIT License)
