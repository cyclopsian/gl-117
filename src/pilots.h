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

/* This file handles all pilots data. */

#ifndef IS_PILOTS_H
#define IS_PILOTS_H

#include <stdio.h>
#include <string.h>

#include "dirs.h"

#define SAVEVERSION "GL117_S1"

const int maxpilotdata = 100;

#define PILOT_PRIMETIME 0
#define PILOT_HEPTARGON 1
#define PILOT_LARA 2
#define PILOT_SHARK 3
#define PILOT_BOSS 4
#define PILOT_DRDOOM 5
#define PILOT_SHADOW 6
#define PILOT_MATRIX 7
#define PILOT_FIREBIRD 8
#define PILOT_THUNDER 9

class TeamPilot
{
  public:
  int ranking;
  char name [100];
  char fullname [100];
  int intelligence, precision, aggressivity;
  int fighterkills;
  TeamPilot (int ranking, char *name, int intelligence, int precision, int aggressivity, int fighterkills);
  void flyMission (int averagekills);
  char *getRank ();
  char *getName ();
  void load (FILE *in);
  void save (FILE *out);
};

class Pilot
{
  public:
  char name [16];
  int mission_state [maxpilotdata];
  int mission_time [maxpilotdata];
  int mission_fighterkills [maxpilotdata];
  int mission_shipkills [maxpilotdata];
  int mission_tankkills [maxpilotdata];
  int mission_otherkills [maxpilotdata];
  int mission_shield [maxpilotdata];
  int mission_points [maxpilotdata];
  int mission_score [maxpilotdata];
  int ranking;
  TeamPilot **tp;

  void load ();
  void save ();
  char *getRank ();
  Pilot (char *name);
};

const int maxpilots = 5;

class PilotList
{
  public:
  int aktpilots, aktpilot;
  Pilot *pilot [maxpilots];

  void load (char *fname);
  void save (char *fname);
  PilotList (char *fname);
  ~PilotList ();
  void rm ();
  void add (char *name);
};

#endif
