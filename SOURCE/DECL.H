//Declarations .

#ifndef _M_DECL_
#define _M_DECL_

//Headers
#include <stdarg.h>
#include <stdio.h>
#include "sysdefs.h"
#include "ver.h"

//General
#define RANDOM_RANGE  64
#define FONT_SIZE 20
#define NUM_MENU_ITEMS 5
#define VERTICAL_SPACING 10
#define HORIZONTAL_SPACING 10
#define MENU_Y_OFFSET ((logo->h)*2 + VERTICAL_SPACING)
#define flickerOffset() ((2*(gs.frameCount%2))-1)
#define PI 3.141592653 //Enough Precision !!!


//Virtual Canvas — game renders at 640x480, then stretched to display
BITMAP *virtualBuffer = NULL;
BITMAP *backBuffer = NULL;
int screenWidth = 640;
int screenHeight = 480;
int savedResW = 0, savedResH = 0, savedGfxMode = 0;
int scaleDestX = 0, scaleDestY = 0, scaleDestW = 640, scaleDestH = 480;

#define drawingBuffer virtualBuffer
#define render() { \
    clear_bitmap(backBuffer); \
    stretch_blit(drawingBuffer, backBuffer, 0, 0, 640, 480, scaleDestX, scaleDestY, scaleDestW, scaleDestH); \
    blit(backBuffer, screen, 0, 0, 0, 0, screenWidth, screenHeight); \
    vsync(); \
}
#define clearScreen() clear_bitmap(drawingBuffer) 

//Drawing shortcuts (hide drawingBuffer/gameFont repetition)
#define drawSprite(src, x, y)                masked_blit((src), drawingBuffer, 0, 0, (x), (y), (src)->w, (src)->h)
#define drawSpriteSize(src, x, y, w, h)      masked_blit((src), drawingBuffer, 0, 0, (x), (y), (w), (h))
#define drawLogoCentered()                   drawSpriteSize(logo, getMidX() - (logo->w / 2), (logo->h) / 4, logo->w, logo->h)

//Effects
#define DISTANT_COLOR 200
#define MAX_STARS 300
#define STAR_SPEED 1
#define COLORS_PER_LAYER (PALETTE_COLORS/STAR_LAYERS)
#define STAR_FREQUENCY 500
#define STAR_LAYERS 8
#define MAX_EXPLOSION_FRAMES 10
#define YPDIR 6
#define YNDIR -YPDIR
#define NODIR 0
#define XPDIR 0.75
#define XNDIR -XPDIR

//Enemy
#define EXPLOSION_MULTIPLIER  2
#define SIN_TABLE_SIZE 360
#define ENEMY_X_LIMIT (SCREEN_WIDTH)
#define MAX_ENEMIES  20
#define MAX_ENEMY_TYPES  120
#define eHeight(i) (enemies[i].bitmap->h)
#define eWidth(i) (enemies[i].bitmap->w)
#define MAX_ENEMY_FRAMES 6
enum EnemyMovement {
    ENEMY_HORIZONTAL,
    ENEMY_SINE,
    ENEMY_LINEAR,
    ENEMY_MOVEMENT_TYPES
};

//Enemy's Bullets
#define MAX_ENEMY_BULLET_TYPES 10
#define MAX_ENEMY_BULLETS 80
#define MAX_ENEMY_BULLET_FRAMES 4
#define ebHeight(i) (ebullets[i].bitmap->h)
#define ebWidth(i) (ebullets[i].bitmap->w)

//Hero
#define MAX_SHIP_FRAMES 10
#define MAX_SHIP_BLINK_FRAMES 10
#define hHeight playerShipBitmaps[playerFrame]->h
#define hWidth playerShipBitmaps[playerFrame]->w
#define ENEMY_BULLET_FIRE_DELAY -1

//Score
#define SCORE_MULTIPLIER 15
#define LIFE_GIVING_SCORE 20000
#define MAX_SCORE 99999999

//Hero's Bullets
#define MAX_BULLET_TYPES 10
#define MAX_BULLETS 80
#define MAX_BULLET_FRAMES 4
enum BulletDirection {
    DIR_HORIZONTAL,
    DIR_VERTICAL,
    DIR_Y,
    DIR_JITTER,
    DIRECTION_MAX_TYPE
};
enum VerticalDir { DIR_UP, DIR_DOWN };
#define DIRECTION_ALTERNATOR 2
#define bHeight(i) (bullets[i].bitmap->h)
#define bWidth(i) (bullets[i].bitmap->w)

