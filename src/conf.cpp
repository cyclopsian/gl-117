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

#include "conf.h"
#include "dirs.h"

int quality = 3;
float view = 50.0;
int width = 800, height = 600, bpp = 32;
int dithering = 0;
int volumesound = 100;
int volumemusic = 100;
int fullscreen = 1;
int controls = CONTROLS_MOUSE;
int difficulty = 1;

unsigned char key_firecannon = 32, key_firemissile = 13, key_dropchaff = 'C', key_dropflare = 'F';
unsigned char key_selectmissile = 'M', key_thrustup = 'I', key_thrustdown = 'D';
unsigned char key_targetnearest = 'E', key_targetnext = 'T', key_targetprevious = 'P', key_targetlocking = 'L';

unsigned int joystick_firecannon = 0, joystick_firemissile = 2, joystick_dropchaff = 3, joystick_dropflare = 3;
unsigned int joystick_selectmissile = 1;
unsigned int joystick_targetnearest = 101, joystick_targetnext = 100, joystick_targetprevious = 102;
unsigned int joystick_aileron = 0, joystick_elevator = 1, joystick_rudder = 3, joystick_throttle = 2;

unsigned char mouse_firecannon = MOUSE_BUTTON_LEFT, mouse_firemissile = MOUSE_BUTTON_RIGHT;
unsigned char mouse_selectmissile = MOUSE_BUTTON_MIDDLE;

ConfigFile::ConfigFile () {}

ConfigFile::ConfigFile (char *fname)
{
  char buf2 [STDSIZE];
  bool commentmode = false;
  FILE *in;
  length = 0;
  in = fopen (fname, "rb");
  if (in != NULL)
  {
    length = fread (buf, 1, 32000, in);
    fclose (in);
  }
  else
  {
    sprintf (buf2, "Could not load %s", fname);
    display (buf2, LOG_WARN);
    buf [0] = 0;
  }
  for (int i = 0; i < length; i ++)
  {
    if (buf [i] == '#') commentmode = true;
    if (buf [i] == '\n') commentmode = false;
    if (commentmode) buf [i] = ' ';
    else buf [i] = tolower (buf [i]);
  }
}

char *ConfigFile::skipwhite (char *str)
{
  while (*str == ' ' || *str == '\t')
    str ++;
  return str;
}

char *ConfigFile::skipnum (char *str)
{
  while (*str >= '0' && *str <= '9')
    str ++;
  return str;
}

char *ConfigFile::skipalphanum (char *str)
{
  while (*str >= '0' && *str <= 'z')
    str ++;
  return str;
}

char *ConfigFile::getString (char *dest, char *str)
{
  char *strf, *stre;
  char cmpstr [256];
  int i;
  if (strlen (str) >= 256) { return NULL; }
  strcpy (cmpstr, str);
  if (buf [0] == 0) { return NULL; }
  for (i = 0; i < (int) strlen (cmpstr); i ++)
  {
    cmpstr [i] = tolower (cmpstr [i]);
  }
  strf = strstr (buf, cmpstr);
  if (strf == NULL) { return NULL; }
  strf += strlen (cmpstr);
  strf = skipwhite (strf);
  if (*strf == 0) { return NULL; }
  strf ++;
  if (*strf == 0) { return NULL; }
  strf = skipwhite (strf);
  if (*strf == 0) { return NULL; }
  stre = skipalphanum (strf);
  if (stre - strf >= 256) { return NULL; }
  if (*strf == 0) { return NULL; }
  for (i = 0; i < (int) (stre - strf); i ++)
    dest [i] = *(strf+i);
  dest [i] = 0;
  return dest;
}

int ConfigFile::getValue (char *str)
{
  char res [256];
  getString (res, str);
  if (res == NULL) return -1;
  return atoi (res);
}

int ConfigFile::openOutput (char *fname)
{
  out = fopen (fname, "wb");
  if (out == NULL) return 0;
  return 1;
}

int ConfigFile::write (char *str1, int n)
{
  char str [256];
  if (strlen (str1) + 8 > 256) return 0;
  sprintf (str, "%s = %d\n", str1, n);
  fwrite (str, 1, strlen (str), out);
  return 1;
}

