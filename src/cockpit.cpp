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

/* This file includes the cockpit rendering methods. */

#ifndef IS_COCKPIT_H

#include "cockpit.h"

Cockpit::Cockpit ()
{
  flarewarning = 0;
  chaffwarning = 0;
}

/*  float getGamma (DynamicObj *obj)
{
  float dy = obj->tl->y - fplayer->tl->y;
  float dx = fplayer->distance (obj);
  if (dx == 0) dx = 1E-5;
  return atan (dy / dx) * 180.0 / PI;
}

float getPhi (DynamicObj *obj)
{
  float dx2 = obj->tl->x - fplayer->tl->x, dz2 = obj->tl->z - fplayer->tl->z; // actual distances
  float a, aw, w = fplayer->phi;
  if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;
  a = atan (dx2 / dz2) * 180 / PI;
  if (dz2 > 0)
  {
    if (dx2 > 0) a -= 180;
    else a += 180;
  }
  aw = a - w;
  if (aw < -180) aw += 360;
  if (aw > 180) aw -= 360;
  return aw;
}*/

void Cockpit::setColor (int alpha)
{
  if (fplayer->o == &model_fig) glColor4ub (255, 255, 0, alpha);
  else if (fplayer->o == &model_figb) glColor4ub (255, 150, 100, alpha);
  else if (fplayer->o == &model_figc) glColor4ub (200, 200, 100, alpha);
  else if (fplayer->o == &model_figg) glColor4ub (255, 0, 0, alpha);
  else glColor4ub (100, 100, 255, alpha);
}

void Cockpit::setColor (CColor *color, int alpha)
{
  if (fplayer->o == &model_fig) color->setColor (255, 255, 0, alpha);
  else if (fplayer->o == &model_figb) color->setColor (255, 150, 100, alpha);
  else if (fplayer->o == &model_figc) color->setColor (200, 200, 100, alpha);
  else if (fplayer->o == &model_figg) color->setColor (255, 0, 0, alpha);
  else color->setColor (100, 100, 255, alpha);
}

void Cockpit::drawCounter ()
{
  int i;
  float xf = 7.0F, yf = -3.0F, zf = -10.0F;
//    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  bool flarewarn = false, chaffwarn = false;
  for (i = 0; i < maxmissile; i ++)
    if (missile [i]->active)
      if (missile [i]->target == fplayer)
      {
        if (missile [i]->id <= MISSILE_AIR1)
          flarewarn = true;
        else
          chaffwarn = true;
      }
  if (flarewarn && game == GAME_PLAY)
  {
    if (flarewarning <= 0) flarewarning = 10;
    flarewarning --;
    if (flarewarning <= 6)
    {
      glColor3ub (200, 0, 0);
      glBegin (GL_QUADS);
      glVertex3f (xf - 1.0F, yf - 1.0F, zf);
      glVertex3f (xf + 1.0F, yf - 1.0F, zf);
      glVertex3f (xf + 1.0F, yf - 0.25F, zf);
      glVertex3f (xf - 1.0F, yf - 0.25F, zf);
      glEnd ();
    }
    if (flarewarning == 1)
      sound->play (SOUND_BEEP2);
  }
  if (chaffwarn && game == GAME_PLAY)
  {
    if (chaffwarning <= 0) chaffwarning = 10;
    chaffwarning --;
    if (chaffwarning <= 6)
    {
      glColor3ub (0, 0, 200);
      glBegin (GL_QUADS);
      glVertex3f (xf - 1.0F, yf + 0.25F, zf);
      glVertex3f (xf + 1.0F, yf + 0.25F, zf);
      glVertex3f (xf + 1.0F, yf + 1.0F, zf);
      glVertex3f (xf - 1.0F, yf + 1.0F, zf);
      glEnd ();
    }
    if (chaffwarning == 1)
      sound->play (SOUND_BEEP1);
  }
  glColor3ub (255, 255, 255);
  gl->enableTextures (texcounter->textureID);
  glEnable (GL_BLEND);
  glBegin (GL_QUADS);
  glTexCoord2f (0, 0);
  glVertex3f (xf - 1.0F, yf - 1.0F, zf);
  glTexCoord2f (0.999, 0);
  glVertex3f (xf + 1.0F, yf - 1.0F, zf);
  glTexCoord2f (0.999, 0.999);
  glVertex3f (xf + 1.0F, yf + 1.0F, zf);
  glTexCoord2f (0, 0.999);
  glVertex3f (xf - 1.0F, yf + 1.0F, zf);
  glEnd ();
  glDisable (GL_BLEND);
  glDisable (GL_TEXTURE_2D);
}

