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
#include "opengl/GlPrimitives.h"



SpaceObj::SpaceObj ()
{
  zoom = 1;
  alpha = 1;
  draw = 1;
  lum = 1;
  drawLight = true;
  explode = 0;
  numRefModels = 0;
  o = NULL;
}

SpaceObj::SpaceObj (Model3d *o, float zoom)
{
  this->o = o;
  this->zoom = zoom;
  alpha = 1;
  draw = 1;
  lum = 1;
  drawLight = true;
  explode = 0;
  numRefModels = 0;
}

SpaceObj::~SpaceObj ()
{
}

void SpaceObj::addRefModel (Model3d *model, Vector3 &tl, Rotation &rot, float scale)
{
  refModel.push_back (model);
  refTl.push_back (Vector3 (tl));
  refRot.push_back (Rotation (rot));
  refScale.push_back (scale);
  numRefModels ++;
}

void SpaceObj::translate (Vector3 &v)
{
  tl.set (v);
}

void SpaceObj::translate (float x, float y, float z)
{
  tl.set (x, y, z);
}

void SpaceObj::rotate (short a, short b, short c)
{
  rot.set (a, b, c);
}

void SpaceObj::rotateOn (short a, short b, short c)
{
  rot.add (a, b, c);
}

void SpaceObj::drawGL (Vector3 &tl, float alpha2, float lum2, bool drawLight2, bool isTextured2)
{
  int i;
  Vector3 tl1;
  tl1.x = tl.x + this->tl.x;
  tl1.y = tl.y + this->tl.y;
  tl1.z = tl.z + this->tl.z;
  if (draw == 0) return;
  if (draw == 2 || frustum.isSphereInFrustum (tl1.x, tl1.y, tl1.z, this->zoom * 1.5))
  {
    if (drawLight && drawLight2)
    {
      glEnable (GL_LIGHTING);
      o->draw (tl, this->tl, this->rot, this->zoom, lum * lum2, explode);
      if (!refModel.empty ())
      {
        glPushMatrix ();
        glTranslatef (tl1.x, tl1.y - 0.001 * explode * explode / 35 / 35, tl1.z); // divide by timestep
        glRotatef (rot.c+90, 0, -1, 0);
        glRotatef (-rot.a+90, 0, 0, 1);
        glRotatef (rot.b+180, 1, 0, 0);
        glScalef (zoom, zoom, zoom);
        if (o->refpoint)
          for (i = 0; i < numRefModels; i ++)
            if (refScale [i] > 0.001)
              refModel [i]->draw (o->refpoint [i / 3], refTl [i], refRot [i], refScale [i], lum * lum2, explode);
        glPopMatrix ();
      }
    }
    else
    {
      glDisable (GL_LIGHTING);
      if (isTextured2)
      {
        o->drawNoLight (tl, this->tl, this->rot, this->zoom, explode);
      }
      else
      {
        if (drawLight2)
          o->drawNoTexture (tl, this->tl, this->rot, this->zoom, lum * lum2, explode);
        else
          o->drawNoTexture (tl, this->tl, this->rot, this->zoom, 1.0, explode);
      }
    }
  }
}

#endif
