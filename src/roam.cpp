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

#ifndef IS_ROAM_H

#include "roam.h"

int ly [2] [MAXX + 1];
int roamlevel = 17;
int fsplit = 0;
const int meminc = 400000;
RTriangleList *trilist; // will cast this to RTriangleList later on
RMatrix *trimatrix;

RTriangle::RTriangle ()
{
  xh = yh = x1 = x2 = y1 = y2 = 0;
  visibility = 0;
  setDirty ();
}

RTriangle::RTriangle (int level, int xh, int yh, int x1, int y1, int x2, int y2)
{
  this->xh = xh;
  this->x1 = x1;
  this->x2 = x2;
  this->yh = yh;
  this->y1 = y1;
  this->y2 = y2;
  setDirty ();
  setLevel (level);
}

bool RTriangle::isClean ()
{
  if (flags & 0x100) return true;
  return false;
}

void RTriangle::setClean ()
{
  flags |= 0x100;
}

void RTriangle::setDirty ()
{
  flags &= 0xFEFF;
}

int RTriangle::getLevel ()
{
  return flags & 0xFF;
}

void RTriangle::setLevel (int level)
{
  if (level > 127) level = 127;
  if (level < 0) level = 0;
  flags &= 0xFF00;
  flags |= level;
}

void RTriangle::split ()
{
  trimatrix->remove (this);
  int px1 = x1, py1 = y1;
  int xm = (x1 + x2) / 2, ym = (y1 + y2) / 2;
  int level = getLevel () + 1;
  if (level % 1)
  {
    x1 = xh; y1 = yh;
  }
  else
  {
    x1 = x2; y1 = y2;
    x2 = xh; y2 = yh;
  }
  xh = xm; yh = ym;
  setLevel (level);
  trimatrix->add (this);
  RTriangle *rt;
  if (level % 1)
    rt = trilist->addTriangle (level, xh, yh, px1, py1, x1, y1);
  else
    rt = trilist->addTriangle (level, xh, yh, x2, y2, px1, py1);
  trimatrix->add (rt);
  setDirty ();
}

void RTriangle::forceSplit (int d)
{
  int px1 = x1, py1 = y1, px2 = x2, py2 = y2;
  split ();
  RTriangle *t = trimatrix->getTriangle (px1, py1, px2, py2);
      fsplit ++;
  if (d > 5) printf (" %d ", d);
  if (t != NULL)
  {
    if ((t->xh == px1 && t->yh == py1) || (t->xh == px2 && t->yh == py2))
    {
      t->forceSplit (d + 1);
      t = trimatrix->getTriangle (px1, py1, px2, py2);
      t->forceSplit (d + 1);
    }
    else
    {
      t->forceSplit (d + 1);
    }
  }
}

void RTriangle::genBorders (int x1, int y1, int x2, int y2, int num)
{
  if (y2 < y1)
  {
    int temp = y1;
    y1 = y2;
    y2 = temp;
    temp = x1;
    x1 = x2;
    x2 = temp;
  }
  int dy = y2 - y1 + 1;
  int dx = x2 - x1 + 1;
  float m = (float) dx / dy;
  float pos = x1;
  for (int i = 0; i < dy; i ++)
  {
    ly [num] [y1 + i] = (int) pos;
    pos += m;
  }
}

