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

/* This file includes all objects instancing models. */

#ifndef IS_AIOBJECT_H

#include "aiobject.h"
#include "gl.h"

CSpaceObj::CSpaceObj ()
{ tl = new CVector3 ();
  rot = new CRotation ();
  zoom = 1; alpha = 1; draw = 1; lum = 1; drawlight = true; explode = 0; }

CSpaceObj::CSpaceObj (CModel *o, float zoom)
{ this->o = o;
  this->zoom = zoom;
  tl = new CVector3 ();
  rot = new CRotation ();
  alpha = 1; draw = 1; lum = 1; drawlight = true; explode = 0; }

CSpaceObj::~CSpaceObj () { delete tl; delete rot; }

void CSpaceObj::translate (CVector3 *v)
{ tl->x = v->x;
  tl->y = v->y;
  tl->z = v->z; }

void CSpaceObj::translate (float x, float y, float z)
{ tl->x = x;
  tl->y = y;
  tl->z = z; }

void CSpaceObj::rotate (short a, short b, short c)
{ rot->setAngles (a, b, c); }

void CSpaceObj::rotateOn (short a, short b, short c)
{ rot->addAngles (a, b, c); }

void CSpaceObj::drawGL (CVector3 *z1, CVector3 *z2, CVector3 *tl, float alpha2, float lum2, bool drawlight2, bool istextured2)
{
  if (draw == 0) return;
  if (draw == 2 || gl->isCubeInFrustum (tl->x + this->tl->x, tl->y + this->tl->y, tl->z + this->tl->z, this->zoom))
  {
    if (drawlight && drawlight2)
    {
      glEnable (GL_LIGHTING);
      o->draw (tl, this->tl, this->rot, this->zoom, lum * lum2, explode);
    }
    else
    {
      glDisable (GL_LIGHTING);
      if (istextured2)
        o->draw2 (tl, this->tl, this->rot, this->zoom, explode);
      else
      {
        if (drawlight2)
          o->draw3 (tl, this->tl, this->rot, this->zoom, lum * lum2, explode);
        else
          o->draw3 (tl, this->tl, this->rot, this->zoom, explode);
      }
    }
  }
}



Space::Space ()
{ no = 0; alpha = 1; lum = 1; drawlight = true;
  tl = new CVector3 ();
  z1 = new CVector3 ();
  z2 = new CVector3 (); }

Space::Space (CVector3 *v, CVector3 *w)
{ no = 0; alpha = 1; lum = 1; drawlight = true;
  tl = new CVector3 ();
  z1 = new CVector3 (v);
  z2 = new CVector3 (w); }

Space::~Space ()
{ delete tl; delete z1; delete z2; }

void Space::init ()
{
  no = 0;
}

bool Space::addObject (CSpaceObj *o2)
{
  if (no < 0 || no >= maxobjperspace) return false;
  o [no] = o2;
  no ++;
  return true;
}

bool Space::removeObject (CSpaceObj *o2)
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
}

void Space::translate (CVector3 *v)
{ tl->x = v->x;
  tl->y = v->y;
  tl->z = v->z; }

void Space::drawGL ()
{
  if (drawlight)
  {
    for (int i = 0; i < no; i ++)
    {
      if (&o [i] != NULL)
        if (o [i]->draw)
          o [i]->drawGL (z1, z2, tl, alpha, lum, true, true);
    }
  }
  else /* not yet implemented */
  {
    for (int i = 0; i < no; i ++)
    {
      if (&o [i] != NULL)
        if (o [i]->draw)
          o [i]->drawGL (z1, z2, tl, alpha, lum, false, false);
    }
  }
}

#endif
