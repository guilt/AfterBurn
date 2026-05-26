#ifndef _M_VID_
#define _M_VID_

#include "decl.h"
#include "files.h"

static void renderVideoFrame()
{
    int vw = fli_bitmap->w, vh = fli_bitmap->h;
    int dw, dh;

    // Fit video within letterboxed display area, maintain aspect (integer math)
    if (vw * scaleDestH <= vh * scaleDestW) {
        dw = vw * scaleDestH / vh;
        dh = scaleDestH;
    } else {
        dw = scaleDestW;
        dh = vh * scaleDestW / vw;
    }
    videoX = scaleDestX + (scaleDestW - dw) / 2;
    videoY = scaleDestY + (scaleDestH - dh) / 2;
    videoWidth = dw;
    videoHeight = dh;

    clear_bitmap(backBuffer);
    stretch_blit(drawingBuffer, backBuffer, 0, 0, vw, vh, videoX, videoY, videoWidth, videoHeight);
    blit(backBuffer, screen, 0, 0, 0, 0, screenWidth, screenHeight);
    vsync();
}

static void videoCleanup()
{
    clearScreen();
    clearKeys();
}

static int onFrameInterrupt()
{
    renderVideoFrame();
    return isKeyPressed();
}

static void playVideo(struct video v)
{
    videoDataFile = NULL;
    packfile_password(v.password);
    convertToLocalPath(v.filename);
    videoDataFile = loadDatafile(v.filename);
    if (!videoDataFile) {
        logWrite("Couldn't process video file: %s", v.filename);
        return;
    }
    logWrite("Processing video file: %s", v.filename);

    if (videoSample) { stopVideoSound(); }
    videoSample = (SAMPLE*)videoDataFile[v.animIndex].dat;
    fliData = videoDataFile[v.videoIndex].dat;
    reset_fli_variables();

    if (videoSample) { playVideoSound(); }
    if (fliData) {
        videoCleanup();
        clear_to_color(screen, makecol(0, 0, 0));
        play_memory_fli(fliData, drawingBuffer, VIDEO_NO_LOOP, onFrameInterrupt);
    }
    if (videoSample) { stopVideoSound(); }
    if (!soundError) { unload_datafile(videoDataFile); }
    videoCleanup();
}

void playCinema(int index)
{
    if (numVideos <= 0) { return; }
    if (index >= numVideos) { return; }
    playVideo(videos[index]);
}

#endif //_M_VID_