void Cockpit::drawTargeter ()
{
  if (fplayer->target)
  if (fplayer->target->active)
  {
    DynamicObj *o = fplayer->target;
    float myphi = fplayer->phi;
    if (myphi < 0) myphi += 360;
    float ex1 = cosi [(int) myphi] * o->zoom, ey1 = -sine [(int) myphi] * o->zoom;
    float ex2 = -ex1, ey2 = -ey1;
    float ez = o->zoom;
    gl->enableAlphaBlending ();
    bool full = false;
    if (((AIObj *) fplayer->target)->party != fplayer->party)
    {
      if (fplayer->ttf <= 0 && fplayer->missiletype != MISSILE_DF1 - MISSILE1)
      {
        glColor4ub (255, 255, 0, 255); full = true;
      }
      else
      {
        glColor4ub (255, (50 - fplayer->ttf) * 255 / 60, 0, 255);
      }
    }
    else
    {
      glColor4ub (0, 0, 255, 255);
    }
    if (!full)
    {
      float dx = (ex2 - ex1) / 4;
      float dy = (ey2 - ey1) / 4;
      float dz = ez / 2;
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez - dz, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex1 + dx, o->tl->y + ez, o->tl->z + ey1 + dy);
      glEnd ();
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex2 - dx, o->tl->y + ez, o->tl->z + ey2 - dy);
      glVertex3f (o->tl->x + ex2, o->tl->y + ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex2, o->tl->y + ez - dz, o->tl->z + ey2);
      glEnd ();
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex2, o->tl->y - ez + dz, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex2, o->tl->y - ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex2 - dx, o->tl->y - ez, o->tl->z + ey2 - dy);
      glEnd ();
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex1 + dx, o->tl->y - ez, o->tl->z + ey1 + dy);
      glVertex3f (o->tl->x + ex1, o->tl->y - ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex1, o->tl->y - ez + dz, o->tl->z + ey1);
      glEnd ();
    }
    else
    {
      glBegin (GL_LINE_STRIP);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex2, o->tl->y + ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex2, o->tl->y - ez, o->tl->z + ey2);
      glVertex3f (o->tl->x + ex1, o->tl->y - ez, o->tl->z + ey1);
      glVertex3f (o->tl->x + ex1, o->tl->y + ez, o->tl->z + ey1);
      glEnd ();
    }
    gl->disableAlphaBlending ();
  }

}

