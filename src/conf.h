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

/* This file contains all configuration parsing code. */

#ifndef IS_CONF_H
#define IS_CONF_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "common.h"

// interface definitions
extern unsigned char key_firecannon, key_firemissile, key_dropchaff, key_dropflare;
extern unsigned char key_selectmissile;
extern unsigned char key_targetnearest, key_targetnext, key_targetprevious;

extern unsigned char joystick_firecannon, joystick_firemissile, joystick_dropchaff, joystick_dropflare;
extern unsigned char joystick_selectmissile;
extern unsigned char joystick_targetnearest, joystick_targetnext, joystick_targetprevious;
extern unsigned char joystick_aileron, joystick_elevator, joystick_rudder, joystick_throttle;

extern unsigned char mouse_firecannon, mouse_firemissile;
extern unsigned char mouse_selectmissile;

// the abstract config file as text file in memory
class ConfigFile
{
  public:
  char buf [32000];
  int length;
  FILE *out;
  ConfigFile ();
  ConfigFile (char *fname);
  char *skipwhite (char *str);
  char *skipnum (char *str);
  char *getString (char *dest, char *str);
  int getValue (char *str);
  int openOutput (char *fname);
  int write (char *str1, int n);
  void writeText (char *str);
  void close ();
};

// special config files of GL-117
extern void save_config ();
extern int load_config ();
extern void save_configInterface ();
extern int load_configInterface ();

#endif