bool RTriangle::trySplit (Landscape *l)
{
  int i, i2;
  int temp;
  if (getLevel () <= 7)
  {
    forceSplit (0);
    return true;
  }
  int px [3], py [3];
  GLLandscape *glland = (GLLandscape *) l;
  px [0] = x1; px [1] = x2; px [2] = xh;
  py [0] = y1; py [1] = y2; py [2] = yh;
  for (i = 1; i < 3; i ++)
    for (i2 = 0; i2 < i; i2 ++)
      if (py [i] < py [i2])
      {
        temp = px [i]; px [i] = px [i2]; px [i2] = temp;
        temp = py [i]; py [i] = py [i2]; py [i2] = temp;
      }
  if (px [1] <= px [0] && px [1] <= px [2])
  {
    genBorders (px [0], py [0], px [1], py [1], 0);
    genBorders (px [1], py [1], px [2], py [2], 0);
    genBorders (px [0], py [0], px [2], py [2], 1);
  }
  else
  {
    genBorders (px [0], py [0], px [1], py [1], 1);
    genBorders (px [1], py [1], px [2], py [2], 1);
    genBorders (px [0], py [0], px [2], py [2], 0);
  }
//    bool split = false;
  int ct = 150, lt = 500;

  int dx = abs (px[0]-px[1]);
  int dx2 = abs (px[2]-px[1]);
  int dx3 = abs (px[2]-px[0]);
  dx = dx2>dx?dx2:dx;
  dx = dx3>dx?dx3:dx;
  int dy = abs (py[0]-py[1]);
  int dy2 = abs (py[2]-py[1]);
  int dy3 = abs (py[2]-py[0]);
  dy = dy2>dy?dy2:dy;
  dy = dy3>dy?dy3:dy;
  if (dx <= 2 && dy <= 2)
    dy=dy;

  if (abs (glland->b[px[0]][py[0]]-glland->b[px[1]][py[1]]) > ct) { forceSplit (0); return true; }
  if (abs (glland->b[px[0]][py[0]]-glland->b[px[2]][py[2]]) > ct) { forceSplit (0); return true; }
  if (abs (glland->b[px[1]][py[1]]-glland->b[px[2]][py[2]]) > ct) { forceSplit (0); return true; }
  if (abs (glland->g[px[0]][py[0]]-glland->g[px[1]][py[1]]) > ct) { forceSplit (0); return true; }
  if (abs (glland->g[px[0]][py[0]]-glland->g[px[2]][py[2]]) > ct) { forceSplit (0); return true; }
  if (abs (glland->g[px[1]][py[1]]-glland->g[px[2]][py[2]]) > ct) { forceSplit (0); return true; }
  if (abs (glland->r[px[0]][py[0]]-glland->r[px[1]][py[1]]) > ct) { forceSplit (0); return true; }
  if (abs (glland->r[px[0]][py[0]]-glland->r[px[2]][py[2]]) > ct) { forceSplit (0); return true; }
  if (abs (glland->r[px[1]][py[1]]-glland->r[px[2]][py[2]]) > ct) { forceSplit (0); return true; }
  if (abs (glland->nl[px[0]][py[0]]-glland->nl[px[1]][py[1]]) > lt) { forceSplit (0); return true; }
  if (abs (glland->nl[px[0]][py[0]]-glland->nl[px[2]][py[2]]) > lt) { forceSplit (0); return true; }
  if (abs (glland->nl[px[1]][py[1]]-glland->nl[px[2]][py[2]]) > lt) { forceSplit (0); return true; }
  for (i = py [0]; i <= py [2]; i ++)
    for (i2 = ly [0] [i]; i2 <= ly [1] [i]; i2 ++)
    {
      float reqheight = (float) glland->h[px[0]][py[0]]*(px[2]-i2)*(py[2]-i) + glland->h[px[2]][py[0]]*(i2-px[0])*(py[2]-i) +
                                glland->h[px[0]][py[2]]*(px[2]-i2)*(i-py[0]) + glland->h[px[2]][py[2]]*(i2-px[0])*(i-py[0]);
      reqheight /= (float) (py[2]-py[0])*(px[2]-px[0]);
      if (fabs (glland->h[i2][i]-reqheight) > 250)
      {
        forceSplit (0); return true;
      }
    }
  // else
  setClean ();
  return false;
}

RTriangleList::RTriangleList ()
{
  aktlength = 0;
  maxlength = meminc;
  t = (RTriangle *) malloc (maxlength * sizeof (RTriangle));
  if (t == NULL)
  {
    printf ("Out of memory"); fflush (stdout);
    exit (1);
  }
}
void RTriangleList::reallocate ()
{
  if (aktlength < maxlength) return;
  printf ("Calloc"); fflush (stdout);
  maxlength += meminc;
  t = (RTriangle *) realloc (t, maxlength * sizeof (RTriangle));
}

