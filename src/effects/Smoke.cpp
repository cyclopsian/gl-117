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

/* This file includes all special effects. */

#ifndef IS_EFFECTS_H

#include <math.h>
#include <stdlib.h>

#include "Effects.h"
#include "opengl/GlPrimitives.h"
#include "common.h"

Texture *texsmoke, *texsmoke2, *texsmoke3;

Smoke::Smoke (int type)
{
  for (int i = 0; i < MAXSMOKEELEM; i ++)
  {
    time [i] = 0;
    zoom [i] = 0.1;
    phi [i] = 0;
  }
  last = 0;
  this->type = type;
}

Smoke::~Smoke ()
{
}

void Smoke::setSmoke (float x, float y, float z, int myphi, int mytime)
{
  last ++;
  if (last >= MAXSMOKEELEM)
    last = 0;
  v [last].x = x;
  v [last].y = y;
  v [last].z = z;
  time [last] = mytime;
  phi [last] = myphi;
}

void Smoke::move (Uint32 dt, int dec)
{
  for (int i = 0; i < MAXSMOKEELEM; i ++)
    if (time [i] > 0)
    {
      time [i] -= dec;
      if (time [i] < 0) time [i] = 0;
    }
}

void Smoke::drawElem (int n)
{
  if (n < 0 || n >= MAXSMOKEELEM) return;
  float myzoom = math.smokezoom [time [n]];

  glPushMatrix ();
  glTranslatef (v [n].x, v [n].y, v [n].z);
  glRotatef (camphi, 0.0, 1.0, 0.0);
  glRotatef (-camgamma, 1.0, 0.0, 0.0);
  glScalef (myzoom, myzoom, myzoom);

  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, time [n] * 5 + 25);
  glTexCoord2f (0, 0);
  glVertex3f (1, 1, 0);
  glTexCoord2f (1, 0);
  glVertex3f (1, -1, 0);
  glTexCoord2f (1, 1);
  glVertex3f (-1, -1, 0);
  glTexCoord2f (0, 1);
  glVertex3f (-1, 1, 0);
  glEnd ();

  glPopMatrix ();
}

void Smoke::drawElemHQ (int n)
{
  if (n < 0 || n >= MAXSMOKEELEM) return;

  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, time [n] * 10 + 55);
  float myzoom = math.smokezoom [time [n]];
  glTexCoord2f (0, 0);
  glVertex3f (v [n].x - myzoom, v [n].y + myzoom, v [n].z);
  glTexCoord2f (1, 0);
  glVertex3f (v [n].x + myzoom, v [n].y + myzoom, v [n].z);
  glTexCoord2f (1, 1);
  glVertex3f (v [n].x + myzoom, v [n].y - myzoom, v [n].z);
  glTexCoord2f (0, 1);
  glVertex3f (v [n].x - myzoom, v [n].y - myzoom, v [n].z);
  glTexCoord2f (0, 0);
  glVertex3f (v [n].x, v [n].y + myzoom, v [n].z - myzoom);
  glTexCoord2f (1, 0);
  glVertex3f (v [n].x, v [n].y + myzoom, v [n].z + myzoom);
  glTexCoord2f (1, 1);
  glVertex3f (v [n].x, v [n].y - myzoom, v [n].z + myzoom);
  glTexCoord2f (0, 1);
  glVertex3f (v [n].x, v [n].y - myzoom, v [n].z - myzoom);
  glEnd ();
}

void Smoke::draw ()
{
  int i;
  Texture *smoketype;
  if (type == 0) smoketype = texsmoke;
  else if (type == 1) smoketype = texsmoke2;
  if (antialiasing)
    smoketype->shadeLinear ();
  else
    smoketype->shadeConst ();
  gl.enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.02);
  gl.enableTexture (smoketype->textureID);
  // draw smoke elements in the best order
  for (i = last; i >= 0; i --)
  {
    if (time [i] > 0)
    {
      drawElem (i);
    }
  }
  for (i = MAXSMOKEELEM - 1; i > last; i --)
  {
    if (time [i] > 0)
    {
      drawElem (i);
    }
  }
  glDisable (GL_TEXTURE_2D);
  glDisable (GL_ALPHA_TEST);
  gl.disableAlphaBlending ();
}

#endif
