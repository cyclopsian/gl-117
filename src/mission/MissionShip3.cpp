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



MissionShip3::MissionShip3 ()
{
  id = MISSION_SHIP3;
  strcpy (name, "CRUISER ATTACK");
  strcpy (briefing, "THE CRUISER C-15 ARAKON HAS ENTERED NEUTRAL TERRITORY WATERS AND MUST BE ELIMINATED. BEWARE OF HIS FRONT MISSILE LAUNCHER.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 3000 * timestep;
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_SHADOW;
}

void MissionShip3::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_SEA, NULL);
  playerInit ();
  fplayer->tl.x = 80;
  fplayer->tl.z = 0;
  fplayer->phi = 90;
  alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
  fighter [1]->tl.x = 85;
  fighter [1]->tl.z = 5;
  fighter [1]->phi = 90;
  fighter [1]->target = fighter [6];
  fighter [2]->party = 0;
  fighter [2]->target = fighter [0];
  fighter [2]->o = &model_ship1;
  fighter [2]->tl.x = -20;
  fighter [2]->tl.z = 0;
  fighter [2]->newinit (SHIP_CRUISER, 0, 200);
  fighter [2]->maxthrust = 0;
  fighter [2]->thrust = 0;
  for (i = 4; i <= 7; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [math.random (2)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl.x = -i * 5;
    fighter [i]->tl.z = 0;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 10);
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

int MissionShip3::processtimer (Uint32 dt)
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

void MissionShip3::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
