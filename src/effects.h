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

/* This file includes all special effects. */

#ifndef IS_EFFECTS_H
#define IS_EFFECTS_H

#include "common.h"

#ifdef USE_GLUT
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

#include "model.h"
#include "mathtab.h"
#include "object.h"

const int maxsmokeelem = 20;

extern CTexture *texsmoke, *texsmoke2, *texsmoke3;
extern const float smokezoom[];

// Smoke of missiles and fighters
class CSmoke
{
  public:
  CVector3 v [maxsmokeelem];
  float zoom [maxsmokeelem];
  int time [maxsmokeelem];
  int phi [maxsmokeelem]; // player's view angle (for low quality smoke)
  int last;
  int type;
  CSmoke (int type);
  void setSmoke (float x, float y, float z, int myphi, int mytime);
  void move ();
  void drawElem (int n);
  void drawElemHQ (int n);
  void draw ();
};

// Flash of a thunderstorm
class Flash
{
  public:
  int time;
  int phi; // player's view angle
  CVector3 w [10];
  Flash ();
  ~Flash ();
  void set (float x, float y, float z, int phi);
  void draw ();
  void drawHQ ();
};

// Explosion balls
class CExplosion : public CSpaceObj
{
  public:
  Space *space;
  int ttl, maxlen;
  float maxzoom;
  CExplosion (Space *space, CModel *sphere);
  void setExplosion (float x, float y, float z, float maxzoom, int len);
  void move ();
};

// Dark smoke after explosions
class CBlackSmoke : public CSpaceObj
{
  public:
  Space *space;
  int ttl, maxlen;
  float maxzoom;
  float myphi; // player's view angle
  CBlackSmoke (Space *space);
  void setBlackSmoke (float x, float y, float z, float myphi, float maxzoom, int len);
  void move ();
  virtual void drawGL (CVector3 *z1, CVector3 *z2, CVector3 *tl, float alpha2, float lum2, bool drawlight2, bool istextured2);
};

// Stars at night
class Star
{
  public:
  int phi, gamma;
  float size;
  Star (int phi, int gamma, float size);
  void draw ();
};

// Font class to create fonts out of bitmaps
class Font
{
  public:
  CTexture *texture;
  char start;
  int height;
  int n;
  short letterx [64];
  short lettery [64];
  short letterw [64];
  bool isPixel (int x, int y);
  void extractLetters (int height, char start);
  Font (char *filename, int height, char start);
  void drawText (float x, float y, float z, char *str, CColor *c, bool centered);
  void drawText (float x, float y, float z, char *str, CColor *c);
  void drawText (float x, float y, float z, char *str);
  void drawTextCentered (float x, float y, float z, char *str, CColor *c);
  void drawTextCentered (float x, float y, float z, char *str);
  void drawTextRotated (float x, float y, float z, char *str, CColor *color, int timer);
  void drawTextScaled (float x, float y, float z, char *str, CColor *color, int timer);
};

class HighClouds : public CSpaceObj
{
  public:
  CSpherePart *sphere;
  HighClouds (int theta)
  {
    init (theta);
  }
  ~HighClouds ()
  {
    delete sphere;
  }
  void init (int theta)
  {
    sphere = new CSpherePart (1, 9, theta);
    CObject *co = sphere->object [0];
    co->hasTexture = true;
    co->material = new CMaterial ();
    for (int i2 = 0; i2 < co->numVertices; i2 ++)
    {
      co->vertex [i2].tex.x = co->vertex [i2].vector.x * 5;
      co->vertex [i2].tex.y = co->vertex [i2].vector.y * 5;
    }
    co->hasTexture = true;
    sphere->displaylist = false;
    o = sphere;
    rot->b = 90;
    draw = 2;
    drawlight = false;
    zoom = 300;
  }
  void setTexture (CTexture *texture)
  {
    sphere->object [0]->material->texture = texture;
  }
  void drawGL (CVector3 *tl, CVector3 *textl)
  {
    int i = 0, j;
    CObject *cm = o->object [0];
    for (int i2 = 0; i2 < cm->numVertices; i2 ++)
    {
      cm->vertex [i2].tex.x = cm->vertex [i2].vector.x * 4 + textl->x / zoom;
      cm->vertex [i2].tex.y = cm->vertex [i2].vector.y * 4 - textl->z / zoom;
    }

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix ();
    glTranslatef (tl->x, tl->y, tl->z);
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
      CVertex *v = cm->triangle [j].v [0];
      for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
      {
        v = cm->triangle [j].v [whichVertex];
        glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        glTexCoord2f (v->tex.x, v->tex.y);
        glVertex3f(v->vector.x, v->vector.y, v->vector.z);
      }
    }
    glEnd();

    glBindTexture (GL_TEXTURE_2D, cm->material->texture->textureID);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBegin(GL_QUADS);
    for (j = 0; j < cm->numQuads; j++)
    {
      CVertex *v = cm->quad [j].v [0];
      for (int whichVertex = 0; whichVertex < 4; whichVertex ++)
      {
        v = cm->quad [j].v [whichVertex];
        glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        glTexCoord2f(v->tex.x, v->tex.y);
        glVertex3f(v->vector.x, v->vector.y, v->vector.z);
      }
    }
    glEnd();

    glDisable (GL_BLEND);
    glDisable (GL_TEXTURE_2D);
    glPopMatrix ();
  }
};

#endif