//Collectibles
#define MAX_COLLECTIBLES 30
#define MAX_COLLECTIBLE_FRAMES 8
#define COLLECTIBLE_SPAWN_INTERVAL 10
#define COLLECTIBLE_ENEMY_THRESHOLD 11
#define COLLECTIBLE_RANDOM_THRESHOLD 17
#define NUM_COLLECTIBLE_TYPES 2
enum CollectibleType { COLLECTIBLE_AMMO, COLLECTIBLE_OTHER };
#define HEALTH_THRESHOLD 0.5
#define MAX_COLLECTIBLE_TYPES 20
#define cHeight(i) (colls[i].bitmap->h)
#define cWidth(i) (colls[i].bitmap->w)

//Statistics
enum StatIcon { ICON_MEDAL, ICON_LIFE, ICON_SCORE, ICON_LEVEL, NUM_ICONS };
#define MEDIX 0
#define MEDIY 0
#define LIFX 0
#define LIFY (MEDIX+iWidth(ICON_MEDAL)+VERTICAL_SPACING)
#define SCOX 0
#define SCOY (getMaxY()-FONT_SIZE-1)
#define LEVX (getMidX())
#define LEVY 0
#define iHeight(i) (statIcons[i]->h)
#define iWidth(i) (statIcons[i]->w)
#define AMMOX (getMidX())
#define AMMOY (getMaxY()-FONT_SIZE-1)

//Scores
#define MAX_HIGH_SCORES 10
#define HIGH_SCORES_MENU_ITEM 2
#define HIGH_SCORES_NAME_X 10
#define HIGH_SCORES_LEVEL_X (getMidX()*1.2)
#define HIGH_SCORES_SCORE_X (getMidX()*1.6)
#define NAMELEN 30

//Credits
#define CREDITS_MENU_ITEM 3
#define CREDIT_SPEED 1
#define NUM_CREDIT_LINES 90
#define NUM_CREDIT_TICKS ((NUM_CREDIT_LINES*(FONT_SIZE+CREDIT_LINE_SPACING))/CREDIT_SPEED)
#define CREDIT_X_MULTIPLIER SCREEN_WIDTH/4
#define CREDIT_LINE_SPACING (FONT_SIZE/4)
#define MAX_COLOR_VALUE 256

//Options
#define NUM_OPTIONS_MENU_ITEMS 4
#define NUM_OPTION_STATES 2

//Levels
#define MAX_LEVELS 20

//Shots


//Videos
#define MAX_VIDEOS 20


//General
//State Machine
#define LPS 60
#define NUM_STATES (STATE_EXIT + 1)

enum GameState {
    STATE_MAIN_MENU,
    STATE_INTRO_VIDEO,
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_PAUSED,
    STATE_QUIT_DIALOG,
    STATE_RESTART,
    STATE_LEVEL_CLEAR,
    STATE_OPTIONS,
    STATE_HIGH_SCORES,
    STATE_HIGH_SCORE_ENTRY,
    STATE_CREDITS,
    STATE_EXIT
};
struct GS {
    int state;
    int ticks;
    int prevState;
    int isDone;
    unsigned int frameCount;
    int score;
    int lives;
    int level;
    int win;
    int isInGame;
    unsigned int enemiesKilled;
} gs = {STATE_MAIN_MENU, 0, STATE_MAIN_MENU, 0, 0, 0, 0, 0, 0, 0, 0};
static void setGameState(int newState) { gs.state = newState; gs.ticks = 0; }

void (*stateInput[NUM_STATES])();
void (*stateUpdate[NUM_STATES])();
void (*stateRender[NUM_STATES])();

#ifndef KEY_MAX
#define KEY_MAX 128
#endif
volatile char lastKeys[KEY_MAX];
#define EDGE(k) (key[k] && !lastKeys[k])
char windowTitle[STRING_BUFFER_SIZE]={""};

