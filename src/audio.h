/*
    GL-117
    Copyright 2001, 2002 Thomas A. Drexl aka heptargon

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

/* This file includes all SDL and SDL_mixer audio processing: sound and music */

#ifndef IS_AUDIO_H
#define IS_AUDIO_H

#include "common.h"

#include <stdlib.h>

#ifdef USE_GLUT
#include <GL/glut.h>
#else
#include <GL/glut.h>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#ifdef HAVE_SDL_MIXER
#include "SDL/SDL_mixer.h"
#endif
#endif

#include "dirs.h"

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
  ~WaveFile ();
  void load (char *filename);
  void play ();
  void playLoop ();
  void stop ();
  void setVolume (int level);
};

#define SOUND_EXPLOSION1 0
#define SOUND_CLICK1 1
#define SOUND_CANNON1 2
#define SOUND_MISSILE1 3
#define SOUND_THUNDER1 4
#define SOUND_PLANE1 5
#define SOUND_HIT1 6
#define MUSIC_DARK1 0
#define MUSIC_WINNER1 1
#define MUSIC_STANDBY1 2
#define MUSIC_ELECTRO1 3

class SoundSystem
{
  public:
  bool sound, music;
  WaveFile *waveexplosion1, *waveclick1, *wavecannon1, *wavemissile1;
  WaveFile *wavethunder1, *waveplane1, *wavehit1;
  bool musicplaying;
  int volumesound, volumemusic;
#ifdef HAVE_SDL_MIXER
  Mix_Music *musicstandby1, *musicwinner1, *musicdark1, *musicelectro1;
#endif
  SoundSystem ();
  ~SoundSystem ();
  void play (int sample);
  void playLoop (int sample);
  void stop (int sample);
  void stopAll ();
  void playMusic (int music);
  void haltMusic ();
  void setVolume (int sample, int level);
  void setVolume ();
  void setVolumeMusic ();
};

#endif
