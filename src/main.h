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

/* This file includes the main program. */

#ifndef IS_MAIN_H
#define IS_MAIN_H

//#define HAVE_SDL_MIXER
//#define USE_GLUT

#include <math.h>
#include <stdio.h>
#include <string.h>

/* Load definitions of GNU autoconf:
    USE_GLUT: no SDL
    HAVE_SDL: SDL installed
    HAVE_SDL_MIXER: SDL_mixer installed
    HAVE_SDL_NET: SDL_net installed */

#include "common.h"

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
#include "model.h"
#include "3ds.h"
#include "gl.h"
#include "land.h"
#include "audio.h"
#include "net.h"
#include "effects.h"
#include "mathtab.h"
#include "aiobject.h"
#include "roam.h"
#include "glland.h"
#include "pilots.h"

extern Server *server;
extern Client *client;

extern CTexture *texsun, *texflare1, *texflare2, *texflare3, *texflare4, *texfont1, *textfont2, *texmoon, *texcross, *texcross2, *texranks, *texmedals;
//extern CTexture *texglitter1;

extern PilotList *pilots;

extern CExplosion *explosion [maxexplosion];
extern CBlackSmoke *blacksmoke [maxblacksmoke];
extern Font *font1, *font2;

#endif