//Credits
int creditsScrollEnabled=1;
//Please Do Not Change ....
const char names[NUM_CREDIT_LINES][STRING_BUFFER_SIZE]={
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"Game Design, Programming,..",
"",
"Karthik Kumar Viswanathan \"Guilt\"",
"Nikhil Karnad \"Niks\" / \"Scribbleink\"",
"",
"",
"Support and Libraries",
"",
"Inspired by: Johan Pietz",
"Allegro: alleg.sourceforge.net",
"JGMOD: Jefferey Guan Foo Wah",
"DUMB: Ben Davis, Robert J Ohannessian and Julien Cugniere",
"Sven: Ogg support via DUMB",
"Ogg Vorbis: xiph.org ",
"AllegroFont: Javier Gonzalez and Doug Eleveld",
"",
"",
"Our Dear Friends",
"",
"Abilash \"Abi\"",
"Adarsh \"Adu\"",
"Anand \"KPoo\"",
"Anand \"Miss\"",
"Arun",
"Arun \"Paal Pandi\"",
"Arun \"Strings\" Vijay",
"Balakrishnan \"Balkoo\"",
"Chandramouli \"Mezhi\"",
"Deepak \"Goundar\"",
"Ganesh@Baskaran \"Gaba\"",
"Gautham \"Thalai\"",
"Gurulakshmanan \"Guru\"",
"Karthikeyan \"Kama\"",
"Karthikeyan \"Patti\"",
"Kaushik \"Vetti\" Narayanan",
"Koushiik",
"Laxmikanth \"Mech\"",
"Madhu \"Palli\"",
"Madhu \"Upili\"",
"Prakash \"Baadu\"",
"Prasanna \"Kapa\"",
"Prashant \"Pachu\"",
"Raghuraman \"B ra\"",
"Ramakrishnan \"Mottai\"",
"Renju",
"Shameem \"Appu\"",
"Srinidhi \"Gultu\"",
"Sriram \"Alladi\"",
"Sudharsan \"Sucha\"",
"Suudhan \"Semi\"",
"Swagath \"Golt\"",
"Ravi",
"Vasanth Kumar",
"Vevek \"Pondy\"",
"Vinu",
"",
"",
"The Rest",
"",
"Almighty",
"Family",
"ANC",
"Munnaji/Nagarji Redi",
"One Typhoon Web-Cam",
"MOD Archives",
"XMMS/Winamp",
"Action Arcade Adventure Set",
"Microsoft Paint",
"GCC and Dev-C++",
"Horse Shelters",
"and, of course, Cut-Copy-Paste ;)",
"",
"",
"We Thank You For Playing ...",
"",
"",
"",
"",
"",
""
};

//Options .
int optionMenuSelection=0;
char optionsMenu[NUM_OPTIONS_MENU_ITEMS][STRING_BUFFER_SIZE]={
{"SFX : %s"}
,{"levelMusic : %s"}
,{"Statistics : %s"}
,{"Master Volume : %3u"}
//,{"MIDI Volume : %3u"}
}
;
char optionStates[NUM_OPTION_STATES][STRING_BUFFER_SIZE]={{"Off"},{"On"}};

//UDF
char unimplementedStr[STRING_BUFFER_SIZE]={"Method Not Implemented Yet"};

//Videos
DATAFILE *videoDataFile;
SAMPLE *videoSample;
void *fliData;
struct video
{
char filename[STRING_BUFFER_SIZE],password[STRING_BUFFER_SIZE];
int animIndex,videoIndex;
}
videos[MAX_VIDEOS]= //Video Data .
{
{"DATA\\VIDEOS\\ALLEGRO.VID","AfterBurn Video",0,1},
};
int videoWidth,videoHeight,videoX,videoY;
int numVideos=1;
int allegroLogoIndex=0;

//Log
FILE *logFile = NULL;

//Ini

FILE *iniFile = NULL;

//High Scores
int hscWriteNeeded = 0;
FILE *hscFile = NULL;
char hsNameStr[STRING_BUFFER_SIZE]={"Name"},hsLevelStr[STRING_BUFFER_SIZE]={"Level"},hsScoreStr[STRING_BUFFER_SIZE]={"Score"},
     hsPromptStr[STRING_BUFFER_SIZE]={"You have achieved a High Score !! Enter your name... "},
     anonymousStr[NAMELEN+1]={"Anonymous"};

//Statistics
int showStats=1;
BITMAP *statIcons[NUM_ICONS];
char textBuffer[STRING_BUFFER_SIZE];

//Levels
int numLevels=5;
unsigned int killsToAdvanceLevel[MAX_LEVELS]={50,75,100,125,150};
char levelClearStr[STRING_BUFFER_SIZE]={"Level Clear"},winStr[STRING_BUFFER_SIZE]={"You've wonSample"};

