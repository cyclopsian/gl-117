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



Container::Container ()
{
  numcomponents = 0;
}

Container::~Container ()
{
}

void Container::add (Component *component)
{
  component->parent = this;
  components.push_back (component);
//  numcomponents ++;
}

void Container::draw ()
{
  int i;
  if (active && visible)
  {
    for (i = 0; i < components.size (); i ++)
    {
      components [i]->draw ();
    }
  }
}

int Container::getNumberOfComponents ()
{
  return components.size ();
}

void Container::eventMouse (int x, int y, int button)
{
  int i, i2;

//  glutSwapBuffers ();
	GLuint selectBuff [20 * 4];
	GLint hits, viewport [4];

	// Select buffer parameters
	glSelectBuffer (20 * 4, selectBuff);
	glGetIntegerv (GL_VIEWPORT, viewport);
	
	// Enter to selection mode
	glEnable (GL_DEPTH_TEST);
	glDisable (GL_LIGHTING);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();

	// Set-up pick matrix
  gluPickMatrix (x, viewport [3] - y, 1, 1, viewport);

	// Set our perpective transformation matrix
  gluPerspective (80.0, 1.0, 0.25 * GLOBALSCALE, view * GLOBALSCALE);
//  gluPerspective (60.0, 1.0, 0.2, 200.0);
	
	glMatrixMode (GL_MODELVIEW);

	// Render all scene and fill selection buffer
	glRenderMode (GL_SELECT);
  
  glInitNames ();
  glPushName (0);
  draw ();

	// Get hits and go back to normal rendering
	hits = glRenderMode (GL_RENDER);

	// Get first model in selection buffer stack
  GLuint *ptr = selectBuff;
  int mypicks = 0;
  int pick [128];
	if (hits > 0)
  {
    int i;
    for (i = 0; i < hits; i ++)
    {
      int names = *ptr;
      ptr ++;
      ptr ++;
      ptr ++;
      pick [mypicks] = *ptr;
      mypicks ++;
      ptr += names;
      if (mypicks >= 20) break;
    }
  }

	glMatrixMode (GL_PROJECTION);

	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);

  for (i = 0; i < components.size (); i ++)
  {
    components [i]->setHighlight (false);
    for (i2 = 0; i2 < hits; i2 ++)
    {
      if (components [i]->contains (pick [i2]))
      {
        components [i]->triggerMouse (pick [i2], button);
      }
    }
  }
}

void Container::triggerMouse (int id, int button)
{
  int i;
  for (i = 0; i < components.size (); i ++)
    if (components [i]->contains (id))
    {
      components [i]->triggerMouse (id, button);
    }
}

void Container::setVisible (bool value)
{
  int i;
  visible = value;
  for (i = 0; i < components.size (); i ++)
  {
    components [i]->setVisible (value);
  }
}

void Container::setActive (bool value)
{
  int i;
  active = value;
  for (i = 0; i < components.size (); i ++)
  {
    components [i]->setActive (value);
  }
}

void Container::setHighlight (bool value)
{
  int i;
  highlight = value;
  for (i = 0; i < components.size (); i ++)
  {
    components [i]->setHighlight (value);
  }
}

bool Container::contains (int id)
{
  int i;
  bool ret = false;
  for (i = 0; i < components.size (); i ++)
  {
    ret |= components [i]->contains (id);
  }
  return ret;
}

void Container::eventKey (unsigned char key)
{
  int i;
  for (i = 0; i < components.size (); i ++)
  {
    components [i]->eventKey (key);
  }
}

void Container::eventSpecial (int key)
{
  int i;
  for (i = 0; i < components.size (); i ++)
  {
    components [i]->eventSpecial (key);
  }
}

void Container::eventJoystick (int button)
{
  int i;
  for (i = 0; i < components.size (); i ++)
  {
    components [i]->eventJoystick (button);
  }
}

#endif
