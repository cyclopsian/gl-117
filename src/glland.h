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

/* This file includes a wrapper for land.cpp to display the landscape. */

#ifndef IS_GLLAND_H
#define IS_GLLAND_H

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

#define PARTS 12

#include <string.h>

#include "gl.h"
#include "model.h"
#include "aiobject.h"
#include "effects.h"
#include "land.h"
#include "roam.h"

// exact landscape type
#define LANDSCAPE_ALPINE 0
#define LANDSCAPE_ALPINE_NOLAKE 1
#define LANDSCAPE_LOW_ALPINE 2
#define LANDSCAPE_ALPINE_EROSION 5
#define LANDSCAPE_ALPINE_SEA 6
#define LANDSCAPE_SEA 10
#define LANDSCAPE_MOON 20
#define LANDSCAPE_FLAT_MOON 21
#define LANDSCAPE_CANYON 30
#define LANDSCAPE_DESERT 40

// textures are loaded in main.cpp
extern CTexture *texgrass, *texrocks, *texwater, *textree, *textree2, *textree3, *texcactus1, *texredstone, *textree4, *texearth, *texsand;

class GLLandscape : public Landscape
{
  public:
  short nl [MAXX + 1] [MAXX + 1]; // lighting mask
  float lv [3];
//  RTerrain *rt;
  unsigned char r [MAXX + 1] [MAXX + 1]; // red
  unsigned char g [MAXX + 1] [MAXX + 1]; // green
  unsigned char b [MAXX + 1] [MAXX + 1]; // blue
  unsigned char drawrule [MAXX] [MAXX]; // draw triangles or quads?
  unsigned char tex1 [MAXX] [MAXX]; // texture for quad or triangle1
  unsigned char tex2 [MAXX] [MAXX]; // texture for triangle2 if needed
  unsigned char dl [MAXX + 1] [MAXX + 1];
  int lsticker; // a timer increased every time draw() is called

  private:
  float hh, hh2, zoomz2;
  unsigned short hray [MAXX + 1] [MAXX + 1]; // height of lowest sunray
  float mat [15] [4], mata [15] [4]; // materials=colors of landscape IDs
  Space *space; // space of all objects
  CColor treecolor;

  void norm (float *c);
  void normalcrossproduct (float *a, float *b, float *c);

  float *selectMaterial (int x, int y);
  int selectColor (int x, int y);
  
  unsigned short lg [MAXX + 1] [MAXX + 1];
  void precalculate ();

  // obsolete, I used this to test quadtrees
  void subdivide (int x1, int y1, int x2, int y2);

  public:

  int getCoord (int a); // modulo MAXX
  float getCoord (float a);

  float getHeight (float x, float z); // height at (int) landscape point

  float getExactHeight (float x, float z); // linear interpolation

  float getExactLSHeight (float x, float z); // linear interpolation

  float getRayHeight (float x, float z); // height of lowest sun ray

  float getExactRayHeight (float x, float z); // linear interpolation

  void drawRotatedTree (float x, float y, float htree, float wtree, int phi);

  void drawTree (float x, float y, float htree, float wtree, int phi); // two in one

  int detail [PARTS] [PARTS]; // LOD
//  int done [100] [100];

  void drawCloudQuadStrip (int x1, int y1, int x2, int y2, int step); // not used

  void drawQuadStrip (int x1, int y1, int x2, int y2, int step);
  void drawTexturedQuad (int x, int y, int step);
  void drawWaterTexturedQuad (int x, int y, int step);
  void drawTexturedTriangle1 (int x, int y, int step);
  void drawTexturedTriangle2 (int x, int y, int step);
  
  void draw (int phi, int gamma); // draw ALL

  void calcDynamicLight (CExplosion **explo, DynamicObj **cannon, DynamicObj **missile); // explosions light up the surface

  GLLandscape (Space *space2, int type, int *heightmask);
};

extern GLLandscape *l;

#endif