int ConfigFile::write (char *str1, char c)
{
  char str [256];
  if (c <= 32 || c >= 97)
  {
    write (str1, (int) c);
    return 1;
  }
  if (strlen (str1) + 8 > 256) return 0;
  sprintf (str, "%s = %c\n", str1, c);
  fwrite (str, 1, strlen (str), out);
  return 1;
}

void ConfigFile::writeText (char *str)
{
  fwrite (str, 1, strlen (str), out);
  fwrite ("\n", 1, 1, out);
}

void ConfigFile::close ()
{
  fclose (out);
}



void save_config ()
{
  char buf [STDSIZE];
  ConfigFile *cf = new ConfigFile ();
  char *confname = dirs->getSaves ("conf");
  sprintf (buf, "Saving %s ", confname);
  display (buf, LOG_MOST);
  int ret1 = cf->openOutput (confname);
  if (ret1 == 0)
  {
    display ("Could not save configuration", LOG_ERROR);
    fflush (stderr);
    return;
  }
  cf->writeText ("# Configuration\n");
  cf->writeText ("# Some possible width x height values for fullscreen mode:");
  cf->writeText ("#  640x480, 800x600, 1024x768, 1152x864, 1280x768, 1280x960, 1280x1024");
  cf->write (" width", width);
  cf->write (" height", height);
  cf->writeText ("# Possible bits per pixel values (color depth):");
  cf->writeText ("#  8 (not recommended), 16, 24, 32");
  cf->write (" bpp", bpp);
  cf->writeText ("# Try to go fullscreen = 1, game in window = 0");
  cf->write (" fullscreen", fullscreen);
  cf->writeText ("# Quality: 0=software rendered up to 5=best (default=2)");
  cf->write (" quality", quality);
  cf->writeText ("# Far clipping plane: 20..100 (default=50)");
  cf->write (" view", (int) view);
  cf->writeText ("# Dithering: 0=off, 1=on (default=0)");
  cf->write (" dithering", dithering);
#ifdef HAVE_SDL_MIXER
  cf->writeText ("# Sound volume: 0..100 (default=100) per cent");
  cf->write (" sound", (int) volumesound);
  cf->writeText ("# Music volume: 0..100 (default=100) per cent");
  cf->write (" music", (int) volumemusic);
#endif
  cf->writeText ("# Piloting controls: 0=keyboard, 1=mouse easy, 2=joystick, 3=mouse reverse");
  cf->write (" controls", controls);
  cf->writeText ("# Difficulty level: 0=easy, 1=medium, 2=hard");
  cf->write (" difficulty", difficulty);
  cf->writeText ("# Brightness: -50..50 per cent (default=0)");
  cf->write (" brightness", brightness);
  cf->writeText ("\n# This file is meant to give sensible startup settings");
  cf->writeText ("# as graphic cards and drivers may differ some 100 times in speed");
  cf->writeText ("\n# To get back to default settings, just delete this file!");
  cf->close ();
  delete cf;
}