//Scores
int numHighScores=10;
char scoreFormat[STRING_BUFFER_SIZE]={"%08u"};
struct Player
{
 char name[NAMELEN+1];
 unsigned int win, level, lives, score, life;
}Players[MAX_HIGH_SCORES+1]=
{
{"Pachu",0,0,0,0,0},
{"B ra",0,0,0,0,0},
{"Kaushik",0,0,0,0,0},
{"Alladi",0,0,0,0,0},
{"Vinu",0,0,0,0,0},
{"Scribbleink",0,0,0,0,0},
{"Mezhi",0,0,0,0,0},
{"Koushiik",0,0,0,0,0},
{"Mottai",0,0,0,0,0},
{"Guilt",0,0,0,0,0}
},tempPlayer;

//Menu
ALFONT_FONT *gameFont, *creditsFont, *highScoresValuesFont;
BITMAP *logo;
int activeColor=0, passiveColor=0, gameOverColor=0, menuSelection=0;
char menu[NUM_MENU_ITEMS][STRING_BUFFER_SIZE]={"Play","Options","High Scores","Credits","Exit"};

//Sounds
static AL_DUH_PLAYER *volatile dumbPlayer=NULL;
static int volatile semaphore=0;

MUSIC *levelMusic[MAX_LEVELS], *menuMusic;
int soundError=0,musicError=0,isSoundEnabled=1,isMusicEnabled=1;

//Display 


//Bullets
int currentBullet,numBulletTypes=6;
SAMPLE *shootSamples[MAX_BULLET_TYPES];
BITMAP *bulletBitmaps[MAX_BULLET_TYPES][MAX_BULLET_FRAMES];
int bulletsPerType[MAX_BULLET_TYPES]={35,25,20,20,20,8,0,0,0,0},
    bulletXSpeed[MAX_BULLET_TYPES]={16,20,18,15,0,0,0,0,0,0}, //< Thinnest FrameWidth of Enemy !
    bulletYSpeed[MAX_BULLET_TYPES]={0,10,0,5,20,0,0,0,0,0},    //< Thinnest FrameHeight of Enemy !
    bulletFrames[MAX_BULLET_TYPES]={2,2,2,4,4,1,0,0,0,0},
    bulletSpawnInterval[MAX_BULLET_TYPES]={1,2,3,3,1,2,0,0,0,0},
    bulletFrameInterval[MAX_BULLET_TYPES]={2,4,3,4,6,1,1,1,1,1},
    bulletDirection[MAX_BULLET_TYPES]={DIR_HORIZONTAL,DIR_JITTER,DIR_HORIZONTAL,DIR_VERTICAL,DIR_Y,DIR_HORIZONTAL,DIR_HORIZONTAL,DIR_HORIZONTAL,DIR_HORIZONTAL,DIR_HORIZONTAL},
    defaultBulletAmmo[MAX_BULLET_TYPES]={-1,50,50,100,100,40,0,0,0,0},
    maxBulletAmmo[MAX_BULLET_TYPES]={-1,3000,3000,2500,2000,400,0,0,0,0},
    bulletAmmo[MAX_BULLET_TYPES];
int defaultBulletDamage[MAX_BULLET_TYPES]={1,2,3,4,7,9,0,0,0,0},
    maxBulletDamage[MAX_BULLET_TYPES]={50,40,40,40,45,60,0,0,0,0},
    bulletDamage[MAX_BULLET_TYPES];
char bulletNames[MAX_BULLET_TYPES][STRING_BUFFER_SIZE]=
{"Fire Fury","Poison Pint","Lightning Lash","Icy Impulse","Vertical Ventura","Mega Mine",
"","","",""},infiniteStr[STRING_BUFFER_SIZE]={"Infinite"};
struct bullholder
{
int x,y;
int active,type;
int directionMode,movementMode;
BITMAP *bitmap;
}
bullets[MAX_BULLETS];

