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

#ifndef IS_MAIN_H
#define IS_MAIN_H

// dummy debug definitions
//#define HAVE_SDL_MIXER
//#define USE_GLUT

/* Load definitions of GNU autoconf:
    USE_GLUT: no SDL
    HAVE_SDL: SDL installed
    HAVE_SDL_MIXER: SDL_mixer installed
    HAVE_SDL_NET: SDL_net installed */

#include "aiobject/AiObj.h"
#include "model3d/Model3d.h"
#include "loadmodel/Load3ds.h"
#include "effects/Effects.h"
#include "sound/SoundSystem.h"
#include "configuration/Dirs.h"
#include "pilot/Pilot.h"

extern Font *font1, *font2;

extern Texture *texsun, *texflare1, *texflare2, *texflare3, *texflare4, *texmoon, *texcross, *texcross2, *texranks, *texmedals;
extern Texture *texradar1, *texradar2, *texarrow;

extern PilotList *pilots; // other pilots of the Eagle Squad

extern Explosion *explosion [maxexplosion];
extern BlackSmoke *blacksmoke [maxblacksmoke];

// all other models
extern DynamicObj *flare [maxflare];
extern DynamicObj *chaff [maxchaff];
extern AIObj *missile [maxmissile];
extern DynamicObj *laser [maxlaser];
extern AIObj *fighter [maxfighter];
extern Star *star [maxstar]; // the stars at night
extern DynamicObj *groundobj [maxgroundobj];
extern Flash *flash1; // the thunderstorm flash

extern Space *space; // the scene (everything) without the landscape

extern AIObj *fplayer; // the player itself

extern SoundSystem *sound;

// own export: main.h may be included in the cpp files
extern Model3d *getModel (int id);

#endif