int load_config ()
{
  char buf [STDSIZE];
  char ret [256];
  char *str;
  char *confname = dirs->getSaves ("conf");
  sprintf (buf, "Loading %s ", confname);
  display (buf, LOG_MOST);
  ConfigFile *cf = new ConfigFile (confname);

  str = cf->getString (ret, "width");
  if (str == NULL)
  { width = 800; }
  else
  { width = atoi (str); }
  if (width < 100) width = 100;
  else if (width > 3000) width = 3000;

  str = cf->getString (ret, "height");
  if (str == NULL)
  { height = 600; }
  else
  { height = atoi (str); }
  if (height < 100) height = 100;
  else if (height > 2000) height = 2000;

  str = cf->getString (ret, "bpp");
  if (str == NULL)
  { bpp = 32; }
  else
  { bpp = atoi (str); }
  if (bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32)
    bpp = 32;

  str = cf->getString (ret, "fullscreen");
  if (str == NULL)
  { fullscreen = 1; }
  else
  { fullscreen = atoi (str); }
  if (fullscreen) fullscreen = 1;

  str = cf->getString (ret, "quality");
  if (str == NULL)
  { quality = 2; }
  else
  { quality = atoi (str); }
  if (quality < 0) quality = 0;
  else if (quality > 5) quality = 5;

  str = cf->getString (ret, "view");
  if (str == NULL)
  { view = 50; }
  else
  { view = atoi (str); }
  if (view < 30)
  {
    view = 30;
//    firststart = true;
  }
  else if (view > 100)
  {
    view = 100;
  }

  str = cf->getString (ret, "dithering");
  dithering = (str == NULL) ? 1 : atoi (str);
  if (dithering) dithering = 1;

#ifdef HAVE_SDL_MIXER
  str = cf->getString (ret, "sound");
  if (str == NULL)
  { volumesound = 100; }
  else
  { volumesound = atoi (str); }
  if (volumesound < 0) volumesound = 0;
  else if (volumesound > 100) volumesound = 100;

  str = cf->getString (ret, "music");
  if (str == NULL)
  { volumemusic = 100; }
  else
  { volumemusic = atoi (str); }
  if (volumemusic < 0) volumemusic = 0;
  else if (volumemusic > 100) volumemusic = 100;
#endif

  str = cf->getString (ret, "controls");
  if (str == NULL)
  { controls = CONTROLS_MOUSE; }
  else
  { controls = atoi (str); }
  if (controls < 0) controls = 0;
  else if (controls > 4) controls = 0;

  str = cf->getString (ret, "difficulty");
  if (str == NULL)
  { difficulty = 1; }
  else
  { difficulty = atoi (str); }
  if (difficulty < 0) difficulty = 0;
  else if (difficulty > 2) difficulty = 0;

  str = cf->getString (ret, "brightness");
  if (str == NULL)
  { brightness = 0; }
  else
  { brightness = atoi (str); }
  if (brightness < -50) brightness = -50;
  else if (brightness > 50) brightness = 50;

  if (cf->buf [0] == 0) // no file found
  {
    delete cf;
    return 0;
  }

  delete cf;
  return 1;
}

void writeJoystick (ConfigFile *cf, char *str, int jn)
{
  char buf [STDSIZE];
  sprintf (buf, "%s = %c%d", str, 'A' + (jn / 1000), jn % 1000);
  cf->writeText (buf);
}

