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
    printf ("\nMixLoad %s", Mix_GetError ());
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
    fprintf (stderr, "\nCouldn't open audio: %s", SDL_GetError ());
    exit (2);
  }
  delete waveclick1;
#else
  printf ("\nUsing SDL_mixer"); fflush (stdout);
  if(Mix_OpenAudio(22050, AUDIO_S16, 2, 4096)) {
         printf("Unable to open audio!\n");
         exit(1);
       }
  musicstandby1 = Mix_LoadMUS (dirs->getMusic ("standby.s3m"));
  if (musicstandby1 == NULL)
  {
    fprintf (stderr, "\nCannot open standby.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
  musicwinner1 = Mix_LoadMUS (dirs->getMusic ("winner.s3m"));
  if (musicwinner1 == NULL)
  {
    fprintf (stderr, "\nCannot open winner.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
  musicdark1 = Mix_LoadMUS (dirs->getMusic ("dark.s3m"));
  if (musicdark1 == NULL)
  {
    fprintf (stderr, "\nCannot open dark.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
  musicelectro1 = Mix_LoadMUS (dirs->getMusic ("electro.s3m"));
  if (musicelectro1 == NULL)
  {
    fprintf (stderr, "\nCannot open electro.s3m: %s", Mix_GetError ()); fflush (stderr);
    exit (1);
  }
#endif
  waveexplosion1 = new WaveFile (dirs->getSounds ("explode1.wav"));
  waveclick1 = new WaveFile (dirs->getSounds ("click1.wav"));
  wavecannon1 = new WaveFile (dirs->getSounds ("cannon1.wav"));
  wavemissile1 = new WaveFile (dirs->getSounds ("missile1.wav"));
  wavethunder1 = new WaveFile (dirs->getSounds ("thunder1.wav"));
  waveplane1 = new WaveFile (dirs->getSounds ("plane1.wav"));
  wavehit1 = new WaveFile (dirs->getSounds ("hit1.wav"));
  wavecannon1->setVolume (50);
  wavemissile1->setVolume (110);
  waveclick1->setVolume (80);
#endif
};

SoundSystem::~SoundSystem ()
{
  delete waveexplosion1;
  delete waveclick1;
  delete wavecannon1;
  delete wavemissile1;
  delete wavethunder1;
  delete waveplane1;
  delete wavehit1;
}

void SoundSystem::play (int sample)
{
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
  }
}

void SoundSystem::playLoop (int sample)
{
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
  }
}

void SoundSystem::playMusic (int sample)
{
  if (!music) return;
  if (volumemusic == 0) return;
#ifdef HAVE_SDL_MIXER
  musicplaying = true;
//  haltMusic ();
  switch (sample)
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
  }
  setVolumeMusic ();
#endif
}

void SoundSystem::haltMusic ()
{
#ifdef HAVE_SDL_MIXER
  musicplaying = false;
  Mix_HaltMusic ();
#endif
}

void SoundSystem::setVolume (int sample, int level)
{
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
  }
}

void SoundSystem::setVolume ()
{
  int level = 128 * volumesound / 100;
  waveclick1->setVolume (level * 80 / 128);
  wavecannon1->setVolume (level * 50 / 128);
  wavemissile1->setVolume (level * 110 / 128);
  waveexplosion1->setVolume (level);
  wavethunder1->setVolume (level);
  waveplane1->setVolume (level);
  wavehit1->setVolume (level);
}

void SoundSystem::setVolumeMusic ()
{
#ifdef HAVE_SDL_MIXER
  int level = 128 * volumemusic / 100;
  Mix_VolumeMusic (level);
#endif
}

void SoundSystem::stop (int sample)
{
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
      waveplane1->stop ();
      break;
  }
}

void SoundSystem::stopAll ()
{
#ifdef HAVE_SDL_MIXER
  Mix_HaltChannel (-1);
#endif
}

#endif
