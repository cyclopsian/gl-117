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



MissionCanyon1::MissionCanyon1 ()
{
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_SHADOW;
  id = MISSION_CANYON1;
  strcpy (name, "RADAR");
  strcpy (briefing, "YOU HAVE TO FLY AN ASSAULT ON A RADAR BASE. STAY AS LOW AS POSSIBLE!");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 2500 * timestep;
}

void MissionCanyon1::start ()
{
  int i, px, py;
  day = 1;
  clouds = 2;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 15;
  heading = 270;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_CANYON, NULL);
  l->searchPlain (-1, -1, &px, &py);
  playerInit ();
  fplayer->tl.x = px + 130;
  fplayer->tl.z = py + 130;
  fplayer->phi = 45;
  fplayer->target = fighter [1];
  fighter [1]->o = &model_egg;
  fighter [1]->newinit (STATIC_COMPLEX1, 0, 0);
  fighter [1]->tl.x = px + 1;
  fighter [1]->tl.z = py + 1;
  fighter [1]->maxthrust = 0;
  fighter [1]->thrust = 0;
  for (i = 2; i < 4; i ++)
  {
    fighter [i]->o = &model_radar;
    fighter [i]->newinit (STATIC_RADAR1, 0, 0);
    fighter [i]->tl.x = px - 2 - (i - 2) * 2;
    fighter [i]->tl.z = py - 2 - (i - 2) * 2;
    fighter [i]->maxthrust = 0;
    fighter [i]->thrust = 0;
  }
  for (i = 4; i <= 10; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_figd;
    fighter [i]->phi = 180;
    fighter [i]->newinit (FIGHTER_BUZZARD, 0, 50 + i * 20);
    fighter [i]->tl.x = px - 15 - i * 3;
    fighter [i]->tl.z = py - 15 - i * 3;
    fighter [i]->deactivate ();
  }
  for (i = 11; i <= 12; i ++)
  {
    fighter [i]->o = &model_flarak1;
    fighter [i]->target = fighter [0];
    fighter [i]->newinit (FLARAK_AIR1, 0, 200);
    fighter [i]->tl.x = px + 4;
    fighter [i]->tl.z = py + i * 3 - 30;
    fighter [i]->maxthrust = 0;
    fighter [i]->thrust = 0;
    fighter [i]->phi = 220;
  }
}

int MissionCanyon1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  if (fplayer->tl.y >= fighter [1]->tl.y && math.dist (fplayer->tl.x - fighter [1]->tl.x, fplayer->tl.z - fighter [1]->tl.z) < 150)
  {
    for (i = 4; i <= 10; i ++)
    {
      if (!fighter [i]->active && fighter [i]->shield > 10)
        fighter [i]->activate ();
    }
  }
  for (i = 0; i <= 3; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionCanyon1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
