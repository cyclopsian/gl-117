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

#include "effects.h"

#include "glland.h"

const float smokezoom[] = { 0.02, 0.027, 0.035, 0.042, 0.05, 0.058, 0.065, 0.075, 0.085, 0.092, 0.1, 0.105, 0.11, 0.115, 0.12, 0.11, 0.1, 0.09, 0.08, 0.06, 0.03 };
CTexture *texsmoke, *texsmoke2, *texsmoke3;

CSmoke::CSmoke (int type)
{
  for (int i = 0; i < maxsmokeelem; i ++)
  {
    time [i] = 0;
    zoom [i] = 0.1;
    phi [i] = 0;
  }
  last = 0;
  this->type = type;
}

void CSmoke::setSmoke (float x, float y, float z, int myphi, int mytime)
{
  last ++;
  if (last >= maxsmokeelem)
    last = 0;
  v [last].x = x;
  v [last].y = y;
  v [last].z = z;
  time [last] = mytime;
  phi [last] = myphi;
}

void CSmoke::move ()
{
  for (int i = 0; i < maxsmokeelem; i ++)
    if (time [i] > 0)
    {
      time [i] -= 4;
      if (time [i] < 0) time [i] = 0;
    }
}

void CSmoke::drawElem (int n)
{
//    glRotatef (phi [n], 0, 1, 0);
  if (n < 0 || n >= maxsmokeelem) return;
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, time [n] * 10 + 55);
  float myzoom = smokezoom [time [n]];
  glTexCoord2f (0, 0);
  glVertex3f (v [n].x - myzoom * cosi [phi [n]], v [n].y + myzoom, v [n].z + myzoom * sine [phi [n]]);
  glTexCoord2f (1, 0);
  glVertex3f (v [n].x + myzoom * cosi [phi [n]], v [n].y + myzoom, v [n].z - myzoom * sine [phi [n]]);
  glTexCoord2f (1, 1);
  glVertex3f (v [n].x + myzoom * cosi [phi [n]], v [n].y - myzoom, v [n].z - myzoom * sine [phi [n]]);
  glTexCoord2f (0, 1);
  glVertex3f (v [n].x - myzoom * cosi [phi [n]], v [n].y - myzoom, v [n].z + myzoom * sine [phi [n]]);
  glEnd ();
}

void CSmoke::drawElemHQ (int n)
{
//    glRotatef (phi [n], 0, 1, 0);
  if (n < 0 || n >= maxsmokeelem) return;
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 255, time [n] * 10 + 55);
  float myzoom = smokezoom [time [n]];
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

void CSmoke::draw ()
{
  int i;
  int smoketype;
  if (type == 0) smoketype = texsmoke->textureID;
  else if (type == 1) smoketype = texsmoke2->textureID;
  if (quality >= 3)
    gl->enableLinearTexture (smoketype);
  else
    gl->disableLinearTexture (smoketype);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.02);
  gl->enableTextures (smoketype);
  for (i = last; i >= 0; i --)
  {
    if (time [i] > 0)
    {
/*      if (quality >= 3)
        drawElemHQ (i);
      else*/
        drawElem (i);
    }
  }
  for (i = maxsmokeelem - 1; i > last; i --)
  {
    if (time [i] > 0)
    {
/*      if (quality >= 3)
        drawElemHQ (i);
      else*/
        drawElem (i);
    }
  }
  glDisable (GL_TEXTURE_2D);
  glDisable (GL_ALPHA_TEST);
  gl->disableAlphaBlending ();
}



Flash::Flash ()
{
  phi = 0;
}

Flash::~Flash ()
{
}

void Flash::set (float x, float y, float z, int phi)
{
  int i;
  this->phi = phi;
  w [0].x = x; w [0].y = y; w [0].z = z;
  for (i = 1; i < 10; i ++)
  {
    w [i].y = y + 6.0 * i;
    w [i].x = w [i - 1].x + 0.1 * myrandom (60) - 3.0;
    w [i].z = w [i - 1].z + 0.1 * myrandom (60) - 3.0;
  }
}