void save_configInterface ()
{
  char buf [STDSIZE];
  ConfigFile *cf = new ConfigFile ();
  char *confname = dirs->getSaves ("conf.interface");
  sprintf (buf, "Saving %s ", confname);
  display (buf, LOG_MOST);
  int ret1 = cf->openOutput (confname);
  if (ret1 == 0)
  {
    display ("Could not save interface configuration", LOG_ERROR);
    return;
  }
  cf->writeText ("# Interface configuration\n");
  cf->writeText ("# ---------------------------------------------------------------------");
  cf->writeText ("# Keyboard section");
  cf->writeText ("# ---------------------------------------------------------------------\n");
  cf->writeText ("# Use ASCII-Code values or letters to remap");
  cf->writeText ("#  8=BACKSPACE, 13=ENTER, 32=SPACE, 65=A...90=Z (NOT case sensitive)");
  cf->write (" key_firecannon", (char) key_firecannon);
  cf->write (" key_firemissile", (char) key_firemissile);
  cf->write (" key_dropflare", (char) key_dropflare);
  cf->write (" key_dropchaff", (char) key_dropchaff);
  cf->write (" key_selectmissile", (char) key_selectmissile);
  cf->writeText ("#  Target nearest enemy with priority for enemies in front of you");
  cf->write (" key_targetnearest", (char) key_targetnearest);
  cf->writeText ("#  Target next enemy who has locked you");
  cf->write (" key_targetlocking", (char) key_targetlocking);
  cf->write (" key_targetnext", (char) key_targetnext);
  cf->write (" key_targetprevious", (char) key_targetprevious);
  cf->write (" key_incthrust", (char) key_thrustup);
  cf->write (" key_decthrust", (char) key_thrustdown);
  cf->writeText ("# All other piloting keys (CURSORS, PGUP/DN) are fixed.");
  cf->writeText ("\n# ---------------------------------------------------------------------");
  cf->writeText ("# Mouse section");
  cf->writeText ("# ---------------------------------------------------------------------\n");
  cf->writeText ("# Buttons: 1=Left, 2=Middle, 3=Right");
  int mousebutton = 1;
  if (mouse_firecannon == MOUSE_BUTTON_MIDDLE) mousebutton = 2;
  else if (mouse_firecannon == MOUSE_BUTTON_RIGHT) mousebutton = 3;
  else mousebutton = 1;
  cf->write (" mouse_firecannon", mousebutton);
  if (mouse_firemissile == MOUSE_BUTTON_MIDDLE) mousebutton = 2;
  else if (mouse_firemissile == MOUSE_BUTTON_RIGHT) mousebutton = 3;
  else mousebutton = 1;
  cf->write (" mouse_firemissile", mousebutton);
  if (mouse_selectmissile == MOUSE_BUTTON_MIDDLE) mousebutton = 2;
  else if (mouse_selectmissile == MOUSE_BUTTON_RIGHT) mousebutton = 3;
  else mousebutton = 1;
  cf->write (" mouse_selectmissile", mousebutton);
  cf->writeText ("\n# ---------------------------------------------------------------------");
  cf->writeText ("# Joystick section");
  cf->writeText ("# ---------------------------------------------------------------------\n");
  cf->writeText ("# The number of axes, buttons, and the coolie hat depends on your joystick!");
  cf->writeText ("# Numbers start with A=first joystick, B=second joystick...J=10th joystick");
  cf->writeText ("#  followed by a number to identify axes, buttons, and coolie hat\n");
  cf->writeText ("# Axis: 0...MAX-1 (maybe 0=aileron 1=elevator 2=throttle 3=rudder)");
  writeJoystick (cf, " joystick_aileron", joystick_aileron);
  writeJoystick (cf, " joystick_elevator", joystick_elevator);
  writeJoystick (cf, " joystick_throttle", joystick_throttle);
  writeJoystick (cf, " joystick_rudder", joystick_rudder);
  cf->writeText ("\n# Buttons: 0...MAX-1");
  writeJoystick (cf, " joystick_firecannon", joystick_firecannon);
  writeJoystick (cf, " joystick_firemissile", joystick_firemissile);
  cf->writeText ("# Dropping chaff AND flare may be preferred");
  writeJoystick (cf, " joystick_dropflare", joystick_dropflare);
  writeJoystick (cf, " joystick_dropchaff", joystick_dropchaff);
  cf->writeText ("\n# Buttons: 0...MAX-1, Coolie: 100=Right, 101=Up, 102=Left, 103=Down");
  writeJoystick (cf, " joystick_selectmissile", joystick_selectmissile);
  writeJoystick (cf, " joystick_targetnearest", joystick_targetnearest);
  writeJoystick (cf, " joystick_targetnext", joystick_targetnext);
  writeJoystick (cf, " joystick_targetprevious", joystick_targetprevious);
  cf->writeText ("\n# This file is meant to give sensible custom interface settings");
  cf->writeText ("\n# To get back to default settings, just delete this file!");
  cf->close ();
  delete cf;
}

int getKey (char *str, int n)
{
  int tmp;
  if (str == NULL) return n;
  tmp = atoi (str);
  if (tmp == 0)
  {
    if (str [0] >= 33)
    {
      return toupper (str [0]);
    }
    return n;
  }
  return tmp;
}

int getJoystick (char *str, int n)
{
  int tmp, jn = 0;
  if (str == NULL) return n;
  int str0 = toupper (str [0]) - (int) 'A';
  if (str0 >= 0 && str0 < 10)
    jn = str0;
  else
    return n;
  tmp = atoi (&str [1]);
/*  if (tmp == 0) // problem: tmp=0 is valid!
  {
    return n;
  }*/
  return jn * 1000 + tmp;
}