//Collectibles
int numCollectibles=5;
BITMAP *ammoCollectBitmaps[MAX_BULLET_TYPES][MAX_COLLECTIBLE_FRAMES];
BITMAP *collectBitmaps[MAX_COLLECTIBLE_TYPES][MAX_COLLECTIBLE_FRAMES];
int collectHealthBonus[MAX_COLLECTIBLE_TYPES]={30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int collectLivesBonus[MAX_COLLECTIBLE_TYPES]={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int collectScore[MAX_COLLECTIBLE_TYPES]={450,1050,3500,8000,12000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int collectFrames[MAX_COLLECTIBLE_TYPES]={4,4,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int collectFrameInterval[MAX_COLLECTIBLE_TYPES]={40,40,20,10,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
int collectibleDuration[MAX_COLLECTIBLE_TYPES]={200,400,350,350,250,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int ammoCollectQuantity[MAX_BULLET_TYPES]={0,300,250,150,200,100,0,0,0,0};
int ammoCollectScore[MAX_BULLET_TYPES]={100,25,20,20,30,35,0,0,0,0};
int ammoCollectFrames[MAX_BULLET_TYPES]={4,4,4,4,4,4,0,0,0,0};
int ammoCollectFrameInterval[MAX_BULLET_TYPES]={40,40,40,40,40,40,0,0,0,0};
int ammoCollectDuration[MAX_BULLET_TYPES]={600,800,800,800,400,300,0,0,0,0};
struct collectibleholder
{
int x,y;
int active,type,itemIndex;
BITMAP *bitmap;
}colls[MAX_COLLECTIBLES];

//Effects
SAMPLE *explodeSample;
char gameOverStr[STRING_BUFFER_SIZE]={"Game Over"},pauseStr[STRING_BUFFER_SIZE]={"Game Paused"},
quitDialogStr[STRING_BUFFER_SIZE]={"Quit (Y/N) ?"},anyKeyStr[STRING_BUFFER_SIZE]={"Press any Key to Continue"};
int starColors[STAR_LAYERS],numStars=288,numExplodeFrames=10;
struct starholder
{
int x,y,state;
}stars[MAX_STARS];
BITMAP *explodeFrames[MAX_EXPLOSION_FRAMES];

//Hero
char unknownStr[STRING_BUFFER_SIZE]={"???"};
int shipBlinkFrames=3,shipFrames=5,shipAnimSpeed=10,maxBlinkActiveFrames=10,maxHealth=100,maxLives=3,
 maxPossibleLives=10,shipSpeed=8,playerX,playerY,playerFrame,playerBlinkFrame,playerHealth,isBlinking,hasScored;
 BITMAP *currentBmp,*playerBlinkBitmaps[MAX_SHIP_BLINK_FRAMES],*playerShipBitmaps[MAX_SHIP_FRAMES],*playerBlankBitmap;
SAMPLE *playerDieSample,*collectSample,*wonSample;

//Enemy
int numEnemyTypes=11; //With Thanks to Nikhil !
BITMAP *enemyAnimations[MAX_ENEMY_TYPES][MAX_ENEMY_FRAMES];
int enemyHealth[MAX_ENEMY_TYPES]={1,4,3,2,2,1,3,2,1,2,4};
int enemyFrames[MAX_ENEMY_TYPES]={4,4,4,2,1,2,2,3,1,1,1};
int enemyFrameInterval[MAX_ENEMY_TYPES]={25,30,60,40,1,40,40,40,1,1,1};
int enemyDamage[MAX_ENEMY_TYPES]={2,10,7,5,2,3,10,4,5,5,10};
int enemyHasMovement[MAX_ENEMY_TYPES]={0,1,1,1,1,0,0,0,0,0,0};
int enemyBulletTypeByEnemy[MAX_ENEMY_TYPES]={-1,0,1,-1,2,-1,3,-1,-1,-1,4};
int enemyLevelMap[MAX_LEVELS][MAX_ENEMY_TYPES]= // A Mapping to check if enemies are
                                     // available .
{
{1,0,0,1,0,1,0,1,0,0,0},
{1,1,0,0,0,0,1,0,0,1,0},
{1,0,1,0,1,0,1,0,0,1,0},
{1,1,1,0,1,1,1,0,1,1,0},
{1,1,1,1,1,1,1,1,1,1,1}
};
int enemyXSpeed[MAX_ENEMY_TYPES]={2,2,2,3,2,4,3,2,3,3,2},
    enemyYSpeed[MAX_ENEMY_TYPES]={0,55,45,35,25,0,0,0,0,0,0};
#define SIN_SCALE 10000
int sinTable[SIN_TABLE_SIZE],cosTable[SIN_TABLE_SIZE];
struct enemyholder
{
 int x, y, active, type, explosionFrame, movementType, oscillationTimer;
 BITMAP *bitmap;
}enemies[MAX_ENEMIES];

//Enemy Bullets
int currentEnemyBullet,numEnemyBulletTypes=5;
SAMPLE *enemyShootSamples[MAX_ENEMY_BULLET_TYPES];
BITMAP *enemyBulletBitmaps[MAX_ENEMY_BULLET_TYPES][MAX_ENEMY_BULLET_FRAMES];
int enemyBulletXSpeed[MAX_ENEMY_BULLET_TYPES]={10,15,30,15,25,0,0,0,0,0}, //<FrameWidth of Hero !
    enemyBulletFrames[MAX_ENEMY_BULLET_TYPES]={1,1,1,1,1,0,0,0,0,0},
    enemyBulletSpawnInterval[MAX_ENEMY_BULLET_TYPES]={10,15,20,5,15,0,0,0,0,0},
    enemyBulletFrameInterval[MAX_ENEMY_BULLET_TYPES]={1,1,1,1,1,1,1,1,1,1};
int enemyBulletDamage[MAX_ENEMY_BULLET_TYPES]={8,5,9,7,10,0,0,0,0,0};
struct ebullholder
{
int x,y,type,active;
BITMAP *bitmap;
}
ebullets[MAX_ENEMY_BULLETS];

//Palette
PALETTE palette;

int gameRScaleOverride=4000,gameGScaleOverride=6000,gameBScaleOverride=5500,gameRScale,gameGScale,gameBScale,   //Greenish-Cyan .
    starRScaleOverride=-1,starGScaleOverride=-1,starBScaleOverride=-1,starRScale,starGScale,starBScale, //Random .
    govRScaleOverride=SIN_SCALE,govGScaleOverride=0,govBScaleOverride=0,govRScale,govGScale,govBScale;    //Red .

int activeColorR=255,activeColorG=255,activeColorB=210,
    passiveColorR=128,passiveColorG=64,passiveColorB=0;

void logInit();
void logWrite(const char* fmt, ...);
void logClose();
void iniOpenFile();
void writePreferences();

//State Machine
void incrementSpeedCounter();
void initStateMachine();

//Null stubs
void nullInput();
void nullUpdate();
void nullRender();

//Drawing
void drawMenu();
void drawGame();
void drawOptionsMenu();

//Main Menu
void menuInput();
void menuRender();

//Playing
void playingInput();
void playingUpdate();
void playingRender();
void initGame();
void oneTimeGameInit();

//Game Over
void gameOverInput();
void gameOverRender();

//Pause / Quit Dialog
void pauseInput();
void pauseRender();

//Restart
void restartInput();
void restartRender();

//Level Clear
void levelClearInput();
void levelClearRender();

//Options
void optionsInput();
void optionsRender();

//High Scores
void highScoresInput();
void highScoresRender();

//High Score Entry
void highScoreEntryInput();
void highScoreEntryUpdate();
void highScoreEntryRender();

//Credits
void creditsInput();
void creditsUpdate();
void creditsRender();

//Unimplemented Feature Dialog
void unimplementedInput();
void unimplementedUpdate();
void unimplementedRender();

//P.S : Multiply all values by the highest possible individual palette value (i.e. 255) - Ta Da !

//Text drawing wrappers (eliminate sprintf+drawText anti-pattern)
static void drawText(int x, int y, int color, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);
    alfont_textout_aa(drawingBuffer, gameFont, textBuffer, x, y, color);
}

static void drawCenteredText(int y, int color, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);
    alfont_textout_centre_aa(drawingBuffer, gameFont, textBuffer, getMidX(), y, color);
}

static void drawCenteredTextNoAA(int y, int color, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);
    alfont_textout_centre(drawingBuffer, gameFont, textBuffer, getMidX(), y, color);
}

static void drawTextCenteredAt(int x, int y, int color, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(textBuffer, format, args);
    va_end(args);
    alfont_textout_centre_aa(drawingBuffer, gameFont, textBuffer, x, y, color);
}

//Animation frame index helpers — safe range [0, numFrames)
static inline int getAnimFrameIndex(int frameCount, int numFrames, int interval) {
    return (frameCount % (numFrames * interval)) / interval;
}
static inline int getAnimFrameIndexRand(int frameCount, int numFrames, int interval) {
    return ((frameCount + rand()) % (numFrames * interval)) / interval;
}

static const char* getUserName()
{
    const char* name = getenv("USERNAME");
    if (name && *name) return name;
    name = getenv("USER");
    if (name && *name) return name;
    name = getenv("LOGNAME");
    if (name && *name) return name;
    return "Player";
}
#endif //_M_DECL_