void Flash::draw ()
{
  int i;
  glDisable (GL_TEXTURE_2D);
  glBegin (GL_QUAD_STRIP);
  glColor3ub (255, 255, 255);
  float myzoom = 0.3;
  for (i = 0; i < 10; i ++)
  {
    glVertex3f (w [i].x - myzoom * cosi [phi], w [i].y, w [i].z + myzoom * sine [phi]);
    glVertex3f (w [i].x + myzoom * cosi [phi], w [i].y, w [i].z - myzoom * sine [phi]);
  }
  glEnd ();
}

void Flash::drawHQ ()
{
  int i;
  glDisable (GL_TEXTURE_2D);
  gl->enableAlphaBlending ();
  glBegin (GL_QUADS);
  float myzoom = 0.4;
  float alphawidth = 0.7;
  for (i = 0; i < 9; i ++)
  {
    glColor4ub (255, 255, 255, 0);
    glVertex3f (w [i].x - myzoom * cosi [phi], w [i].y, w [i].z + myzoom * sine [phi]);
    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i].x - myzoom * alphawidth * cosi [phi], w [i].y, w [i].z + myzoom * alphawidth * sine [phi]);
    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i + 1].x - myzoom * alphawidth * cosi [phi], w [i + 1].y, w [i + 1].z + myzoom * alphawidth * sine [phi]);
    glColor4ub (255, 255, 255, 0);
    glVertex3f (w [i + 1].x - myzoom * cosi [phi], w [i + 1].y, w [i + 1].z + myzoom * sine [phi]);

    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i].x - myzoom * alphawidth * cosi [phi], w [i].y, w [i].z + myzoom * alphawidth * sine [phi]);
    glVertex3f (w [i].x + myzoom * alphawidth * cosi [phi], w [i].y, w [i].z - myzoom * alphawidth * sine [phi]);
    glVertex3f (w [i + 1].x + myzoom * alphawidth * cosi [phi], w [i + 1].y, w [i + 1].z - myzoom * alphawidth * sine [phi]);
    glVertex3f (w [i + 1].x - myzoom * alphawidth * cosi [phi], w [i + 1].y, w [i + 1].z + myzoom * alphawidth * sine [phi]);

    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i].x + myzoom * alphawidth * cosi [phi], w [i].y, w [i].z - myzoom * alphawidth * sine [phi]);
    glColor4ub (255, 255, 255, 0);
    glVertex3f (w [i].x + myzoom * cosi [phi], w [i].y, w [i].z - myzoom * sine [phi]);
    glColor4ub (255, 255, 255, 0);
    glVertex3f (w [i + 1].x + myzoom * cosi [phi], w [i + 1].y, w [i + 1].z - myzoom * sine [phi]);
    glColor4ub (255, 255, 255, 255);
    glVertex3f (w [i + 1].x + myzoom * alphawidth * cosi [phi], w [i + 1].y, w [i + 1].z - myzoom * alphawidth * sine [phi]);
  }
  glEnd ();
  gl->disableAlphaBlending ();
}



CExplosion::CExplosion (Space *space, CModel *sphere)
{
  this->space = space;
  o = sphere;
  ttl = 0;
  space->addObject (this);
  draw = false;
  zoom = 0.1;
  drawlight = false;
}

void CExplosion::setExplosion (float x, float y, float z, float maxzoom, int len)
{
  tl->x = x;
  tl->y = y;
  tl->z = z;
  this->maxzoom = maxzoom;
  ttl = len;
  maxlen = len;
  draw = true;
}

void CExplosion::move ()
{
  if (ttl > 0)
  {
    zoom = sine [ttl * 180 / maxlen] * maxzoom;
    ttl --;
    tl->y += 0.01;
    if (ttl <= 0)
    {
      ttl = 0;
      draw = false;
    }
  }
}



CBlackSmoke::CBlackSmoke (Space *space)
{
  this->space = space;
  ttl = 0;
  space->addObject (this);
  draw = false;
  zoom = 0.1;
  drawlight = false;
}

