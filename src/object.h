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

#include "model.h" // ok

// CSpaceObj represents an abstract object (like a cannon shot or a complete fighter)
class CSpaceObj
{
  public:
  int draw; // draw/hide
  int explode; // explosion stadium (0=no explosion)
  bool drawlight; // draw with/without light
  float zoom; // zoom value of the model
  float alpha; // overall alpha value (should be 1.0)
  float lum; // luminance (default 1.0)
  Model3d *o; // reference to a model
  Vector3 *tl; // translation
  Rotation *rot; // rotation
  // reference models (e.g. missiles for fighters)
  int numRefModels;
  Model3d **refmodel;
  Vector3 *reftl;
  Rotation *refrot;
  float *refscale;

  CSpaceObj ();
  CSpaceObj (Model3d *o, float zoom);
  virtual ~CSpaceObj ();
  void addRefModel (Model3d *model, Vector3 *tl, Rotation *rot, float scale);
  void translate (Vector3 *v);
  void translate (float x, float y, float z);
  void rotate (short a, short b, short c);
  void rotateOn (short a, short b, short c);
  // z1,z2=easy preculling space/cube, tl=translation
  virtual void drawGL (Vector3 *z1, Vector3 *z2, Vector3 *tl, float alpha2, float lum2, bool drawlight2, bool istextured2);
};

const int maxobjperspace = 1024;

// CSpace represents a collection of CSpaceObj
class Space
{
  public:
  int no; // number of objects in space
  bool drawlight; // draw with/without light
  float alpha;
  float lum; // luminance
  CSpaceObj *o [maxobjperspace]; // reference to all objects in space
  Vector3 *z1, *z2; // easy preclipping space/cube
  Vector3 *tl; // translation
  
  Space ();
  Space (Vector3 *v, Vector3 *w);
  virtual ~Space ();
  void init ();
  bool addObject (CSpaceObj *o2);
  bool removeObject (CSpaceObj *o2);
  void translate (Vector3 *v);
  void drawGL ();
};

#endif
