#ifndef _M_PAL_
#define _M_PAL_

#include "decl.h"

void seedRandom()
{
    srand((unsigned)time(NULL));
}

void doColorScale(BITMAP* src, int x, int y, int w, int h)
{
    int i, j;
    for (i = 0; i < h; i++) {
        int* line = (int*)src->line[y + i];
        for (j = 0; j < w; j++) {
            int pixelColor = line[x + j];
            int average = (getr(pixelColor) + getg(pixelColor) + getb(pixelColor)) / 3;
            line[x + j] = makecol(average * gameRScale / SIN_SCALE,
                                  average * gameGScale / SIN_SCALE,
                                  average * gameBScale / SIN_SCALE);
        }
    }
}

static int randomColorScalarInt(int override)
{
    if (override >= 0) { return override; }
    return ((rand() % ((DISTANT_COLOR / 2) + 1)) + (DISTANT_COLOR / 2)) * SIN_SCALE / DISTANT_COLOR;
}

void initGenColors()
{
    seedRandom();
    gameRScale = randomColorScalarInt(gameRScaleOverride);
    gameGScale = randomColorScalarInt(gameGScaleOverride);
    gameBScale = randomColorScalarInt(gameBScaleOverride);
    seedRandom();
    govRScale = randomColorScalarInt(govRScaleOverride);
    govGScale = randomColorScalarInt(govGScaleOverride);
    govBScale = randomColorScalarInt(govBScaleOverride);
}

void initStarColors()
{
    seedRandom();
    starRScale = randomColorScalarInt(starRScaleOverride);
    starGScale = randomColorScalarInt(starGScaleOverride);
    starBScale = randomColorScalarInt(starBScaleOverride);
    int i;
    for (i = 0; i < STAR_LAYERS; i++) {
        int layerIntensity = (stars[i].state + 1) * COLORS_PER_LAYER;
        starColors[i] = makecol(layerIntensity * starRScale / SIN_SCALE,
                                layerIntensity * starGScale / SIN_SCALE,
                                layerIntensity * starBScale / SIN_SCALE);
    }
}

void setPalette()
{
    palette[0].r = palette[0].g = palette[0].b = 0;
    set_palette(palette);
    activeColor = makecol(activeColorR, activeColorG, activeColorB);
    passiveColor = makecol(passiveColorR, passiveColorG, passiveColorB);
}

void updateGameOverColor()
{
    int fadeStep = abs((long)(gs.frameCount % (PALETTE_COLORS * 2)) - PALETTE_COLORS);
    gameOverColor = makecol(fadeStep * govRScale / SIN_SCALE,
                     fadeStep * govGScale / SIN_SCALE,
                     fadeStep * govBScale / SIN_SCALE);
}

#endif // _M_PAL_
