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
//#include "GlLandscape.h"
#include "opengl/GlPrimitives.h"
#include "common.h"



HighClouds::HighClouds (int theta)
{
  init (theta);
}

HighClouds::~HighClouds ()
{
  delete sphere;
}

void HighClouds::init (int theta)
{
  sphere = new SpherePart (1, 9, theta);
  Object3d *co = sphere->object [0];
  co->hasTexture = true;
  co->material = new Material ();
  for (int i2 = 0; i2 < co->numVertices; i2 ++)
  {
    co->vertex [i2].tex.x = co->vertex [i2].vector.x * 5;
    co->vertex [i2].tex.y = co->vertex [i2].vector.y * 5;
  }
  co->hasTexture = true;
  sphere->displaylist = false;
  o = sphere;
  rot.b = 90;
  draw = 2;
  drawLight = false;
  zoom = 300;
}

void HighClouds::setTexture (Texture *texture)
{
  sphere->object [0]->material->texture = texture;
}

void HighClouds::drawGL (Vector3 &tl, Vector3 &textl)
{
  int j;
  Object3d *cm = o->object [0];
  for (int i2 = 0; i2 < cm->numVertices; i2 ++)
  {
    cm->vertex [i2].tex.x = cm->vertex [i2].vector.x * 4 + textl.x / zoom;
    cm->vertex [i2].tex.y = cm->vertex [i2].vector.y * 4 - textl.z / zoom;
  }

  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.015);
  glDisable (GL_DEPTH_TEST);

  glPushMatrix ();
  glTranslatef (tl.x, tl.y, tl.z);
  glRotatef (90, 0, -1, 0);
  glRotatef (270, 1, 0, 0);
  glRotatef (90, 0, 0, 1);
  glScalef (zoom, zoom, zoom);

  glShadeModel (GL_SMOOTH);

  glEnable (GL_TEXTURE_2D);
  glColor3ub (255, 255, 255);

  glBindTexture (GL_TEXTURE_2D, cm->material->texture->textureID);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBegin (GL_TRIANGLES);
  for (j = 0; j < cm->numTriangles; j++)
  {
    Vertex *v = cm->triangle [j].v [0];
    for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
    {
      v = cm->triangle [j].v [whichVertex];
      glNormal3f (v->normal.x, v->normal.y, v->normal.z);
      glTexCoord2f (v->tex.x, v->tex.y);
      glVertex3f(v->vector.x, v->vector.y, v->vector.z);
    }
  }
  glEnd();

  if (quality <= 2 || !antialiasing)
  {
    glBindTexture (GL_TEXTURE_2D, cm->material->texture->textureID);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }
  glBegin(GL_QUADS);
  for (j = 0; j < cm->numQuads; j++)
  {
    Vertex *v = cm->quad [j].v [0];
    for (int whichVertex = 0; whichVertex < 4; whichVertex ++)
    {
      v = cm->quad [j].v [whichVertex];
      glNormal3f (v->normal.x, v->normal.y, v->normal.z);
      glTexCoord2f(v->tex.x, v->tex.y);
      glVertex3f(v->vector.x, v->vector.y, v->vector.z);
    }
  }
  glEnd();

  glEnable (GL_DEPTH_TEST);
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_BLEND);
  glDisable (GL_TEXTURE_2D);
  glPopMatrix ();
}

#endif
