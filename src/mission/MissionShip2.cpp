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



MissionShip2::MissionShip2 ()
{
  id = MISSION_SHIP2;
  strcpy (name, "OILRIG");
  strcpy (briefing, "AN OILRIG IS BEING ATTACKED BY ENEMY FORCES. DEFEND! OUR NEW SUPERIOR FIGHTER GL-15 IS NOW AVAILABLE.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 2500 * timestep;
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_SHADOW;
}

void MissionShip2::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 15;
  heading = 270;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_SEA, NULL);
  playerInit ();
  fplayer->tl.x = 80;
  fplayer->tl.z = 0;
  fplayer->currot.phi = 90;
  alliedInit (FIGHTER_REDARROW, alliedpilot [0], fighter [1]);
  fighter [1]->tl.x = 85;
  fighter [1]->tl.z = 5;
  fighter [1]->currot.phi = 90;
  fighter [1]->target = fighter [6];
  fighter [2]->o = &model_oilrig;
  fighter [2]->tl.x = 20;
  fighter [2]->tl.z = 0;
  fighter [2]->newinit (STATIC_OILRIG1, 0, 0);
  fighter [2]->maxthrust = 0;
  fighter [2]->thrust = 0;
  fighter [2]->party = 1;
  for (i = 3; i <= 5; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [math.random (2)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl.x = -50 - i * 10;
    fighter [i]->tl.z = 0;
    fighter [i]->newinit (FIGHTER_CROW, 0, 300 - i * 10);
  }
  for (i = 6; i <= 7; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [2];
    fighter [i]->o = &model_figa;
    fighter [i]->tl.x = -80 - i * 10;
    fighter [i]->tl.z = 0;
    fighter [i]->newinit (FIGHTER_SWALLOW, 0, 160);
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

int MissionShip2::processtimer (Uint32 dt)
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
  if (!fighter [2]->active)
    return 2;
  if (b) return 0;
  return 1;
}

void MissionShip2::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif