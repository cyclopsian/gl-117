/*
    GL-117
    Copyright 2001-2004 Thomas A. Drexl aka heptargon

    This file is part of GL-117.

    GL-117 is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    GL-117 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GL-117; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef IS_SOUNDSYSTEM_H
#define IS_SOUNDSYSTEM_H

#include "opengl/includegl.h"
#include "configuration/Configuration.h"

/**
* This class loads and plays a Wavefile with SDL or SDL_mixer if available.
*/
class WaveFile
{
  public:

#ifndef USE_GLUT
 #ifndef HAVE_SDL_MIXER
    SDL_AudioSpec spec;
    Uint8 *sound;
    Uint32 soundlen;
    int soundpos;
 #else
    Mix_Chunk *chunk;
    int channel;
 #endif
#endif
    int volume;

    WaveFile ();
    WaveFile (char *filename);
    virtual ~WaveFile ();

    void load (char *filename);
    void play (int channel, bool loop);
    void stop ();
    void setVolume (int level);
};

#define SOUND_EXPLOSION1 1000
#define SOUND_CLICK1 1
#define SOUND_CANNON1 2
#define SOUND_MISSILE1 3
#define SOUND_THUNDER1 4
#define SOUND_PLANE1 5
#define SOUND_HIT1 6
#define SOUND_BEEP1 7
#define SOUND_CHAFF1 8
#define SOUND_BEEP2 9
#define MUSIC_DARK1 0
#define MUSIC_WINNER1 1
#define MUSIC_STANDBY1 2
#define MUSIC_ELECTRO1 3
#define MUSIC_STARS1 4
#define MUSIC_LOSER1 5
#define MUSIC_SOFTTEC1 6
#define MUSIC_AMBIENT1 7
#define MUSIC_COMPUTA2 8

/**
* This class manages all wave and tracker files for GL-117.
*/
class SoundSystem
{
  public:

    bool audio;
    bool sound, music;
    bool musicplaying;
    int volumesound, volumemusic;
    int engine;
#ifdef HAVE_SDL_MIXER
    int playtime;
    Mix_Music *music1;
#endif
    WaveFile *waveexplosion1, *waveclick1, *wavecannon1, *wavemissile1;
    WaveFile *wavethunder1, *waveplane [10], *wavehit1, *wavebeep1, *wavechaff1, *wavebeep2;

    SoundSystem ();
    virtual ~SoundSystem ();

    void play (int sample, bool loop);
    void playLoop (int sample);
    void setPosition (int sample, int angle, int dist);
    void stop (int sample);
    void stopAll ();
    void playMusic ();
    void playMusic (int times);
    void loadMusic (int music);
    void haltMusic ();
    void setVolume (int sample, int level);
    void setVolume ();
    void setVolumeMusic ();
};

#endif