void CBlackSmoke::setBlackSmoke (float x, float y, float z, float myphi, float maxzoom, int len)
{
  this->myphi = myphi;
  tl->x = x;
  tl->y = y;
  tl->z = z;
  this->maxzoom = maxzoom;
  ttl = len;
  maxlen = len;
  draw = true;
  zoom = 0;
}

void CBlackSmoke::move ()
{
  if (ttl > 0)
  {
    zoom += maxzoom / ttl;
    tl->y += 0.04;
    ttl --;
    if (ttl <= 0)
    {
      ttl = 0;
      draw = false;
    }
  }
}

void CBlackSmoke::drawGL (CVector3 *z1, CVector3 *z2, CVector3 *tl, float alpha2, float lum2, bool drawlight2, bool istextured2)
{
  if (ttl <= 0 || quality == 0) return;
  if (draw == 2 || gl->isSphereInFrustum (tl->x + this->tl->x, tl->y + this->tl->y, tl->z + this->tl->z, this->zoom))
  {
    glDepthMask (GL_FALSE);
    if (quality >= 3)
      gl->enableLinearTexture (texsmoke3->textureID);
    else
      gl->disableLinearTexture (texsmoke3->textureID);
    gl->enableAlphaBlending ();
    glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GEQUAL, 0.02);
    gl->enableTextures (texsmoke3->textureID);
    glBegin (GL_QUADS);
    int myalpha = 255 - (100 - ttl * 5);
    if (myalpha > 255) myalpha = 255;
    glColor4ub (0, 0, 0, myalpha);
    float myzoom = zoom;
    glTexCoord2f (0, 0);
    glVertex3f (this->tl->x - myzoom * cosi [(int) myphi], this->tl->y + myzoom, this->tl->z + myzoom * sine [(int) myphi]);
    glTexCoord2f (1, 0);
    glVertex3f (this->tl->x + myzoom * cosi [(int) myphi], this->tl->y + myzoom, this->tl->z - myzoom * sine [(int) myphi]);
    glTexCoord2f (1, 1);
    glVertex3f (this->tl->x + myzoom * cosi [(int) myphi], this->tl->y - myzoom, this->tl->z - myzoom * sine [(int) myphi]);
    glTexCoord2f (0, 1);
    glVertex3f (this->tl->x - myzoom * cosi [(int) myphi], this->tl->y - myzoom, this->tl->z + myzoom * sine [(int) myphi]);
    glEnd ();
    glDisable (GL_TEXTURE_2D);
    glDisable (GL_ALPHA_TEST);
    gl->disableAlphaBlending ();
    glDepthMask (GL_TRUE);
  }
}



Star::Star (int phi, int gamma, float size)
{
  this->phi = phi;
  this->gamma = gamma;
  this->size = size;
}

void Star::draw ()
{
  glPointSize (size);
//    gl->enableAntiAliasing ();
  glBegin (GL_POINTS);
  glColor3ub (255, 255, 255);
  glVertex3f (0, 0, 0);
  glEnd ();
//    gl->disableAntiAliasing ();
}



bool Font::isPixel (int x, int y)
{
  y = texture->height - y;
  int index = (y * texture->width + x) * 4;
  if (texture->data [index] > 50 && texture->data [index + 1] > 50 && texture->data [index + 2] > 50)
    return true;
  return false;
}

void Font::extractLetters (int height, char start)
{
  int i;
  int mode = 0;
  bool modeon = 0;
  this->start = start;
  this->height = height;
  int x = 0, y = 0;
  int xs, xe;
  n = 0;

  for (;;)
  {
    mode ++;
    for (i = y + 2; i <= y + height - 2; i += 2)
    {
      if (isPixel (x, i))
      {
        mode = 0;
      }
    }
    if (mode == 0)
    {
      if (!modeon)
      {
        xs = x;
        modeon = true;
      }
    }
    if (mode >= 5)
    {
      if (modeon)
      {
        xe = x;
        modeon = false;
        letterx [n] = xs - 2;
        lettery [n] = y;
        letterw [n] = xe - xs + 1;
        n ++;
        if (n >= 64) return;
      }
    }
    x ++;
    if (x >= texture->width)
    {
      if (modeon)
      {
        xe = x;
        modeon = false;
        letterx [n] = xs - 2;
        lettery [n] = y;
        letterw [n] = xe - xs + 3;
        n ++;
        if (n >= 64) return;
      }
      y += height;
      x = 0;
    }
    if (y >= texture->height) return;
  }
}

