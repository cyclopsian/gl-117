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

#ifndef IS_MISSION_H

#include "Mission.h"
#include "game/globals.h"
#include "math/Math.h"
#include "configuration/Configuration.h"
#include "gllandscape/GlLandscape.h"

#include <stdio.h>
#include <string.h>



MissionShip1::MissionShip1 ()
{
  id = MISSION_SHIP1;
  strcpy (name, "DESTROYER");
  strcpy (briefing, "OUR SQUAD HAS BEEN ORDERED INTO THE MEDITERRANEAN. YOUR FIRST GOAL ARE SOME DESTROYERS, GUARDED BY CROWS.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 4000 * timestep;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_SHADOW;
}

void MissionShip1::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_SEA, NULL);
  playerInit ();
  fplayer->tl.x = 80;
  fplayer->tl.z = 0;
  fplayer->currot.phi = 90;
  alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
  fighter [1]->tl.x = 85;
  fighter [1]->tl.z = 5;
  fighter [1]->currot.phi = 90;
  fighter [1]->target = fighter [6];
  for (i = 2; i <= 3; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [i - 2];
    fighter [i]->o = &model_ship2;
    fighter [i]->tl.x = -i * 4;
    fighter [i]->tl.z = -i * 4;
    fighter [i]->newinit (SHIP_DESTROYER1, 0, 50);
    fighter [i]->maxthrust = 0;
    fighter [i]->thrust = 0;
  }
  for (i = 4; i <= 8; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [math.random (2)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl.x = -i * 10;
    fighter [i]->tl.z = 0;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 20);
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

int MissionShip1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 15; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionShip1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif