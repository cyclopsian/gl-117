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

#ifndef IS_COMPONENT_H

#include "Component.h"
#include "main.h"



EditField::EditField ()
{
  memset (text, 0, 64);
  ptr = 0;

#ifndef USE_GLUT
  timer = SDL_GetTicks ();
#else
  timer = glutGet (GLUT_ELAPSED_TIME);
#endif
}

EditField::EditField (int maxlen)
{
  memset (this->text, 0, 64);
  ptr = 0;
  if (maxlen > 62) maxlen = 62;
  else if (maxlen < 0) maxlen = 0;
  this->maxlen = maxlen;

#ifndef USE_GLUT
  timer = SDL_GetTicks ();
#else
  timer = glutGet (GLUT_ELAPSED_TIME);
#endif
}

void EditField::setText (char *text)
{
  if (strlen (text) > (unsigned int) maxlen) return;
  memset (this->text, 0, 64);
  strcpy (this->text, text);
  ptr = strlen (this->text);
}

void EditField::draw ()
{
  char str [64];
  int akttime;

  if (!visible) return;
  Color *col = &DeactiveTextColor;

  strcpy (str, text);
  if (highlight)
  {
#ifndef USE_GLUT
    akttime = SDL_GetTicks ();
#else
    akttime = glutGet (GLUT_ELAPSED_TIME);
#endif

    if (((akttime - timer) & 0xff) > 128)
    {
      str [ptr] = 'I';
      str [ptr + 1] = 0;
    }
  }

  glLoadName (id);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBegin (GL_QUADS);
  if (!highlight)
    glColor4ubv (colback.c);
  else
    glColor4ub (255, 255, 0, colback.c [3]);
  glVertex3f (0.1 * x, 0.1 * y, menuZ);
  glVertex3f (0.1 * (x + w), 0.1 * y, menuZ);
  glVertex3f (0.1 * (x + w), 0.1 * (y - h), menuZ);
  glVertex3f (0.1 * x, 0.1 * (y - h), menuZ);
  glEnd ();
  glDisable (GL_BLEND);
  font1->zoom = 0.1 * (h - 0.1);
  float yf = (y - h - 0.01) / (h - 0.1);
  float xf = (x + 0.25) / (h - 0.1);
  font1->drawText (xf, yf, menuZ, str, *col);
  font1->zoom = 0.1;
}

void EditField::triggerMouse (int id, int button)
{
  if (!active) return;
  highlight = true;
}

void EditField::eventKey (unsigned char key)
{
  if (!highlight) return;
  if (ptr >= 62) return;
  if (key >= 32)
  {
    text [ptr] = key;
    ptr ++;
    text [ptr] = 0;
  }
  else if (key == 8 && ptr > 0)
  {
    ptr --;
    text [ptr] = 0;
  }
}

#endif