Font::Font (char *filename, int height, char start)
{
  texture = gl->genTextureTGA (filename, 0, 1, 0);
  extractLetters (height, start);
}

void Font::drawText (float x, float y, float z, char *str, CColor *c, bool centered)
{
  int len = strlen (str);
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  gl->enableTextures (texture->textureID);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  glBegin (GL_QUADS);
  glColor4ub (c->c [0], c->c [1], c->c [2], c->c [3]);
  float xz = x * 0.1, yz = y * 0.1;

  if (centered)
  {
    float xw = 0;
    for (int i = 0; i < len; i ++)
    {
      if (str [i] >= start && str [i] <= start + n)
      {
        int c = (int) (str [i] - start);
        xw += 0.1 * letterw [c] / height;
      }
      else if (str [i] == '\t')
      {
        int xzint = (int) (xw * 10.0);
        xzint -= xzint & 3; // modulo 4
        xzint += 4;
        xw = (float) xzint / 10.0;
      }
      else
      {
        xw += 0.05;
      }
    }
    xz -= xw / 2;
  }

  for (int i = 0; i < len; i ++)
  {
    if (str [i] >= start && str [i] <= start + n)
    {
      int c = (int) (str [i] - start);
      float tx = (float) letterx [c] / texture->width;
      float ty = 1.0 - (float) lettery [c] / texture->height;
      float tx2 = (float) tx + (float) letterw [c] / texture->width;
      float ty2 = (float) ty - (float) height / texture->height;
      float xi = 0.1 * letterw [c] / height;
      float yi = 0.1;
      glTexCoord2f (tx, ty2);
      glVertex3f (xz, yz, z);
      glTexCoord2f (tx, ty);
      glVertex3f (xz, yz + yi, z);
      glTexCoord2f (tx2, ty);
      glVertex3f (xz + xi, yz + yi, z);
      glTexCoord2f (tx2, ty2);
      glVertex3f (xz + xi, yz, z);
      xz += xi;
    }
    else if (str [i] == '\t')
    {
      int xzint = (int) (xz * 10.0);
      xzint -= xzint & 3; // modulo 4
      xzint += 4;
      xz = (float) xzint / 10.0;
    }
    else if (str [i] == '\n')
    {
      yz -= 0.1;
      xz = x * 0.1;
    }
    else
    {
      xz += 0.05;
    }
  }
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  gl->disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void Font::drawText (float x, float y, float z, char *str, CColor *c)
{
  drawText (x, y, z, str, c, false);
}

void Font::drawText (float x, float y, float z, char *str)
{
  CColor stdcol (255, 255, 255, 220);
  drawText (x, y, z, str, &stdcol, false);
}

void Font::drawTextCentered (float x, float y, float z, char *str, CColor *c)
{
  drawText (x, y, z, str, c, true);
}

void Font::drawTextCentered (float x, float y, float z, char *str)
{
  CColor stdcol (255, 255, 255, 150);
  drawText (x, y, z, str, &stdcol, true);
}

void Font::drawTextRotated (float x, float y, float z, char *str, CColor *color, int timer)
{
  int len = strlen (str);
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  gl->enableTextures (texture->textureID);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  float xz = x * 0.1, yz = y * 0.1;
  for (int i = 0; i < len; i ++)
  {
    if (str [i] >= start && str [i] <= start + n)
    {
      int c = (int) (str [i] - start);
      float tx = (float) letterx [c] / texture->width;
      float ty = 1.0 - (float) lettery [c] / texture->height;
      float tx2 = (float) tx + (float) letterw [c] / texture->width;
      float ty2 = (float) ty - (float) height / texture->height;
      float xi = 0.1 * letterw [c] / height;
      float yi = 0.1;
      glPushMatrix ();
      glTranslatef (xz + xi / 2, yz + yi / 2, z);
      glRotatef (timer + i * 20, 1, 0, 0);
      glBegin (GL_QUADS);
      glColor4ub (color->c [0], color->c [1], color->c [2], color->c [3]);
      glTexCoord2f (tx, ty2);
      glVertex3f (-xi / 2, -yi / 2, 0);
      glTexCoord2f (tx, ty);
      glVertex3f (-xi / 2, yi / 2, 0);
      glTexCoord2f (tx2, ty);
      glVertex3f (xi / 2, yi / 2, 0);
      glTexCoord2f (tx2, ty2);
      glVertex3f (xi / 2, -yi / 2, 0);
      glEnd ();
      glPopMatrix ();
      xz += xi;
    }
    else if (str [i] == '\t')
    {
      int xzint = (int) (xz * 10.0);
      xzint -= xzint & 3; // modulo 4
      xzint += 4;
      xz = (float) xzint / 10.0;
    }
    else
    {
      xz += 0.05;
    }
  }
  glDisable (GL_ALPHA_TEST);
  gl->disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

void Font::drawTextScaled (float x, float y, float z, char *str, CColor *color, int timer)
{
  int i;
  int len = strlen (str);
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  gl->enableTextures (texture->textureID);
  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);

  float xw = 0;
  for (i = 0; i < len; i ++)
  {
    if (str [i] >= start && str [i] <= start + n)
    {
      int c = (int) (str [i] - start);
      xw += 0.1 * letterw [c] / height;
    }
    else if (str [i] == '\t')
    {
      int xzint = (int) (xw * 10.0);
      xzint -= xzint & 3; // modulo 4
      xzint += 4;
      xw = (float) xzint / 10.0;
    }
    else
    {
      xw += 0.05;
    }
  }

  float xz = x * 0.1, yz = y * 0.1;
  glPushMatrix ();
  glTranslatef (xz + xw / 2, yz, z);
  xz = 0;
  glScalef (1.0 + 0.08 * sine [abs (timer * 8 % 360)], 1, 1);
  for (i = 0; i < len; i ++)
  {
    if (str [i] >= start && str [i] <= start + n)
    {
      int c = (int) (str [i] - start);
      float tx = (float) letterx [c] / texture->width;
      float ty = 1.0 - (float) lettery [c] / texture->height;
      float tx2 = (float) tx + (float) letterw [c] / texture->width;
      float ty2 = (float) ty - (float) height / texture->height;
      float xi = 0.1 * letterw [c] / height;
      float yi = 0.1;
      glPushMatrix ();
      glTranslatef (xz + xi / 2 - xw / 2, yi / 2, 0);
//      glScalef (1.0 + 0.25 * sine [abs (timer * 8 % 360)], 1, 1);
      glBegin (GL_QUADS);
      glColor4ub (color->c [0], color->c [1], color->c [2], color->c [3]);
      glTexCoord2f (tx, ty2);
      glVertex3f (-xi / 2, -yi / 2, 0);
      glTexCoord2f (tx, ty);
      glVertex3f (-xi / 2, yi / 2, 0);
      glTexCoord2f (tx2, ty);
      glVertex3f (xi / 2, yi / 2, 0);
      glTexCoord2f (tx2, ty2);
      glVertex3f (xi / 2, -yi / 2, 0);
      glEnd ();
      glPopMatrix ();
      xz += xi;
    }
    else if (str [i] == '\t')
    {
      int xzint = (int) (xz * 10.0);
      xzint -= xzint & 3; // modulo 4
      xzint += 4;
      xz = (float) xzint / 10.0;
    }
    else
    {
      xz += 0.05;
    }
  }
  glPopMatrix ();
  glDisable (GL_ALPHA_TEST);
  gl->disableAlphaBlending ();
  glDisable (GL_TEXTURE_2D);
}

#endif
