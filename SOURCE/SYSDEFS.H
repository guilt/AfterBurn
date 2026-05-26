//System Declarations .

#ifndef _M_SYSDEFS_
#define _M_SYSDEFS_

//Headers
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
#include "allegro.h"
#include "alfont.h"
#include "aldumb.h"

//General
#ifndef DIR_DELIMITER
#define DIR_DELIMITER '\\' //Change for GNU/Linux etc.
#else
#undef DIR_DELIMITER
#define DIR_DELIMITER '/'
#endif //DIR_DELIMITER
#ifndef ALT_DIR_DELIMITER
#define ALT_DIR_DELIMITER '/'
#else
#undef ALT_DIR_DELIMITER
#define ALT_DIR_DELIMITER '\\'
#endif //ALT_DIR_DELIMITER
#define ARROW_SEPARATOR '|'
#define SPACE_CHAR ' '
#define NEWLINE_CHAR '\n'
#define BACKSPACE_CHAR '\b'
#define RESULT_OK  0
#define RESULT_ERROR -1
#define STRING_BUFFER_SIZE  80

//GFX
#define GFX_AUTO_MODE GFX_AUTODETECT_FULLSCREEN
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BITS_PER_PIXEL 16
#define getMaxX() (SCREEN_WIDTH-1)
#define getMaxY() (SCREEN_HEIGHT-1)
#define getMinX() 0
#define getMinY() 0
#define getMidX() SCREEN_WIDTH/2
#define getMidY() SCREEN_HEIGHT/2
#define PALETTE_COLORS 256

//Videos
#define VIDEO_LOOP 1
#define VIDEO_NO_LOOP 0

//Sound
#define SOUND_LOOP 1
#define SOUND_NO_LOOP 0

#define MUSIC_LOOP TRUE
#define MUSIC_NO_LOOP FALSE

#define PAN_LEFT 0
#define PAN_CENTER 127
#define PAN_RIGHT 255

#define SPEED_SLOW 500
#define SPEED_NORMAL 1000
#define SPEED_FAST 2000

#define DEFAULT_PAN PAN_CENTER
#define DEFAULT_SPEED SPEED_NORMAL

#define NUM_DIGITAL_VOLUMES 8
#define NUM_MIDI_VOLUMES 8
#define SPEAKER_VOLUME -1

#define DUMB_MONO 1
#define DUMB_STEREO 2
#define DUMB_CHANNELS DUMB_STEREO

#define DUMB_POS 0
#define DUMB_FREQ 48000
#define DUMB_BUFSIZE 8192

#define DUMB_RES_QUAL 4
#define DUMB_ITM2M 64

#define DUMB_REFRESH (int)((1000*DUMB_BUFSIZE)/DUMB_FREQ)
#define DUMB_VOL 1.0

#define MUSIC DUH

#define musicExit() dumb_exit()

#define DIGITAL_MODE DIGI_AUTODETECT
#define MIDI_MODE MIDI_AUTODETECT

//Maximum Volume .
#define MAX_VOLUME 255

//KeyBoard
#define isKeyPressed() keypressed()
#define clearKeys() clear_keybuf()
#define getChar() while(!isKeyPressed()){}clearKeys()

//For Sound .
unsigned int DVOL=MAX_VOLUME,MVOL=MAX_VOLUME,PAN=DEFAULT_PAN;

#ifdef ALLEGRO_STATICLINK
//I got this Fund. Straight !
int (*keyPressed) () = keypressed;
#else
#define keyPressed keypressed
#endif //ALLEGRO_STATICLINK


//Working version : courtesy Nikhil .
int readKeyChar()
{
int c;
if(isKeyPressed())
{
if(key[KEY_ENTER]||key[KEY_ESC]){return NEWLINE_CHAR;}
if(key[KEY_SPACE]||key[KEY_TAB]){return SPACE_CHAR;}
if(key[KEY_BACKSPACE]){return BACKSPACE_CHAR;}
c=readkey();
clearKeys();
return (c&0xff);
}
return NEWLINE_CHAR;
}

#endif //_M_SYSDEFS_


