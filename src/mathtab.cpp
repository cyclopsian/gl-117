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

/* This file includes a collection of functions and precalculated tabless. */

#ifndef IS_MATHTAB_H

#include "mathtab.h"

float PI;
float sine[360];
float cosi[360];

void mathtab_init ()
{
  int i;
  PI = (float) (atan (1.0) * 4.0);
  for (i = 0; i < 360; i ++)
  {
    sine [i] = (float) sin ((float) i / 180.0 * PI);
    cosi [i] = (float) cos ((float) i / 180.0 * PI);
  }
}

int myrandom (int n)
{
  if (n == 0) return 0;
  return rand () % n;
}

float dist (float dx, float dy)
{
  return (float) sqrt (dx*dx + dy*dy);
}

#endif
