#ifndef _M_SOUND_
#define _M_SOUND_

#include "decl.h"
#include "flog.c"

void stopCollectSound();
void stopShootSound();
void stopExplodeSound();
void stopPlayerExplodeSound();
void stopEnemyShootSound();
void stopWonSound();

static void pollMod()
{
    ++semaphore;
    al_poll_duh(dumbPlayer);
    --semaphore;
}

static void stopMod()
{
    AL_DUH_PLAYER* p;
    if (dumbPlayer) {
        p = dumbPlayer;
        remove_int(pollMod);
        dumbPlayer = NULL;
        while (semaphore) { rest(0); }
        al_stop_duh(p);
    }
}

static void playMod(MUSIC* duh, int shouldLoop)
{
    stopMod();
    dumbPlayer = al_start_duh(duh, DUMB_CHANNELS, DUMB_POS, DUMB_VOL, DUMB_BUFSIZE, DUMB_FREQ);
    if (dumbPlayer) { install_int_ex(pollMod, BPS_TO_TIMER(DUMB_REFRESH)); }
}

void preSoundInit()
{
    reserve_voices(NUM_DIGITAL_VOLUMES, NUM_MIDI_VOLUMES);
    set_volume_per_voice(SPEAKER_VOLUME);
    dumb_resampling_quality = DUMB_RES_QUAL;
    dumb_it_max_to_mix = DUMB_ITM2M;
    dumb_register_packfiles();
}

void applyVolume()
{
    if (soundError) { return; }
    set_volume(DVOL, MVOL);
}

void postSoundInit()
{
    if (soundError) { return; }
    applyVolume();
}

void stopMusic()
{
    if (soundError || musicError) { return; }
    stopMod();
}

void pauseMusic()
{
    if (soundError || musicError || !dumbPlayer) { return; }
    remove_int(pollMod);
    while (semaphore) { rest(0); }
    al_pause_duh(dumbPlayer);
}

void resumeMusic()
{
    if (soundError || musicError || !dumbPlayer) { return; }
    al_resume_duh(dumbPlayer);
    install_int_ex(pollMod, BPS_TO_TIMER(DUMB_REFRESH));
}

void playMusic()
{
    if (soundError || musicError) { return; }
    stopMusic();
    if (!isMusicEnabled) { return; }
    if (gs.isInGame) { playMod(levelMusic[gs.level], MUSIC_LOOP); }
    else          { playMod(menuMusic, MUSIC_LOOP); }
}

static void onSwitchOut()
{
    set_volume(0, 0);
    pauseMusic();
}

static void onSwitchIn()
{
    applyVolume();
    resumeMusic();
}

void toggleMusic()
{
    if (soundError || musicError) { return; }
    isMusicEnabled = 1 - isMusicEnabled;
    if (isMusicEnabled) {
        logWrite("Switched levelMusic on.");
        playMusic();
    } else {
        logWrite("Switched levelMusic off.");
        stopMusic();
    }
}

void toggleSound()
{
    if (soundError) { return; }
    isSoundEnabled = 1 - isSoundEnabled;
    if (!isSoundEnabled) {
        logWrite("Switched sounds off.");
        stopCollectSound();
        stopShootSound();
        stopExplodeSound();
        stopPlayerExplodeSound();
        stopEnemyShootSound();
        stopWonSound();
    } else {
        logWrite("Switched sounds on.");
    }
}

void stopVideoSound()
{
    if (soundError) { return; }
    stop_sample(videoSample);
}

void playVideoSound()
{
    if (soundError) { return; }
    stopVideoSound();
    if (isSoundEnabled) { play_sample(videoSample, MAX_VOLUME, PAN, DEFAULT_SPEED, SOUND_NO_LOOP); }
}

void stopShootSound()
{
    if (soundError) { return; }
    stop_sample(shootSamples[currentBullet]);
}

void playShootSound()
{
    if (soundError) { return; }
    stopShootSound();
    if (isSoundEnabled) { play_sample(shootSamples[currentBullet], MAX_VOLUME, PAN, DEFAULT_SPEED, SOUND_NO_LOOP); }
}

void stopCollectSound()
{
    if (soundError) { return; }
    stop_sample(collectSample);
}

void playCollectSound()
{
    if (soundError) { return; }
    stopCollectSound();
    if (isSoundEnabled) { play_sample(collectSample, MAX_VOLUME, PAN, DEFAULT_SPEED, SOUND_NO_LOOP); }
}

void stopEnemyShootSound()
{
    if (soundError || currentEnemyBullet < 0) { return; }
    stop_sample(enemyShootSamples[currentEnemyBullet]);
}

void playEnemyShootSound()
{
    if (soundError || currentEnemyBullet < 0) { return; }
    stopEnemyShootSound();
    if (isSoundEnabled) { play_sample(enemyShootSamples[currentEnemyBullet], MAX_VOLUME, PAN, DEFAULT_SPEED, SOUND_NO_LOOP); }
}

void stopExplodeSound()
{
    if (soundError) { return; }
    stop_sample(explodeSample);
}

void playExplodeSound()
{
    if (soundError) { return; }
    stopExplodeSound();
    if (isSoundEnabled) { play_sample(explodeSample, MAX_VOLUME, PAN, DEFAULT_SPEED, SOUND_NO_LOOP); }
}

void stopWonSound()
{
    if (soundError) { return; }
    stop_sample(wonSample);
}

void playWonSound()
{
    if (soundError) { return; }
    stopWonSound();
    if (isSoundEnabled) { play_sample(wonSample, MAX_VOLUME, PAN, DEFAULT_SPEED, SOUND_NO_LOOP); }
}

void stopPlayerExplodeSound()
{
    if (soundError) { return; }
    stop_sample(playerDieSample);
}

void playPlayerExplodeSound()
{
    if (soundError) { return; }
    stopPlayerExplodeSound();
    if (isSoundEnabled) { play_sample(playerDieSample, MAX_VOLUME, PAN, DEFAULT_SPEED, SOUND_NO_LOOP); }
}

#endif //_M_SOUND_