RTriangle *RTriangleList::addTriangle (int level, unsigned short xh, unsigned short yh, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
  reallocate ();
  RTriangle *tr = &t [aktlength];
  tr->x1 = x1; tr->x2 = x2; tr->xh = xh;
  tr->y1 = y1; tr->y2 = y2; tr->yh = yh;
  tr->setLevel (level);
  tr->setDirty ();
  aktlength ++;
  return tr;
}

RMatrix::RMatrix ()
{
  t = (RTriangle ****) malloc ((MAXX + 1) * sizeof (RTriangle ***));
  for (int i = 0; i < MAXX + 1; i ++)
  {
    t [i] = (RTriangle ***) malloc ((MAXX + 1) * sizeof (RTriangle **));
    for (int i2 = 0; i2 < MAXX + 1; i2 ++)
      t [i] [i2] = NULL;
  }
  printf ("size=%d", sizeof (RTriangle *)); fflush (stdout);
  printf ("size=%d", sizeof (RTriangle)); fflush (stdout);
}

void RMatrix::allocate (int x, int y)
{
  t [y] [x] = (RTriangle **) malloc (8 * sizeof (RTriangle *));
  for (int i = 0; i < 8; i ++)
    t [y] [x] [i] = NULL;
}

void RMatrix::addVertex (RTriangle *rt, int x, int y)
{
  if (t [y] [x] == NULL)
    allocate (x, y);
  for (int i = 0; i < 8; i ++)
    if (t [y] [x] [i] == 0x0)
    {
      t [y] [x] [i] = rt;
      return;
    }
  printf ("Fehler!\n");
  fflush (stdout);
}

void RMatrix::add (RTriangle *rt)
{
  addVertex (rt, rt->x1, rt->y1);
  addVertex (rt, rt->x2, rt->y2);
  addVertex (rt, rt->xh, rt->yh);
}

void RMatrix::removeVertex (RTriangle *rt, int x, int y)
{
  for (int i = 0; i < 8; i ++)
    if (t [y] [x] [i] == rt)
    {
      t [y] [x] [i] = NULL;
      return;
    }
}

void RMatrix::remove (RTriangle *rt)
{
  removeVertex (rt, rt->x1, rt->y1);
  removeVertex (rt, rt->x2, rt->y2);
  removeVertex (rt, rt->xh, rt->yh);
}

RTriangle *RMatrix::getTriangle (unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
  RTriangle *rt;
  if (t [y1] [x1] == NULL)
    return NULL;
  for (int i = 0; i < 8; i ++)
  {
    rt = t [y1] [x1] [i];
    if (rt != NULL)
      if ((x1 == rt->x1 && y1 == rt->y1) ||
          (x1 == rt->x2 && y1 == rt->y2) ||
          (x1 == rt->xh && y1 == rt->yh))
        if ((x2 == rt->x1 && y2 == rt->y1) ||
            (x2 == rt->x2 && y2 == rt->y2) ||
            (x2 == rt->xh && y2 == rt->yh))
          return rt;
  }
  return NULL;
}


//  RTriangle ****trimatrix;
void RTerrain::roam (Landscape *l)
{
  int level = 1;
//    int i = 0;
  trimatrix = new RMatrix ();
  trilist = new RTriangleList ();
  RTriangle *rt;
  rt = trilist->addTriangle (1, 0, 0, MAXX, 0, 0, MAXX);
  trimatrix->add (rt);
  rt = trilist->addTriangle (1, MAXX, MAXX, 0, MAXX, MAXX, 0);
  trimatrix->add (rt);
  while (level <= roamlevel)
  {
    int n = trilist->aktlength;
    for (int i = 0; i < n; i ++)
//      if (!trilist->t [i].isClean ())
      if (trilist->t [i].getLevel () == level)
        if (trilist->t [i].trySplit (l))
        {
        }
    level ++;
  }
}

#endif