int load_configInterface ()
{
  char buf [STDSIZE];
  char ret [256];
  char *str;
  char *confname = dirs->getSaves ("conf.interface");
  sprintf (buf, "Loading %s ", confname);
  display (buf, LOG_MOST);
  ConfigFile *cf = new ConfigFile (confname);

  str = cf->getString (ret, "key_firecannon");
  key_firecannon = getKey (str, 32);

  str = cf->getString (ret, "key_firemissile");
  key_firemissile = getKey (str, 13);

  str = cf->getString (ret, "key_dropchaff");
  key_dropchaff = getKey (str, 'C');

  str = cf->getString (ret, "key_dropflare");
  key_dropflare = getKey (str, 'F');

  str = cf->getString (ret, "key_selectmissile");
  key_selectmissile = getKey (str, 'M');

  str = cf->getString (ret, "key_targetnearest");
  key_targetnearest = getKey (str, 'E');

  str = cf->getString (ret, "key_targetlocking");
  key_targetlocking = getKey (str, 'L');

  str = cf->getString (ret, "key_targetnext");
  key_targetnext = getKey (str, 'T');

  str = cf->getString (ret, "key_targetprevious");
  key_targetprevious = getKey (str, 'P');

  str = cf->getString (ret, "key_incthrust");
  key_thrustup = getKey (str, 'I');

  str = cf->getString (ret, "key_decthrust");
  key_thrustdown = getKey (str, 'D');

  int mousebutton = 1;
  str = cf->getString (ret, "mouse_firecannon");
  if (str == NULL)
  { mousebutton = 1; }
  else
  { mousebutton = atoi (str); }
  if (mousebutton == 2) mouse_firecannon = MOUSE_BUTTON_MIDDLE;
  else if (mousebutton == 3) mouse_firecannon = MOUSE_BUTTON_RIGHT;
  else mouse_firecannon = MOUSE_BUTTON_LEFT;

  str = cf->getString (ret, "mouse_firemissile");
  if (str == NULL)
  { mousebutton = 3; }
  else
  { mousebutton = atoi (str); }
  if (mousebutton == 2) mouse_firemissile = MOUSE_BUTTON_MIDDLE;
  else if (mousebutton == 3) mouse_firemissile = MOUSE_BUTTON_RIGHT;
  else mouse_firemissile = MOUSE_BUTTON_LEFT;

  str = cf->getString (ret, "mouse_selectmissile");
  if (str == NULL)
  { mousebutton = 2; }
  else
  { mousebutton = atoi (str); }
  if (mousebutton == 2) mouse_selectmissile = MOUSE_BUTTON_MIDDLE;
  else if (mousebutton == 3) mouse_selectmissile = MOUSE_BUTTON_RIGHT;
  else mouse_selectmissile = MOUSE_BUTTON_LEFT;

  str = cf->getString (ret, "joystick_aileron");
  joystick_aileron = getJoystick (str, 0);

  str = cf->getString (ret, "joystick_elevator");
  joystick_elevator = getJoystick (str, 1);

  str = cf->getString (ret, "joystick_throttle");
  joystick_throttle = getJoystick (str, 2);

  str = cf->getString (ret, "joystick_rudder");
  joystick_rudder = getJoystick (str, 3);

  str = cf->getString (ret, "joystick_firecannon");
  joystick_firecannon = getJoystick (str, 0);

  str = cf->getString (ret, "joystick_firemissile");
  joystick_firemissile = getJoystick (str, 2);

  str = cf->getString (ret, "joystick_dropchaff");
  joystick_dropchaff = getJoystick (str, 3);

  str = cf->getString (ret, "joystick_dropflare");
  joystick_dropflare = getJoystick (str, 3);

  str = cf->getString (ret, "joystick_selectmissile");
  joystick_selectmissile = getJoystick (str, 1);

  str = cf->getString (ret, "joystick_targetnearest");
  joystick_targetnearest = getJoystick (str, 101);

  str = cf->getString (ret, "joystick_targetnext");
  joystick_targetnext = getJoystick (str, 100);

  str = cf->getString (ret, "joystick_targetprevious");
  joystick_targetprevious = getJoystick (str, 102);

  if (cf->buf [0] == 0) // no file found
  {
    delete cf;
    return 0;
  }

  delete cf;
  return 1;
}

#endif
