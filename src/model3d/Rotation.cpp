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

#ifndef IS_MODEL3D_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Model3d.h"



Rotation::Rotation ()
{
  a = 0;
  b = 0;
  c = 0;
//  calculate ();
}

Rotation::Rotation (float a, float b, float c)
{
  this->a = a;
  this->b = b;
  this->c = c;
//  calculate ();
}

void Rotation::set (const Rotation &r)
{
  a = r.a;
  b = r.b;
  c = r.c;
}

void Rotation::set (float a, float b, float c)
{
  this->a = a;
  this->b = b;
  this->c = c;
}

void Rotation::add (const Rotation &r)
{
  a += r.a;
  b += r.b;
  c += r.c;
}

void Rotation::add (float a, float b, float c)
{
  this->a += a;
  this->b += b;
  this->c += c;
}

/*void Rotation::calculate ()
{
  rot [0] [0] = COS(c) * COS(b);
  rot [0] [1] = SIN(a) * SIN(b) * COS(c) - SIN(c) * COS(a);
  rot [0] [2] = SIN(a) * SIN(c) + COS(a) * SIN(b) * COS(c);
  rot [1] [0] = SIN(c) * COS(b);
  rot [1] [1] = COS(c) * COS(a) + SIN(a) * SIN(b) * SIN(c);
  rot [1] [2] = COS(a) * SIN(b) * SIN(c) - SIN(a) * COS(c);
  rot [2] [0] = -SIN(b);
  rot [2] [1] = SIN(a) * COS(b);
  rot [2] [2] = COS(a) * COS(b);
}

float Rotation::rotateX (const Vector3 &v) const
{
  return v.x * rot [0] [0] + v.y * rot [0] [1] + v.z * rot [0] [2];
}

float Rotation::rotateY (const Vector3 &v) const
{
  return v.x * rot [1] [0] + v.y * rot [1] [1] + v.z * rot [1] [2];
}

float Rotation::rotateZ (const Vector3 &v) const
{
  return v.x * rot [2] [0] + v.y * rot [2] [1] + v.z * rot [2] [2];
}*/

#endif

