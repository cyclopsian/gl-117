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
//#include "configuration/Dirs.h"
#include "logging/Logging.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



TeamPilot::TeamPilot (int ranking, char *name, int intelligence, int precision, int aggressivity, int fighterkills)
{
  this->ranking = ranking;
  strcpy (this->name, name);
  this->intelligence = intelligence;
  this->precision = precision;
  this->aggressivity = aggressivity;
  this->fighterkills = fighterkills;
}

void TeamPilot::flyMission (int averagekills)
{
  float f = (float) (intelligence + precision + aggressivity);
  f /= 1000;
  f = 1.0F - f;
  f *= (float) averagekills;
  if (f < 0) f = 0;
  fighterkills += (int) f;
}

char *TeamPilot::getRank ()
{
  if (ranking == 0) return "AIRMAN BASIC";
  else if (ranking == 1) return "AIRMAN";
  else if (ranking == 2) return "AIRMAN 1ST CLASS";
  else if (ranking == 3) return "2ND LIEUTNANT";
  else if (ranking == 4) return "1ST LIEUTNANT";
  else if (ranking == 5) return "CAPTAIN";
  else if (ranking == 6) return "MAJOR";
  else if (ranking == 7) return "COLONEL";
  else return "GENERAL";
}

char *TeamPilot::getShortRank ()
{
  if (ranking == 0) return "AB";
  else if (ranking == 1) return "ARM";
  else if (ranking == 2) return "A1C";
  else if (ranking == 3) return "2ND LT";
  else if (ranking == 4) return "1ST LT";
  else if (ranking == 5) return "CAPT";
  else if (ranking == 6) return "MAJ";
  else if (ranking == 7) return "COL";
  else return "GEN";
}

char *TeamPilot::getName ()
{
  strcpy (fullname, getRank ());
  strcat (fullname, " ");
  strcat (fullname, name);
  return fullname;
}

char *TeamPilot::getShortName ()
{
  strcpy (fullname, getShortRank ());
  strcat (fullname, " ");
  strcat (fullname, name);
  return fullname;
}

void TeamPilot::load (FILE *in)
{
  fread (&ranking, sizeof (int), 1, in);
  int len;
  fread (&len, sizeof (int), 1, in);
  if (len > 50) return;
  fread (name, sizeof (char), len, in);
  fread (&intelligence, sizeof (int), 1, in);
  fread (&precision, sizeof (int), 1, in);
  fread (&aggressivity, sizeof (int), 1, in);
  fread (&fighterkills, sizeof (int), 1, in);
}

void TeamPilot::save (FILE *out)
{
  fwrite (&ranking, sizeof (int), 1, out);
  int len = strlen (name);
  fwrite (&len, sizeof (int), 1, out);
  fwrite (name, sizeof (char), len, out);
  fwrite (&intelligence, sizeof (int), 1, out);
  fwrite (&precision, sizeof (int), 1, out);
  fwrite (&aggressivity, sizeof (int), 1, out);
  fwrite (&fighterkills, sizeof (int), 1, out);
}

#endif
