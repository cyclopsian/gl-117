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



MissionDeathmatch2::MissionDeathmatch2 ()
{
  id = MISSION_DEATHMATCH2;
  strcpy (name, "TEAM DEATHMATCH");
  strcpy (briefing, "THIS IS A SHORT TEAM DEATHMATCH - 8 OPPONENTS IN 4 TEAMS AND 12 KILLS TO WIN. THE DIFFICULTY STILL DETERMINES THE OPPONENTS STRENGTH.");
  autoLFBriefing ();
  alliedfighters = 1;
  selweapons = 1;
  selfighters = 1;
  maxtime = 10000 * timestep;
}
  
void MissionDeathmatch2::start ()
{
  int i;
  day = 1;
  clouds = 2;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 45;
  heading = 220;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->tl.x = 0;
  fplayer->tl.z = 50;
  for (i = 1; i <= 7; i ++)
  {
    if (i <= 1)
    {
      fighter [i]->newinit (FIGHTER_FALCON, 0, 200);
      fighter [i]->o = &model_fig;
    }
    else if (i <= 3)
    {
      fighter [i]->newinit (FIGHTER_CROW, 0, 200);
      fighter [i]->o = &model_fige;
    }
    else if (i <= 5)
    {
      fighter [i]->newinit (FIGHTER_BUZZARD, 0, 200);
      fighter [i]->o = &model_figd;
    }
    else
    {
      fighter [i]->newinit (FIGHTER_SWALLOW, 0, 200);
      fighter [i]->o = &model_figa;
    }
    fighter [i]->party = i / 2 + 1;
    fighter [i]->target = fighter [(i + 4) % 8];
    fighter [i]->tl.x = 50 * SIN(i * 360 / 8);
    fighter [i]->tl.z = 50 * COS(i * 360 / 8);
  }
  state = 0;
  laststate = 0;
  texttimer = 0;
}

int MissionDeathmatch2::processtimer (Uint32 dt)
{
//  bool b = false;
  int i;
  if (texttimer >= 200 * timestep) texttimer = 0;
  if (texttimer > 0) texttimer += dt;
  timer += dt;
  for (i = 0; i <= 3; i ++)
  {
    if (fighter [i * 2]->stat.fighterkills + fighter [i * 2 + 1]->stat.fighterkills >= 12)
    {
//      fplayer->shield = 1;
      if (i == 0) return 1;
      else return 2;
    }
  }
  for (i = 0; i <= 7; i ++)
  {
    if (!fighter [i]->active && fighter [i]->explode >= 35 * timestep)
    {
      fighter [i]->explode = 0;
      int temp = fighter [i]->stat.fighterkills;
      fighter [i]->aiinit ();
      if (i == 0)
      {
        playerInit ();
      }
      else
      {
        fighter [i]->newinit (FIGHTER_FALCON, i + 1, 200);
      }
      fighter [i]->party = i / 2 + 1;
      fighter [i]->shield = fighter [i]->maxshield;
      fighter [i]->immunity = 50 * timestep;
      fighter [i]->activate ();
//      fighter [i]->killed = false;
      fighter [i]->stat.fighterkills = temp;
      fighter [i]->stat.killed = false;
      camera = 0;
    }
  }
  return 0;
}

void MissionDeathmatch2::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif