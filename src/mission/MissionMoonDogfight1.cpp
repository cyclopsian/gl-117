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



MissionMoonDogfight1::MissionMoonDogfight1 ()
{
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_HAWK2;
  selfighter [2] = FIGHTER_PHOENIX;
  alliedpilot [0] = PILOT_MATRIX;
  id = MISSION_MOON2;
  strcpy (name, "ELITE DOGFIGHT");
  strcpy (briefing, "ENEMY FIGHTERS HAVE APPEARED IN THIS REGION. CAREFUL, THERE MIGHT BE SOME ELITE PILOTS AMONG THEM.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 3000 * timestep;
}

void MissionMoonDogfight1::start ()
{
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_MOON, NULL);
  playerInit ();
  fplayer->tl.x = -100;
  fplayer->tl.z = -100;
  fplayer->currot.phi = 200;
  fplayer->target = fighter [2];
  fighter [1]->tl.x = -110;
  fighter [1]->tl.z = -110;
  fighter [1]->currot.phi = 200;
  fighter [1]->target = fighter [3];
  alliedInit (FIGHTER_REDARROW, alliedpilot [0], fighter [1]);
  fighter [2]->tl.x = 0;
  fighter [2]->tl.z = 0;
  fighter [2]->target = fighter [0];
  fighter [2]->currot.phi = 200;
  fighter [2]->o = &model_figh;
  fighter [2]->newinit (FIGHTER_BLACKBIRD, 0, 150);
  fighter [3]->tl.x = 10;
  fighter [3]->tl.z = 10;
  fighter [3]->target = fighter [1];
  fighter [3]->currot.phi = 200;
  fighter [3]->o = &model_figh;
  fighter [3]->newinit (FIGHTER_BLACKBIRD, 0, 200);
  fighter [4]->tl.x = 200;
  fighter [4]->tl.z = 200;
  fighter [4]->target = fighter [0];
  fighter [4]->currot.phi = 200;
  fighter [4]->o = &model_figh;
  fighter [4]->newinit (FIGHTER_BLACKBIRD, 0, 60);
  fighter [5]->tl.x = 210;
  fighter [5]->tl.z = 210;
  fighter [5]->target = fighter [1];
  fighter [5]->currot.phi = 200;
  fighter [5]->o = &model_figh;
  fighter [5]->newinit (FIGHTER_BLACKBIRD, 0, 140);
  invertZ (); // only invert if NO objects are mapped to flat ground
}

int MissionMoonDogfight1::processtimer (Uint32 dt)
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

void MissionMoonDogfight1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif