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

/* This file includes the main program. */

#ifndef IS_MAIN_H
#define IS_MAIN_H

// dummy debug definitions
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

#include "aiobject.h" // ok
#include "model3d/Model3d.h" // ok
#include "loadmodel/Load3ds.h" // ok
#include "effects/Effects.h" // ok
#include "sound/SoundSystem.h" // ok
#include "configuration/Dirs.h" // ok
#include "pilot/Pilot.h" // ok

//extern Dirs *dirs; ///< paths to directories with game data or saves
extern Font *font1, *font2;

extern Texture *texsun, *texflare1, *texflare2, *texflare3, *texflare4, *texmoon, *texcross, *texcross2, *texranks, *texmedals;
extern Texture *texradar1, *texradar2, *texarrow;

extern PilotList *pilots; // other pilots of the Eagle Squad

extern Explosion *explosion [maxexplosion];
extern BlackSmoke *blacksmoke [maxblacksmoke];

//extern Load3ds g_Load3ds;
extern Model3d model_fig; // fighter models
extern Model3d model_figa;
extern Model3d model_figb;
extern Model3d model_figc;
extern Model3d model_figd;
extern Model3d model_fige;
extern Model3d model_figf;
extern Model3d model_figg;
extern Model3d model_figh;
extern Model3d model_figi;
extern Model3d model_figt;

extern Model3d model_cannon1;
extern Model3d model_cannon2;
extern Model3d model_cannon1b;
extern Model3d model_cannon2b;
extern Model3d model_flare1;
extern Model3d model_chaff1;
extern Model3d model_missile1;
extern Model3d model_missile2;
extern Model3d model_missile3;
extern Model3d model_missile4;
extern Model3d model_missile5;
extern Model3d model_missile6;
extern Model3d model_missile7;
extern Model3d model_missile8;
extern Model3d model_flak1;
extern Model3d model_flarak1;
extern Model3d model_tent1;
extern Model3d model_ship1;
extern Model3d model_ship2;
extern Model3d model_gl117;
extern Model3d model_tank1;
extern Model3d model_container1;
extern Model3d model_truck1;
extern Model3d model_truck2;
extern Model3d model_trsam;
extern Model3d model_pickup1;
extern Model3d model_pickup2;
extern Model3d model_tank2;
extern Model3d model_tent4;
extern Model3d model_hall1;
extern Model3d model_hall2;
extern Model3d model_oilrig;
extern Model3d model_egg;
extern Model3d model_radar;
extern Model3d model_mine1;
extern Model3d model_aster1;
extern Model3d model_base1;
extern Model3d model_barrier1;
extern Model3d model_rubble1;
extern Model3d model_depot1;
extern Model3d model_house1;

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

// get name of a keyboard's key (ENTER, SPACE)
//extern char *getKeyString (int key, char *str);

// own export: main.h may be included in the cpp files
extern Model3d *getModel (int id);

#endif

