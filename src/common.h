#ifndef IS_COMMON_H
#define IS_COMMON_H

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
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#ifdef HAVE_SDL_MIXER
#include "SDL/SDL_mixer.h"
#endif
#ifdef HAVE_SDL_NET
#include "SDL/SDL_net.h"
#endif
#endif

#define ZOOM 256
#define VERSIONSTRING "GL-117 V. 0.8.1 (BETA) BY THOMAS A. DREXL"

// data types
#ifdef USE_GLUT
#define Uint32 unsigned int
#define Uint16 unsigned short
#endif

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
#define GAME_QUIT 12

// weather
#define WEATHER_SUNNY 0
#define WEATHER_THUNDERSTORM 1
#define WEATHER_CLOUDY 2

// controls
#define CONTROLS_KEYBOARD 0
#define CONTROLS_MOUSE 1
#define CONTROLS_JOYSTICK 2
#define CONTROLS_MOUSE_REVERSE 3
#define CONTROLS_MOUSE_EXP 4

// preferences
extern int quality;
extern float view;
extern int width, height, bpp;
extern int volumesound;
extern int volumemusic;
extern int dithering;

// mouse coordinates
extern int mousex, mousey;

extern int debug;
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

const int maxexplosion = 30, maxfighter = 30, maxlaser = 120, maxmissile = 30, maxstar = 70, maxgroundobj = 10, maxblacksmoke = 50, maxflare = 30, maxchaff = 30;

//#define SIN(x) sine[(int)x]
//#define COS(x) cosi[(int)x]

#define SIN(x) sin(x*PI/180.0)
#define COS(x) cos(x*PI/180.0)

#ifdef USE_GLUT
  #define MOUSE_BUTTON_LEFT GLUT_LEFT_BUTTON
  #define MOUSE_BUTTON_MIDDLE GLUT_MIDDLE_BUTTON
  #define MOUSE_BUTTON_RIGHT GLUT_RIGHT_BUTTON
  #define MOUSE_UP GLUT_UP
  #define MOUSE_DOWN GLUT_DOWN
  #define KEY_F1 GLUT_KEY_F1
  #define KEY_F2 GLUT_KEY_F2
  #define KEY_F3 GLUT_KEY_F3
  #define KEY_F4 GLUT_KEY_F4
  #define KEY_F5 GLUT_KEY_F5
  #define KEY_F6 GLUT_KEY_F6
  #define KEY_F7 GLUT_KEY_F7
  #define KEY_F8 GLUT_KEY_F8
  #define KEY_F9 GLUT_KEY_F9
  #define KEY_UP GLUT_KEY_UP
  #define KEY_DOWN GLUT_KEY_DOWN
  #define KEY_LEFT GLUT_KEY_LEFT
  #define KEY_RIGHT GLUT_KEY_RIGHT
  #define KEY_PGUP GLUT_KEY_PAGE_UP
  #define KEY_PGDOWN GLUT_KEY_PAGE_DOWN
#else
  #define MOUSE_BUTTON_LEFT SDL_BUTTON_LEFT
  #define MOUSE_BUTTON_MIDDLE SDL_BUTTON_MIDDLE
  #define MOUSE_BUTTON_RIGHT SDL_BUTTON_RIGHT
  #define MOUSE_UP SDL_RELEASED
  #define MOUSE_DOWN SDL_PRESSED
  #define KEY_F1 SDLK_F1
  #define KEY_F2 SDLK_F2
  #define KEY_F3 SDLK_F3
  #define KEY_F4 SDLK_F4
  #define KEY_F5 SDLK_F5
  #define KEY_F6 SDLK_F6
  #define KEY_F7 SDLK_F7
  #define KEY_F8 SDLK_F8
  #define KEY_F9 SDLK_F9
  #define KEY_UP SDLK_UP
  #define KEY_DOWN SDLK_DOWN
  #define KEY_LEFT SDLK_LEFT
  #define KEY_RIGHT SDLK_RIGHT
  #define KEY_PGUP SDLK_PAGEUP
  #define KEY_PGDOWN SDLK_PAGEDOWN
#endif

#endif
