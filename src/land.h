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

/* This file includes a random landscape generator. */

#ifndef IS_LAND_H
#define IS_LAND_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mathtab.h"

// surface extents
#define MAXX 512
#define MAXX_MASK 0x000001ff
#define MAXX_8 64
#define DIVISIONS 4
#define DIVX 128
#define DIVX_8 16
#define LOG2MAXX 9 // number of iterations
#define LOG2SPLINE 9 // number of iterations until spline interpolation
#define HEIGHT 120

// landscapes
#define LAND_ALPINE 0
#define LAND_MOON 1
#define LAND_CANYON 2
#define LAND_DESERT 3

// maximum Gaussian convolution kernel extent
#define MAXCORE 10

// static ID values for the landscape type (array "f")
#define GRASS 0
#define CONIFEROUSWOODS0 1
#define CONIFEROUSWOODS1 2
#define CONIFEROUSWOODS2 3
#define CONIFEROUSWOODS3 4
#define DECIDUOUSWOODS0 5
#define DECIDUOUSWOODS1 6
#define DECIDUOUSWOODS2 7
#define DECIDUOUSWOODS3 8
#define MIXEDWOODS0 9
#define MIXEDWOODS1 10
#define MIXEDWOODS2 11
#define MIXEDWOODS3 12
#define DWARFPINES0 13
#define DWARFPINES1 14
#define DWARFPINES2 15
#define DWARFPINES3 16
#define BUSHES0 17
#define BUSHES1 18
#define BUSHES2 19
#define BUSHES3 20
#define CACTUS0 25
#define ROCKS 30
#define ROCKS2 31
#define GLACIER 32
#define GREYSAND 33
#define GRAVEL 34
#define REDSTONE 35
#define REDSAND 36
#define REDTREE0 37
#define WATER 40
#define SHALLOWWATER 41
#define DEEPWATER 42
#define XSHALLOWWATER 43
#define XDEEPWATER 44
#define MOONSAND 50
#define DESERTSAND 60
#define TRYLAKE 100

class Landscape
{
  public:
  // height values without fluids
  unsigned short h [MAXX + 1] [MAXX + 1];
  // height values including fluids
  unsigned short hw [MAXX + 1] [MAXX + 1];
  // landscape type (ID values)
  unsigned char f [MAXX + 1] [MAXX + 1];
  // landscape type (0=alpine, 1=moon, 2=canyon)
  int type;

  private:
  unsigned char ftry [MAXX + 1] [MAXX + 1]; // dummy data field
  unsigned short hg [MAXX + 1] [MAXX + 1]; // dummy data field to apply convolution kernels
  int rockborder;
  int maxx; // same as MAXX, 512 is ideal
  int hoehe, maxn, n; // surface extents

  protected:
  int highestpoint, lowestpoint;

  public:
  int getCoord (int a); // modulo MAXX
  void convolveGauss (int radius, int hmin, int hmax); // fast convolution function (isotropic)
  void gauss (int x, int y); // convolution for only one raster point
  void smoothGlacier (); // special erosion function
  bool isType (unsigned char type, unsigned char id);
  bool isWoods (int type);
  bool isWater (int type);
  bool isGlacier (int type);
  void init (); // reset data fields
  void genSurface (int hoehepc, int *heightmap); // alpine
  void genErosionSurface (int hoehepc, int *heightmap); // alpine and erosion
  void genCanyonSurface (int hoehepc); // canyon
  void genMoonSurface (int height); // moon
  void genDesertSurface (int hoehepc); // desert
  void genRocks (int diffmin, int percent);
  int calcLake (int ys, int xs, unsigned short level, int num, int maxextent);
  void genLake (int depthpc);
  void calcWoods (int dy); // calculate woods on steep grass terrain
  bool isGround (int x, int y);
  bool riverCheck (int x, int y, int *fl, int z, int z2);
  void genRiver ();
  void searchPlain (int divx, int divy, int *x, int *y); // search a plain
  Landscape ();
  ~Landscape ();
};

#endif

