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
#include "../config.h"
#define USE_GLUT
#ifdef HAVE_SDL
#undef USE_GLUT
#else
#undef HAVE_SDL_MIXER
#endif

#ifdef USE_GLUT
#include <GL/glut.h>
#else
#include <GL/glut.h>
/*#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>*/
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#ifdef HAVE_SDL_MIXER
#include "SDL/SDL_mixer.h"
#endif
//#include "SDL/SDL_audio.h"
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

#define ZOOM 256
#define VERSIONSTRING "GL-117 V. 0.6.5 (BETA) BY THOMAS A. DREXL"

// game states
#define GAME_INIT 0
#define GAME_PLAY 1
#define GAME_PAUSE 2
#define GAME_MENU 3
#define GAME_CREDITS 4
#define GAME_CREATE 5
#define GAME_JOIN 6
#define GAME_MISSION 7
#define GAME_STATS 8
#define GAME_FAME 9
#define GAME_FIGHTER 10
#define GAME_FINISH 11

// weather
#define WEATHER_SUNNY 0
#define WEATHER_THUNDERSTORM 1
#define WEATHER_CLOUDY 2

// controls
#define CONTROLS_KEYBOARD 0
#define CONTROLS_MOUSE 1
#define CONTROLS_JOYSTICK 2

// mouse coordinates
extern int mousex, mousey;

// preferences
extern int quality;
extern float view;
extern int width, height, bpp;
extern int volumesound;
extern int volumemusic;

extern bool debug;
extern bool multiplayer, isserver;
extern int fullscreen;
extern int day;
extern int weather; // 0: sunny, 1: thunderstorm, 2: no atmosphere (moon)
extern float sungamma; // angle of sunrays dropping on horizon

extern int camera;
extern float camx, camy, camz;
extern float camphi, camgamma, camtheta; // phi: x-z, theta: x-y, gamma: y-z
extern float sunlight;

extern int lighting; // 1=on, 0=off
extern int mode; // always off=0, 1 would mean to calculate ROAM!

extern float getView (); // return view dependant on weather

// maximum allocation values
//extern const int maxexplosion, maxfighter, maxlaser, maxmissile, maxstar, maxgroundobj;

extern int difficulty;

extern Server *server;
extern Client *client;

extern bool sunblinding;

const int maxexplosion = 30, maxfighter = 30, maxlaser = 120, maxmissile = 30, maxstar = 70, maxgroundobj = 10, maxblacksmoke = 50;
extern CTexture *texsun, *texflare1, *texflare2, *texflare3, *texflare4, *texfont1, *textfont2, *texmoon, *texcross, *texcross2, *texranks, *texmedals;

extern PilotList *pilots;

extern CExplosion *explosion [maxexplosion];
extern CBlackSmoke *blacksmoke [maxblacksmoke];
extern Font *font1, *font2;

#endif

