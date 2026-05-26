//Statistics .

#ifndef _M_STAT_
#define _M_STAT_

#include "decl.h"

#define toggleStats() { showStats = (1 - showStats); }

static int getProgressColor(int value, int maxValue)
{
    int pct = value * 100 / maxValue;
    if (pct < 17)  { return makecol(255, 0, 0); }       /* red */
    if (pct < 33)  { return makecol(255, 165, 0); }     /* orange */
    if (pct < 50)  { return makecol(255, 255, 0); }     /* yellow */
    if (pct < 67)  { return makecol(0, 255, 0); }       /* green */
    if (pct < 84)  { return makecol(0, 0, 255); }       /* blue */
    return makecol(138, 43, 226);                        /* violet */
}

void drawProgressBar()
{
    if (gs.state != STATE_PLAYING) { return; }
    unsigned int killTarget = killsToAdvanceLevel[gs.level];
    if (killTarget <= 0) { return; }

    int barWidth = SCREEN_WIDTH / 10;
    int barHeight = 10;
    int barX = LEVX + iWidth(ICON_LEVEL) + HORIZONTAL_SPACING + (FONT_SIZE * 2);
    int barY = LEVY + (iHeight(ICON_LEVEL) - barHeight) / 2;

    int progress = gs.enemiesKilled * barWidth / killTarget;
    if (progress > barWidth) { progress = barWidth; }

    rectfill(drawingBuffer, barX, barY, barX + barWidth, barY + barHeight, makecol(30, 30, 30));
    if (progress > 0) {
        rectfill(drawingBuffer, barX, barY, barX + progress, barY + barHeight,
                 getProgressColor(progress, barWidth));
    }
}

void drawStatusBar()
{
    int i;
    if (!showStats) { return; }
    drawProgressBar();

    /* top-left: medal icon + health */
    drawSpriteSize(statIcons[ICON_MEDAL], MEDIX, MEDIY, iWidth(ICON_MEDAL), iHeight(ICON_MEDAL));
    if (playerHealth > 0) {
        drawText(MEDIX + iWidth(ICON_MEDAL) + HORIZONTAL_SPACING, MEDIY, activeColor, "%3u", playerHealth);
    } else {
        drawText(MEDIX + iWidth(ICON_MEDAL) + HORIZONTAL_SPACING, MEDIY, activeColor, "%s", unknownStr);
    }

    /* top-center: level icon + level number */
    drawSpriteSize(statIcons[ICON_LEVEL], LEVX, LEVY, iWidth(ICON_LEVEL), iHeight(ICON_LEVEL));
    drawText(LEVX + iWidth(ICON_LEVEL) + HORIZONTAL_SPACING, LEVY, activeColor, "%u", gs.level);

    /* mid-left: life icons (one per remaining life) */
    for (i = 0; i < gs.lives; i++) {
        drawSpriteSize(statIcons[ICON_LIFE], LIFX + (i * iWidth(ICON_LIFE)), LIFY,
                       iWidth(ICON_LIFE), iHeight(ICON_LIFE));
    }

    /* bottom-left: score icon + score value */
    drawSpriteSize(statIcons[ICON_SCORE], SCOX, SCOY, iWidth(ICON_SCORE), iHeight(ICON_SCORE));
    drawText(SCOX + iWidth(ICON_SCORE) + HORIZONTAL_SPACING, SCOY, activeColor, scoreFormat, gs.score);

    /* bottom-center: bullet name + ammo count */
    if (!currentBullet) {
        drawText(AMMOX, AMMOY, activeColor, "%s (%s)", bulletNames[currentBullet], infiniteStr);
    } else {
        drawText(AMMOX, AMMOY, activeColor, "%s (%3d)", bulletNames[currentBullet], bulletAmmo[currentBullet]);
    }
}

#endif //_M_STAT_