void Cockpit::drawCross ()
{
  glDisable (GL_LIGHTING);

  gl->enableAlphaBlending ();
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, 0.1);
  //  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  if (fplayer->o == &model_fig || fplayer->o == &model_figg)
  {
    gl->enableTextures (texcross->textureID);
  }
  else
  {
    gl->enableTextures (texcross2->textureID);
  }

  float xf = 0.1, yf = 0.1, zf = 1.0;
  setColor (150);
  glBegin (GL_QUADS);
  glTexCoord2d (0, 0);
  glVertex3f (-xf, -yf, -zf);
  glTexCoord2d (0, 1);
  glVertex3f (-xf, yf, -zf);
  glTexCoord2d (1, 1);
  glVertex3f (xf, yf, -zf);
  glTexCoord2d (1, 0);
  glVertex3f (xf, -yf, -zf);
  glEnd ();
  glDisable (GL_ALPHA_TEST);
  glDisable (GL_TEXTURE_2D);
  gl->disableAlphaBlending ();
  //  glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void Cockpit::drawHeading ()
{
  CColor color;
  int i = 0;
  int alpha = 175;
  setColor (&color, alpha);
  glDisable (GL_LIGHTING);
  glLineWidth (2.0);
  glDisable (GL_DEPTH_TEST);
  float xf;
  float yf = 17.5;
  float zf = -2.5;
  float g = 0.5;
//    float p1 = fplayer->phi - 40.0;
//    float p2 = fplayer->phi + 40.0;
  for (i = 0; i < 360; i += 5)
  {
    float p = (float) i - fplayer->phi;
    if (p < -180) p += 360;
    if (p > 180) p -= 360;
    if (p >= -40 && p <= 40)
    {
      if ((i - mission->heading) % 90 == 0)
        g = 0.6;
      else if (!(i % 10))
        g = 0.3;
      else
        g = 0.1;
      xf = p / 6.0;
      gl->enableAlphaBlending ();
      setColor (alpha);
      glBegin (GL_LINES);
      glVertex3f (xf * 0.1, (yf - g) * 0.1, zf);
      glVertex3f (xf * 0.1, yf * 0.1, zf);
      glEnd ();
      if (i == mission->heading)
        font1->drawText (xf - 0.5, yf - 2.0, zf, "N", &color);
      else if (i == mission->heading + 90 || i == mission->heading - 270)
        font1->drawText (xf - 0.5, yf - 2.0, zf, "W", &color);
      else if (i == mission->heading + 180 || i == mission->heading - 180)
        font1->drawText (xf - 0.5, yf - 2.0, zf, "S", &color);
      else if (i == mission->heading + 270 || i == mission->heading - 90)
        font1->drawText (xf - 0.5, yf - 2.0, zf, "E", &color);
    }
  }

  if (fplayer->target != NULL)
    if (fplayer->target->active)
    {
      float disttarget = fplayer->distance (fplayer->target);
      float dgamma = atan ((fplayer->target->tl->y - fplayer->tl->y) / disttarget) * 180 / PI - (fplayer->gamma - 180);
      float dphi = fplayer->getAngle (fplayer->target);
      if (fabs (dphi) > 50 || fabs (dgamma) > 50)
      {
        if (dphi < -90) dphi = -90;
        else if (dphi > 90) dphi = 90;
        float delta = atan (dgamma / dphi) * 180 / PI;
        if (dphi > 0) delta -= 180;
//        printf ("delta=%f, dgamma=%f, dphi=%f\n",delta, dgamma, dphi);
        delta += fplayer->theta;
        while (delta < 0) delta += 360;
        while (delta >= 360) delta -= 360;
        float xs = cosi [(int) delta] * 3;
        float ys = -sine [(int) delta] * 3;
/*        if (xs > 3) xs = 3;
        else if (xs < -3) xs = -3;
        if (ys > 3) ys = 3;
        else if (ys < -3) ys = -3;*/
        zf = -4;
        glBegin (GL_LINE_STRIP);
        glColor4ub (255, 0, 0, alpha);
        glVertex3f (xs - 0.12, ys - 0.12, zf);
        glVertex3f (xs + 0.12, ys - 0.12, zf);
        glVertex3f (xs + 0.12, ys + 0.12, zf);
        glVertex3f (xs - 0.12, ys + 0.12, zf);
        glVertex3f (xs - 0.12, ys - 0.12, zf);
        glEnd ();
        glBegin (GL_LINES);
        glVertex3f (xs - 0.15, ys - 0.15, zf);
        glVertex3f (xs + 0.15, ys + 0.15, zf);
        glVertex3f (xs + 0.15, ys - 0.15, zf);
        glVertex3f (xs - 0.15, ys + 0.15, zf);
        glEnd ();
      }
    }

  zf = -3.5;
  for (i = 0; i < 360; i += 5)
  {
    float p = (float) i - fplayer->gamma;
    if (p < -180) p += 360;
    if (p > 180) p -= 360;
    if (p >= -30 && p <= 30)
    {
      if (i == 0 || i == 90 || i == 180 || i == 270)
        g = 0.6;
      else if (!(i % 10))
        g = 0.3;
      else
        g = 0.1;
      xf = p / 6.0;
      gl->enableAlphaBlending ();
      yf = -7;
      setColor (alpha);
      glBegin (GL_LINES);
      glVertex3f ((yf - g) * 0.1, xf * 0.1, zf);
      glVertex3f (yf * 0.1, xf * 0.1, zf);
      glEnd ();
      yf = -8.5;
      if (i == 120)
        font1->drawText (yf - 2.0, xf - 0.5, zf, "-60", &color);
      else if (i == 150)
        font1->drawText (yf - 2.0, xf - 0.5, zf, "-30", &color);
      else if (i == 180)
        font1->drawText (yf - 2.0, xf - 0.5, zf, "0", &color);
      else if (i == 210)
        font1->drawText (yf - 2.0, xf - 0.5, zf, "30", &color);
      else if (i == 240)
        font1->drawText (yf - 2.0, xf - 0.5, zf, "60", &color);
    }
  }

  gl->enableAlphaBlending ();
  int t1 = (int) (fplayer->theta - 135.0);
  int t2 = t1 + 90, t3 = t1 + 180, t4 = t1 + 270;
  if (t1 != 0) t1 %= 360;
  if (t2 != 0) t2 %= 360;
  if (t3 != 0) t3 %= 360;
  if (t4 != 0) t4 %= 360;
  if (t1 < 0) t1 += 360;
  if (t2 < 0) t2 += 360;
  if (t3 < 0) t3 += 360;
  if (t4 < 0) t4 += 360;
  xf = 0.6; yf = 0.6;
  setColor (130);
  glBegin (GL_LINES);
  glVertex3f (xf * cosi [t1], yf * sine [t1], zf);
  glVertex3f (xf * cosi [t2], yf * sine [t2], zf);
  glVertex3f (xf * cosi [t3], yf * sine [t3], zf);
  glVertex3f (xf * cosi [t4], yf * sine [t4], zf);
  glEnd ();
  gl->disableAlphaBlending ();
}

