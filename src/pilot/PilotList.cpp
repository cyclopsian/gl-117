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

#ifndef IS_PILOTS_H

#include "pilot/Pilot.h"
#include "configuration/Dirs.h"
#include "logging/Logging.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



void PilotList::load (char *fname)
{
  char buf [16];
  int i;
  FILE *in = fopen (fname, "rb");
  if (in == NULL)
  {
    logging.display ("Could not load saves/pilots", LOG_WARN);
    aktpilots = 0;
    add ("PLAYER");
    return;
  }
  fgets (buf, 16, in);
  aktpilots = atoi (buf);
  fgets (buf, 16, in);
  aktpilot = atoi (buf);
  int z = 0;
  while (fgets (buf, 16, in) && z < maxpilots)
  {
    for (i = 0; i < (int) strlen (buf); i ++)
      if (buf [i] == '\n')
        buf [i] = '\0';
    pilot [z] = new Pilot (buf);
    z ++;
  }
  fclose (in);
}

void PilotList::save (char *fname)
{
  char buf [256];
  int i;
  
  FILE *out = fopen (fname, "wb");
  if (out == NULL)
  {
    logging.display ("Could not write saves/pilots", LOG_WARN);
    return;
  }
  sprintf (buf, "%d\n%d\n", aktpilots, aktpilot);
  fwrite (buf, 1, strlen (buf), out);
  for (i = 0; i < aktpilots; i ++)
  {
    sprintf (buf, "%s\n", pilot [i]->name);
    fwrite (buf, 1, strlen (buf), out);
  }
  fclose (out);
  
  for (i = 0; i < aktpilots; i ++)
  {
    pilot [i]->save ();
  }
}

PilotList::PilotList (char *fname)
{
  int i;
  for (i = 0; i < maxpilots; i ++)
    pilot [i] = NULL;
  load (fname);
}

PilotList::~PilotList ()
{
  int i;
  for (i = 0; i < aktpilots; i ++)
  {
    if (pilot [i] != NULL)
      delete pilot [i];
  }
}

void PilotList::rm ()
{
  int i, z = 0;
  for (i = 0; i < maxpilots; i ++)
  {
    if (pilot [i] != NULL) z ++;
  }
  if (z <= 1) return;
  if (pilot [aktpilot] != NULL)
  {
    char buf [4096];
    strcpy (buf, dirs.getSaves (pilot [aktpilot]->name));
    remove (buf);
    delete pilot [aktpilot];
    pilot [aktpilot] = NULL;
  }
  for (i = aktpilot; i < aktpilots; i ++)
    pilot [i] = pilot [i + 1];
  pilot [aktpilots] = NULL;
  aktpilots --;
  if (aktpilots < 0) aktpilots = 0;
  aktpilot = 0;
}

void PilotList::add (char *name)
{
  if (aktpilots >= maxpilots) return;
  aktpilot = aktpilots;
  pilot [aktpilot] = new Pilot (name);
  aktpilots ++;
}

#endif
