#ifndef IS_COMMON_H
#define IS_COMMON_H

#include "../config.h"
#define USE_GLUT
#ifdef HAVE_SDL
#undef USE_GLUT
#else
#undef HAVE_SDL_MIXER
#endif

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

// preferences
extern int quality;
extern float view;
extern int width, height, bpp;
extern int volumesound;
extern int volumemusic;
extern int dithering;

// mouse coordinates
extern int mousex, mousey;

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

extern bool sunblinding;

const int maxexplosion = 30, maxfighter = 30, maxlaser = 120, maxmissile = 30, maxstar = 70, maxgroundobj = 10, maxblacksmoke = 50;

#endif
