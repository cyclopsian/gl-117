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
#define IS_COCKPIT_H

#include "main.h"

class Cockpit
{
  public:
  int flarewarning, chaffwarning;

  Cockpit ();

  void setColor (int alpha);
  void setColor (CColor *color, int alpha);
  void drawCounter ();
  void drawTargeter ();
  void drawCross ();
  void drawHeading ();
  void drawTargetedElement ();
  void drawWeapon ();
  void drawRadar ();
};

#endif