void Cockpit::drawTargetedElement ()
{
  char str [20];
  CColor color;
  CVector3 tl, n;
  CRotation r;
  color.setColor (255, 0, 0);
  color.c [3] = 255;
  tl.x = -0.35; tl.y = -0.3; tl.z = -0.5;
  gl->disableAlphaBlending ();
  glEnable (GL_DEPTH_TEST);
  if (fplayer->target != NULL)
    if (fplayer->target->active)

    {
      glEnable (GL_LIGHTING);
      fplayer->target->o->draw (&n, &tl, fplayer->target->rot, 0.05, 1.0, 0);
      glDisable (GL_LIGHTING);
      if (((AIObj *) fplayer->target)->party == fplayer->party)
        color.setColor (0, 0, 255);
      font1->drawText (-24.0, -23.0, -4.0, fplayer->target->o->name, &color);
      sprintf (str, "%d", (int) (15.0 * fplayer->distance (fplayer->target)));
      font1->drawText (-24.0, -25.0, -4.0, str, &color);
    }
/*    color.setColor (255, 255, 255);
  sprintf (str, "MISSILES: %d", fplayer->missiles);
  font1->drawText (20.0, -25.0, -4.0, str, &color);*/
  glDisable (GL_DEPTH_TEST);
//    font1->drawText (0, 0, -5.0, "ABCDEFG", &color);
}

