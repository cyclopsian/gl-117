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

#ifndef IS_SPACE_H

#include "Model3d.h"



Space::Space ()
{
  no = 0;
  alpha = 1;
  lum = 1;
  drawLight = true;
}

/*Space::Space (Vector3 &v, Vector3 &w)
{
  no = 0;
  alpha = 1;
  lum = 1;
  drawLight = true;
}*/

Space::~Space ()
{
}

void Space::init ()
{
  no = 0;
}

void Space::addObject (SpaceObj *o)
{
  this->o.push_back (o);
  no ++;
}

/*bool Space::removeObject (SpaceObj *o2)
{
  int i, i2;
  for (i = 0; i < no; i ++)
    if (o [i] == o2)
    {
      for (i2 = i + 1; i2 < no; i2 ++)
      {
        o [i2 - 1] = o [i2];
      }
      no --;
      return true;
    }
  return false;
}*/

void Space::translate (Vector3 &v)
{
  tl.set (v);
}

void Space::drawGL ()
{
  int i;
  frustum.extractFrustum ();
  
  if (drawLight)
  {
    for (i = 0; i < no; i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o != NULL)
          if (o [i]->draw && !o [i]->o->alpha)
            o [i]->drawGL (tl, alpha, lum, true, true);
    }
    for (i = 0; i < no; i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o != NULL)
          if (o [i]->draw && o [i]->o->alpha)
            o [i]->drawGL (tl, alpha, lum, true, true);
    }
    // TODO: why the following code block???
/*    for (i = 0; i < no; i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o == NULL)
          if (o [i]->draw)
            o [i]->drawGL (tl, alpha, lum, true, true);
    }*/
  }
  else /* not yet implemented */
  {
    for (i = 0; i < no; i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o != NULL)
          if (o [i]->draw && !o [i]->o->alpha)
            o [i]->drawGL (tl, alpha, lum, false, false);
    }
    for (i = 0; i < no; i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o != NULL)
          if (o [i]->draw && o [i]->o->alpha)
            o [i]->drawGL (tl, alpha, lum, false, false);
    }
    for (i = 0; i < no; i ++)
    {
      if (o [i] != NULL)
        if (o [i]->o == NULL)
          if (o [i]->draw)
            o [i]->drawGL (tl, alpha, lum, false, false);
    }
  }
}

#endif
