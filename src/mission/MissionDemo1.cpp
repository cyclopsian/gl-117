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



MissionDemo1::MissionDemo1 ()
{
  id = MISSION_DEMO;
  strcpy (name, "DEMO");
  alliedfighters = 2;
}

void MissionDemo1::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 5;
  sungamma = 45;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE, NULL);
  fplayer = fighter [0];
  fighter [0]->tl.x = 220;
  fighter [0]->tl.z = -30;
  fighter [0]->o = &model_fig;
  fighter [0]->target = fighter [2];
  fighter [0]->newinit (FIGHTER_FALCON, 1, 0);
  fighter [1]->target = fighter [3];
  fighter [1]->o = &model_fig;
  fighter [1]->tl.x = 49;
  fighter [1]->tl.z = -30;
  fighter [1]->newinit (FIGHTER_FALCON, 1, 0);
  fighter [1]->aggressivity = 300;
  for (i = 2; i <= 8; i ++)
  {
    fighter [i]->target = fighter [math.random (2)];
    fighter [i]->o = &model_figa;
    fighter [i]->newinit (FIGHTER_SWALLOW, 0, 400 - i * 20);
  }
}

int MissionDemo1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 8; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionDemo1::draw ()
{
}

#endif