void Cockpit::drawWeapon ()
{
  char str [20];
  CColor color;
  CVector3 tl, n;
  CRotation r;
  if (fplayer->missiles [fplayer->missiletype] <= 0)
    return;
  color.setColor (255, 0, 0);
  color.c [3] = 255;
  tl.x = 0.35; tl.y = -0.3; tl.z = -0.5;
  gl->disableAlphaBlending ();
  glEnable (GL_DEPTH_TEST);
  CModel *missile;
  if (fplayer->missiletype == 0) missile = &model_missile1;
  else if (fplayer->missiletype == 1) missile = &model_missile2;
  else if (fplayer->missiletype == 2) missile = &model_missile3;
  else if (fplayer->missiletype == 3) missile = &model_missile4;
  else if (fplayer->missiletype == 4) missile = &model_missile5;
  else if (fplayer->missiletype == 5) missile = &model_missile6;
  else if (fplayer->missiletype == 6) missile = &model_missile7;
  else if (fplayer->missiletype == 7) missile = &model_missile8;
  glEnable (GL_LIGHTING);
  missile->draw (&n, &tl, fplayer->rot, 0.05, 1.0, 0);
  glDisable (GL_LIGHTING);
  glDisable (GL_DEPTH_TEST);
  font1->drawText (16.0, -22.0, -4.0, missile->name, &color);
  sprintf (str, "N %d", fplayer->missiles [fplayer->missiletype]);
  font1->drawText (16.0, -24.0, -4.0, str, &color);
}

void Cockpit::drawRadar ()
{
  int i;
  float yf = -4.2, zf = -7.0;
gl->enableAlphaBlending ();
glEnable (GL_ALPHA_TEST);
glAlphaFunc (GL_GEQUAL, 0.1);
glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
setColor (150);
float xl, yl;
int type;
if (fplayer->o == &model_fig || fplayer->o == &model_figg)
{
  gl->enableTextures (texradar2->textureID);
  xl = 1.4; yl = 1.3;
  type = 0;
}
else
{
  gl->enableTextures (texradar1->textureID);
  xl = 1.5; yl = 1.2;
  type = 1;
}
glBegin (GL_QUADS);
glTexCoord2d (0, 0);
glVertex3f (-xl, yf - yl, zf);
glTexCoord2d (0, 1);
glVertex3f (-xl, yf + yl, zf);
glTexCoord2d (1, 1);
glVertex3f (xl, yf + yl, zf);
glTexCoord2d (1, 0);
glVertex3f (xl, yf - yl, zf);
glEnd ();
glDisable (GL_ALPHA_TEST);
glDisable (GL_TEXTURE_2D);
//  gl->disableAlphaBlending ();
/*    gl->enableAlphaBlending ();
  glDisable (GL_DEPTH_TEST);
  glBegin (GL_QUADS);
  glColor4ub (255, 255, 0, 100);
  glVertex3f (-1.2, yf - 1.0, zf);
  glVertex3f (1.2, yf - 1.0, zf);
  glVertex3f (1.2, yf + 1.0, zf);
  glVertex3f (-1.2, yf + 1.0, zf);
  glEnd ();*/
  glPointSize (2.0);
  glBegin (GL_POINTS);
  glColor4ub (255, 255, 255, 255);
  glVertex3f (0.0, yf, zf);
  for (i = 0; i < maxfighter; i ++)
    if (fighter [i] != fplayer && fighter [i]->active)
    {
      int aw = fplayer->getAngle (fighter [i]);
      if (aw < 0) aw += 360;
      float d = fplayer->distance (fighter [i]) / 100.0;
      float px = -d * sine [aw];
      float py = yf + d * cosi [aw];
      if ((type == 0 && d < 1.2) || (type == 1 && px >= -1.2 && px <= 1.2 && py >= yf - 1.1 && py <= yf + 1.1))
      {
        if (fighter [i] == fplayer->target)
          glColor4ub (255, 200, 0, 255);
        else if (fighter [i]->party != fplayer->party)
          glColor4ub (255, 0, 0, 255);
        else
          glColor4ub (0, 0, 255, 255);
        glVertex3f (px, py, zf);
      }
    }
  glEnd ();
  gl->disableAlphaBlending ();
}

#endif
