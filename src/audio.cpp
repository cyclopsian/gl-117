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

#include "audio.h"

WaveFile *wave = NULL;

#ifndef USE_GLUT
#ifndef HAVE_SDL_MIXER
void fillrepeat (void *unused, Uint8 *stream, int len)
{
  Uint8 *waveptr;
  int waveleft;

  waveptr = wave->sound + wave->soundpos;
  waveleft = wave->soundlen - wave->soundpos;
//  printf ("%p ", wave); fflush (stdout);

  while (waveleft <= len)
  {
    SDL_MixAudio (stream, waveptr, waveleft, SDL_MIX_MAXVOLUME);
    stream += waveleft;
    len -= waveleft;
    waveptr = wave->sound;
    waveleft = wave->soundlen;
    wave->soundpos = 0;
    SDL_PauseAudio (1);
    return;
  }
  SDL_MixAudio (stream, waveptr, len, SDL_MIX_MAXVOLUME);
  wave->soundpos += len;
}

void fillonce (void *unused, Uint8 *stream, int len)
{
  Uint8 *waveptr;
  int waveleft;

  waveleft = wave->soundlen - wave->soundpos;

  if (waveleft <= len)
  {
    if (SDL_GetAudioStatus () == SDL_AUDIO_PLAYING)
    {
      SDL_PauseAudio (0);
//      SDL_CloseAudio ();
    }
    return;
  }

  waveptr = wave->sound + wave->soundpos;

  SDL_MixAudio (stream, waveptr, len, SDL_MIX_MAXVOLUME);
  wave->soundpos += len;
}
#endif
#endif

WaveFile::WaveFile ()
{
#ifndef USE_GLUT
#ifndef HAVE_SDL_MIXER
  soundpos = 0;
#else
  channel = -1;
#endif
#endif
}

WaveFile::WaveFile (char *filename)
{
#ifndef USE_GLUT
#ifndef HAVE_SDL_MIXER
  soundpos = 0;
#else
  channel = -1;
#endif
  volume = 100;
  load (filename);
#endif
}

WaveFile::~WaveFile () {}

void WaveFile::load (char *filename)
{
#ifndef USE_GLUT
#ifndef HAVE_SDL_MIXER
  if (SDL_LoadWAV (filename, &spec, &sound, &soundlen) == NULL)
  {
    fprintf (stderr, "\nCouldn't load %s: %s", filename, SDL_GetError ());
    exit (1);
  }
  spec.callback = fillrepeat;
  wave = this;
#else
  chunk = Mix_LoadWAV (filename);
  if (chunk == NULL)
  {
    fprintf (stderr, "\nError: %s", Mix_GetError ());
    exit (1);
  }
#endif
#endif
}

void WaveFile::play ()
{
#ifndef USE_GLUT
#ifndef HAVE_SDL_MIXER
  SDL_PauseAudio (1);
/*  if (SDL_GetAudioStatus () == SDL_AUDIO_PLAYING)
  {
    SDL_PauseAudio (0);
  }
  SDL_CloseAudio ();
  if (SDL_OpenAudio (&spec, NULL) < 0)
  {
    fprintf (stderr, "\nCouldn't open audio: %s", SDL_GetError ());
    SDL_FreeWAV (sound);
    exit (2);
  }
//  wave = this;
  SDL_PauseAudio (0);*/
  if (wave != NULL)
  {
    wave->soundpos = 0;
  }
  wave = this;
  SDL_PauseAudio (0);
#else
  channel = Mix_PlayChannel (-1, chunk, 0);
#endif
#endif
}

void WaveFile::playLoop ()
{
#ifndef USE_GLUT
#ifndef HAVE_SDL_MIXER
  // No loop sounds on one channel
#else
  channel = Mix_PlayChannel (-1, chunk, -1);
#endif
#endif
}

void WaveFile::stop ()
{
#ifndef USE_GLUT
#ifndef HAVE_SDL_MIXER
  // No loop sounds on one channel
#else
  if (channel < 0) return;
  if (Mix_Playing (channel))
    Mix_HaltChannel (channel);
#endif
#endif
}

void WaveFile::setVolume (int level)
{
#ifndef USE_GLUT
#ifndef HAVE_SDL_MIXER
#else
  if (abs (volume - level) <= 5) return;
  volume = level;
  bool pause = false;
  if (channel >= 0)
  {
    if (Mix_Playing (channel))
    {
      Mix_Pause (channel);
      pause = true;
    }
  }
  Mix_VolumeChunk (chunk, level);
  if (pause && channel >= 0)
    Mix_Resume (channel);
#endif
#endif
}

