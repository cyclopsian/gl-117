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

/* This file includes simple objects instancing models. */

#ifndef IS_OBJECT_H
#define IS_OBJECT_H

#include "model.h"

// CSpaceObj represents an abstract object (like a cannon shot or a complete fighter)
class CSpaceObj
{
  public:
  int draw; // draw/hide
  int explode; // explosion stadium (0=no explosion)
  bool drawlight; // draw with/without light
  float zoom;
  float alpha;
  float lum; // luminance (default 1.0)
  CModel *o; // reference to a model
  CVector3 *tl; // translation
  CRotation *rot;
  CSpaceObj ();
  CSpaceObj (CModel *o, float zoom);
  ~CSpaceObj ();
  void translate (CVector3 *v);
  void translate (float x, float y, float z);
  void rotate (short a, short b, short c);
  void rotateOn (short a, short b, short c);
  // z1,z2=easy preclipping space/cube, tl=translation
  virtual void drawGL (CVector3 *z1, CVector3 *z2, CVector3 *tl, float alpha2, float lum2, bool drawlight2, bool istextured2);
};

const int maxobjperspace = 500;

// CSpace represents a collection of CSpaceObj_ects
class Space
{
  public:
  int no;
  bool drawlight; // draw with/without light
  float alpha;
  float lum; // luminance
  CSpaceObj *o [maxobjperspace]; // reference to all objects in space
  CVector3 *z1, *z2; // easy preclipping space/cube
  CVector3 *tl; // translation
  Space ();
  Space (CVector3 *v, CVector3 *w);
  ~Space ();
  void init ();
  bool addObject (CSpaceObj *o2);
  bool removeObject (CSpaceObj *o2);
  void translate (CVector3 *v);
  void drawGL ();
};

#endif
