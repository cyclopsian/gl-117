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

/* This file includes a real optimal adapting meshes algorithm. */

/* Look at http://tom.4president.de for an explanation of this algorithm! */

/* This algorithm is currently NOT used! */

#ifndef IS_ROAM_H
#define IS_ROAM_H

#include "land.h"
#include "glland.h"

class RTerrain
{
public:
  void roam (Landscape *l);
};

extern int ly [2] [MAXX + 1];

class RTriangle
{
public:
  unsigned short flags;
  unsigned short xh, yh, x1, y1, x2, y2;
  unsigned short visibility;
  RTriangle ();
  RTriangle (int level, int xh, int yh, int x1, int y1, int x2, int y2);
  bool isClean ();
  void setClean ();
  void setDirty ();
  int getLevel ();
  void setLevel (int level);
  void split ();
  void forceSplit (int d);
  void genBorders (int x1, int y1, int x2, int y2, int num);
  bool trySplit (Landscape *l);
};

class RMatrix
{
public:
  RTriangle ****t;
  RMatrix ();
  void allocate (int x, int y);
  void addVertex (RTriangle *t, int x, int y);
  void add (RTriangle *t);
  void removeVertex (RTriangle *t, int x, int y);
  void remove (RTriangle *t);
  RTriangle *getTriangle (unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
};

extern const int meminc;
extern int roamlevel; // 18
#define DIVS (MAXX / 16 + 1)
#define DIVFAC 16

class RTriangleList
{
public:
  RTriangle *t;
  int aktlength, maxlength;
  RTriangleList ();
  void reallocate ();
  RTriangle *addTriangle (int level, unsigned short xh, unsigned short yh, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
};

extern RTriangleList *trilist; // will cast this to RTriangleList later on
extern RMatrix *trimatrix;
extern int fsplit;

#endif