SoundSystem::SoundSystem ()
{
  audio = false;
  sound = true;
  music = true;
  musicplaying = false;
  volumesound = 100;
  volumemusic = 100;
#ifndef USE_GLUT
#ifndef HAVE_SDL_MIXER
  waveclick1 = new WaveFile (dirs->getSounds ("click1.wav"));
  if (SDL_OpenAudio (&waveclick1->spec, NULL) < 0)
  {
    fprintf (stderr, "\nCouldn't open audio: %s, no sound available!", SDL_GetError ());
    audio = false;
    delete waveclick1;
    return;
  }
  else
  {
    audio = true;
  }
  delete waveclick1;
#else
  fprintf (stdout, "\nUsing SDL_mixer"); fflush (stdout);
  if (Mix_OpenAudio (22050, AUDIO_S16, 2, 4096))
  {
    fprintf (stderr, "\nUnable to open audio device!");
    audio = false;
    return;
  }
  else
  {
    audio = true;
  }
  music1 = Mix_LoadMUS (dirs->getMusic ("winner.s3m"));
  if (music1 == NULL)
  {
    fprintf (stderr, "\nCannot open winner.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (dirs->getMusic ("loser.s3m"));
  if (music1 == NULL)
  {
    fprintf (stderr, "\nCannot open loser.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (dirs->getMusic ("dark.s3m"));
  if (music1 == NULL)
  {
    fprintf (stderr, "\nCannot open dark.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (dirs->getMusic ("electro.s3m"));
  if (music1 == NULL)
  {
    fprintf (stderr, "\nCannot open electro.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (dirs->getMusic ("stars.s3m"));
  if (music1 == NULL)
  {
    fprintf (stderr, "\nCannot open stars.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
  Mix_FreeMusic (music1);
  music1 = Mix_LoadMUS (dirs->getMusic ("standby.s3m"));
  if (music1 == NULL)
  {
    fprintf (stderr, "\nCannot open standby.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
  playtime = 0;
#endif
  waveexplosion1 = new WaveFile (dirs->getSounds ("explode1.wav"));
  waveclick1 = new WaveFile (dirs->getSounds ("click1.wav"));
  wavecannon1 = new WaveFile (dirs->getSounds ("cannon1.wav"));
  wavemissile1 = new WaveFile (dirs->getSounds ("missile1.wav"));
  wavethunder1 = new WaveFile (dirs->getSounds ("thunder1.wav"));
  waveplane1 = new WaveFile (dirs->getSounds ("plane1.wav"));
  wavehit1 = new WaveFile (dirs->getSounds ("hit1.wav"));
  wavebeep1 = new WaveFile (dirs->getSounds ("beep1.wav"));
  wavechaff1 = new WaveFile (dirs->getSounds ("chaff1.wav"));
  wavecannon1->setVolume (50);
  wavemissile1->setVolume (110);
  waveclick1->setVolume (80);
#endif
};

SoundSystem::~SoundSystem ()
{
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  if (music1)
    Mix_FreeMusic (music1);
  Mix_CloseAudio ();
#else
#ifdef HAVE_SDL
  SDL_CloseAudio ();
#endif
#endif
  delete waveexplosion1;
  delete waveclick1;
  delete wavecannon1;
  delete wavemissile1;
  delete wavethunder1;
  delete waveplane1;
  delete wavehit1;
  delete wavebeep1;
  delete wavechaff1;
}

void SoundSystem::play (int sample)
{
  if (!audio) return;
  if (!sound) return;
  switch (sample)
  {
    case SOUND_CLICK1:
      waveclick1->play ();
      break;
    case SOUND_CANNON1:
      wavecannon1->play ();
      break;
    case SOUND_MISSILE1:
      wavemissile1->play ();
      break;
    case SOUND_EXPLOSION1:
      waveexplosion1->play ();
      break;
    case SOUND_THUNDER1:
      wavethunder1->play ();
      break;
    case SOUND_PLANE1:
      waveplane1->play ();
      break;
    case SOUND_HIT1:
      wavehit1->play ();
      break;
    case SOUND_BEEP1:
      wavebeep1->play ();
      break;
    case SOUND_CHAFF1:
      wavechaff1->play ();
      break;
  }
}

void SoundSystem::playLoop (int sample)
{
  if (!audio) return;
  if (!sound) return;
  switch (sample)
  {
    case SOUND_CLICK1:
      waveclick1->playLoop ();
      break;
    case SOUND_CANNON1:
      wavecannon1->playLoop ();
      break;
    case SOUND_MISSILE1:
      wavemissile1->playLoop ();
      break;
    case SOUND_EXPLOSION1:
      waveexplosion1->playLoop ();
      break;
    case SOUND_THUNDER1:
      wavethunder1->playLoop ();
      break;
    case SOUND_PLANE1:
      waveplane1->playLoop ();
      break;
    case SOUND_HIT1:
      wavehit1->playLoop ();
      break;
    case SOUND_BEEP1:
      wavebeep1->playLoop ();
      break;
    case SOUND_CHAFF1:
      wavechaff1->playLoop ();
      break;
  }
}

void SoundSystem::playMusic ()
{
  if (!audio) return;
  if (!music) return;
  if (volumemusic == 0) return;
#ifdef HAVE_SDL_MIXER
/*  int akttime = SDL_GetTicks ();
  if (playtime == 0)
  {
    playtime = akttime;
  }
  else
  {
    if (abs (akttime - playtime) < 1500) return;
    playtime = akttime;
  }*/
  musicplaying = true;
//  haltMusic ();
  Mix_PlayMusic (music1, -1);
/*  switch (sample)
  {
    case MUSIC_DARK1:
      Mix_PlayMusic (musicdark1, -1);
      break;
    case MUSIC_WINNER1:
      Mix_PlayMusic (musicwinner1, -1);
      break;
    case MUSIC_STANDBY1:
      Mix_PlayMusic (musicstandby1, -1);
      break;
    case MUSIC_ELECTRO1:
      Mix_PlayMusic (musicelectro1, -1);
      break;
  }*/
  setVolumeMusic ();
#endif
}

void SoundSystem::haltMusic ()
{
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  musicplaying = false;
  playtime = SDL_GetTicks ();
  Mix_HaltMusic ();
#endif
}

void SoundSystem::loadMusic (int sample)
{
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  if (music1)
    Mix_FreeMusic (music1);
  switch (sample)
  {
    case MUSIC_DARK1:
      music1 = Mix_LoadMUS (dirs->getMusic ("dark.s3m"));
      break;
    case MUSIC_WINNER1:
      music1 = Mix_LoadMUS (dirs->getMusic ("winner.s3m"));
      break;
    case MUSIC_LOSER1:
      music1 = Mix_LoadMUS (dirs->getMusic ("loser.s3m"));
      break;
    case MUSIC_STANDBY1:
      music1 = Mix_LoadMUS (dirs->getMusic ("standby.s3m"));
      break;
    case MUSIC_ELECTRO1:
      music1 = Mix_LoadMUS (dirs->getMusic ("electro.s3m"));
      break;
    case MUSIC_STARS1:
      music1 = Mix_LoadMUS (dirs->getMusic ("stars.s3m"));
      break;
  }
#endif
}

void SoundSystem::setVolume (int sample, int level)
{
  if (!audio) return;
  int ret = -1;
  level *= volumesound;
  level /= 100;
  switch (sample)
  {
    case SOUND_CLICK1:
      waveclick1->setVolume (level);
      break;
    case SOUND_CANNON1:
      wavecannon1->setVolume (level);
      break;
    case SOUND_MISSILE1:
      wavemissile1->setVolume (level);
      break;
    case SOUND_EXPLOSION1:
      waveexplosion1->setVolume (level);
      break;
    case SOUND_THUNDER1:
      wavethunder1->setVolume (level);
      break;
    case SOUND_PLANE1:
      waveplane1->setVolume (level);
      break;
    case SOUND_HIT1:
      wavehit1->setVolume (level);
      break;
    case SOUND_BEEP1:
      wavebeep1->setVolume (level);
      break;
    case SOUND_CHAFF1:
      wavechaff1->setVolume (level);
      break;
  }
}

void SoundSystem::setVolume ()
{
  if (!audio) return;
  int level = 128 * volumesound / 100;
  waveclick1->setVolume (level * 80 / 128);
  wavecannon1->setVolume (level * 50 / 128);
  wavemissile1->setVolume (level * 110 / 128);
  waveexplosion1->setVolume (level);
  wavethunder1->setVolume (level);
  waveplane1->setVolume (level);
  wavehit1->setVolume (level);
  wavebeep1->setVolume (level);
  wavechaff1->setVolume (level);
}

void SoundSystem::setVolumeMusic ()
{
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  int level = 128 * volumemusic / 100;
  Mix_VolumeMusic (level);
#endif
}

void SoundSystem::stop (int sample)
{
  if (!audio) return;
  switch (sample)
  {
    case SOUND_CLICK1:
      waveclick1->stop ();
      break;
    case SOUND_CANNON1:
      wavecannon1->stop ();
      break;
    case SOUND_MISSILE1:
      wavemissile1->stop ();
      break;
    case SOUND_EXPLOSION1:
      waveexplosion1->stop ();
      break;
    case SOUND_THUNDER1:
      wavethunder1->stop ();
      break;
    case SOUND_PLANE1:
      waveplane1->stop ();
      break;
    case SOUND_HIT1:
      wavehit1->stop ();
      break;
    case SOUND_BEEP1:
      wavebeep1->stop ();
      break;
    case SOUND_CHAFF1:
      wavechaff1->stop ();
      break;
  }
}

void SoundSystem::stopAll ()
{
  if (!audio) return;
#ifdef HAVE_SDL_MIXER
  Mix_HaltChannel (-1);
#endif
}

#endif
