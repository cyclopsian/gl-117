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



MissionBase::MissionBase ()
{
  id = MISSION_BASE;
  strcpy (name, "BASE");
  strcpy (briefing, "NOW IT IS TIME TO STRIKE AGAINST THE TERRORISTS BASE.");
  autoLFBriefing ();
  alliedfighters = 4;
  alliedpilot [0] = PILOT_PRIMETIME;
  alliedpilot [1] = PILOT_SHADOW;
  alliedpilot [2] = PILOT_HEPTARGON;
  alliedpilot [3] = PILOT_DRDOOM;
  maxtime = 6500 * timestep;
}

void MissionBase::start ()
{
  int i, i2;
  day = 0;
  clouds = 3;
  weather = WEATHER_THUNDERSTORM;
  camera = 0;
  sungamma = 40;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE_EROSION, NULL);
  int px, py;
  l->searchPlain (-1, -1, &px, &py);
  l->flatten (px, py, 8, 8);
  for (i = px - 6; i <= px + 6; i ++)
    for (i2 = py - 8; i2 <= py + 7; i2 ++)
    {
      l->f [i] [i2] = GREYSAND;
      int sum = (int) l->r [i] [i2] + l->g [i] [i2] + l->b [i] [i2];
      sum /= 3;
      l->r [i] [i2] = sum + 15;
      l->g [i] [i2] = sum + 15;
      l->b [i] [i2] = sum - 15;
    }
  playerInit ();
  fplayer->tl.x = px + 10;
  fplayer->tl.z = py + 100;
  fplayer->target = fighter [4];
  for (i = 1; i <= 3; i ++)
  {
    alliedInit (FIGHTER_FALCON, alliedpilot [i - 1], fighter [i]);
    fighter [i]->tl.x = px + 10 + i * 5;
    fighter [i]->tl.z = py + 100 + i * 5;
  }
  int n = 4;
  fighter [n]->tl.x = px - 4;
  fighter [n]->tl.z = py;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall2;
  fighter [n]->newinit (STATIC_HALL2, 0, 400);
  n ++;
  fighter [n]->tl.x = px + 2;
  fighter [n]->tl.z = py + 5;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall2;
  fighter [n]->newinit (STATIC_HALL2, 0, 400);
  n ++;
  fighter [n]->tl.x = px;
  fighter [n]->tl.z = py - 1;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  n ++;
  fighter [n]->tl.x = px + 2;
  fighter [n]->tl.z = py - 1;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  n ++;
  fighter [n]->tl.x = px + 4;
  fighter [n]->tl.z = py - 1;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  n ++;
  fighter [n]->tl.x = px - 1.5;
  fighter [n]->tl.z = py - 4.5;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl.x = px + 1.5;
  fighter [n]->tl.z = py - 4.5;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl.x = px - 3;
  fighter [n]->tl.z = py + 4;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl.x = px - 5.5;
  fighter [n]->tl.z = py + 4;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl.x = px - 4;
  fighter [n]->tl.z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl.x = px - 6;
  fighter [n]->tl.z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->currot.phi = 90;
  fighter [n]->maxrot.theta = 0;
  n ++;
  fighter [n]->tl.x = px - 6;
  fighter [n]->tl.z = py - 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->currot.phi = 0;
  fighter [n]->maxrot.theta = 0;
  n ++;
  fighter [n]->tl.x = px + 6;
  fighter [n]->tl.z = py - 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->currot.phi = 270;
  fighter [n]->maxrot.theta = 0;
  n ++;
  fighter [n]->tl.x = px + 6;
  fighter [n]->tl.z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->currot.phi = 180;
  fighter [n]->maxrot.theta = 0;
  n ++;
  for (i = n; i < n + 6; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [math.random (4)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl.x = px + i * 5 - 60;
    fighter [i]->tl.z = py + i * 5 - 60;
    fighter [i]->newinit (FIGHTER_CROW, 0, 180 + math.random (200));
  }
}

int MissionBase::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 19; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionBase::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif