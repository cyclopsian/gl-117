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
#include "loadmodel/Model3dRegistry.h"

#include <stdio.h>
#include <string.h>



MissionCanyon2::MissionCanyon2 ()
{
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_MATRIX;
  alliedpilot [1] = PILOT_PRIMETIME;
  alliedpilot [2] = PILOT_FIREBIRD;
  alliedpilot [3] = PILOT_LARA;
  id = MISSION_CANYON2;
  strcpy (name, "CANYON BATTLE");
  strcpy (briefing, "THIS MAY BECOME A REAL AIR BATTLE...");
  autoLFBriefing ();
  alliedfighters = 5;
  maxtime = 6500 * timestep;
}

void MissionCanyon2::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 40;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_CANYON, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 100;
  fplayer->trafo.translation.z = 100;
  fplayer->currot.phi = 45;
  fplayer->target = fighter [5];
  for (i = 1; i <= 4; i ++)
  {
    alliedInit (FIGHTER_REDARROW, alliedpilot [i - 1], i);
    if (i % 2)
      fighter [i]->trafo.translation.x = 100 + ((i + 1) / 2) * 5;
    else
      fighter [i]->trafo.translation.x = 100 - ((i + 1) / 2) * 5;
    fighter [i]->trafo.translation.z = 100 + i * 5;
    fighter [i]->target = fighter [5 + i];
    fighter [i]->currot.phi = 45;
  }
  for (i = 5; i <= 22; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [math.random (5)];
    if (i <= 15)
    {
      fighter [i]->trafo.translation.x = -i * 5;
      fighter [i]->trafo.translation.z = -i * 5;
//      fighter [i]->o = &model_fige;
      fighter [i]->newinit (FIGHTER_CROW, 0, math.random (120) + 260);
    }
    else if (i <= 19)
    {
      fighter [i]->trafo.translation.x = -i * 8 - 160;
      fighter [i]->trafo.translation.z = -i * 8 - 160;
//      fighter [i]->o = &model_figd;
      fighter [i]->newinit (FIGHTER_BUZZARD, 0, math.random (120) + 160);
    }
    else
    {
      fighter [i]->trafo.translation.x = -i * 8 - 240;
      fighter [i]->trafo.translation.z = -i * 8 - 240;
//      fighter [i]->o = Model3dRegistry::get ("Swallow");
      fighter [i]->newinit (FIGHTER_SWALLOW, 0, math.random (120) + 200);
    }
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

int MissionCanyon2::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 22; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionCanyon2::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
