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
//#include "loadmodel/Model3dFactory.h"

#include <stdio.h>
#include <string.h>



MissionTeamBase1::MissionTeamBase1 ()
{
  id = MISSION_TEAMBASE1;
  strcpy (name, "TEAM BASE");
  strcpy (briefing, "ATTACK YOUR OPPONENTS BASE AND DEFEND YOUR OWN ONE. TWO TEAMS - ONE FIGHTER AND ONE BOMBER EACH.");
  autoLFBriefing ();
  alliedfighters = 1;
  selweapons = 2;
  maxtime = 10000 * timestep;
}
  
void MissionTeamBase1::start ()
{
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  heading = 180;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE_EROSION, NULL);
  int px, py;
  l->searchPlain (1, 1, &px, &py);
  l->flatten (px, py, 8, 8);
  team1x = px; team1y = py + 50;
  playerInit ();
  fplayer->trafo.translation.x = px;
  fplayer->trafo.translation.z = py;
  fplayer->currot.phi = 180;
  if (fplayer->id == FIGHTER_FALCON)
  {
    fighter [1]->newinit (FIGHTER_HAWK, 0, 200);
    fighter [1]->target = NULL;
  }
  else
  {
    fighter [1]->newinit (FIGHTER_FALCON, 0, 200);
    fighter [1]->target = fighter [3];
  }
  fighter [1]->party = 1;
  fighter [1]->trafo.translation.x = px + 5;
  fighter [1]->trafo.translation.z = py + 5;

  int n = 4;
  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py + 5;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_hall2;
  fighter [n]->newinit (STATIC_HALL2, 0, 400);
  fighter [n]->party = 1;
  n ++;
  fighter [n]->trafo.translation.x = px;
  fighter [n]->trafo.translation.z = py - 1;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  fighter [n]->party = 1;
  n ++;
  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py - 1;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  fighter [n]->party = 1;
  n ++;
  fighter [n]->trafo.translation.x = px - 1.5;
  fighter [n]->trafo.translation.z = py - 4.5;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  fighter [n]->party = 1;
  n ++;
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py + 6;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->currot.phi = 90;
  fighter [n]->maxrot.theta = 0;
  fighter [n]->party = 1;
  n ++;
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py - 6;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->currot.phi = 0;
  fighter [n]->maxrot.theta = 0;
  fighter [n]->party = 1;
  n ++;
  fighter [n]->trafo.translation.x = px - 15;
  fighter [n]->trafo.translation.z = py - 20;
  fighter [n]->target = NULL;
//  fighter [n]->o = &model_flarak1;
  fighter [n]->newinit (FLARAK_AIR1, 0, 300);
  fighter [n]->party = 1;
  n ++;
  fighter [n]->trafo.translation.x = px + 0;
  fighter [n]->trafo.translation.z = py - 20;
  fighter [n]->target = NULL;
//  fighter [n]->o = &model_flarak1;
  fighter [n]->newinit (FLARAK_AIR1, 0, 300);
  fighter [n]->party = 1;
  n ++;
  fighter [n]->trafo.translation.x = px + 15;
  fighter [n]->trafo.translation.z = py - 20;
  fighter [n]->target = NULL;
//  fighter [n]->o = &model_flarak1;
  fighter [n]->newinit (FLARAK_AIR1, 0, 300);
  fighter [n]->party = 1;

  l->searchPlain (1, 2, &px, &py);
  l->flatten (px, py, 8, 8);
  team2x = px; team2y = py - 50;
  fighter [2]->newinit (FIGHTER_BUZZARD, 0, 200);
//  fighter [2]->o = &model_figd;
  fighter [2]->trafo.translation.x = px;
  fighter [2]->trafo.translation.z = py;
  if (fplayer->id == FIGHTER_FALCON)
    fighter [2]->target = fighter [1];
  else
    fighter [2]->target = fighter [0];
  fighter [2]->party = 2;
  fighter [3]->newinit (FIGHTER_SWALLOW, 0, 200);
//  fighter [3]->o = Model3dRegistry::get ("Swallow");
  fighter [3]->trafo.translation.x = px + 5;
  fighter [3]->trafo.translation.z = py + 5;
  fighter [3]->target = NULL;
  fighter [3]->party = 2;

  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py + 5;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_hall2;
  fighter [n]->newinit (STATIC_HALL2, 0, 400);
  fighter [n]->party = 2;
  n ++;
  fighter [n]->trafo.translation.x = px;
  fighter [n]->trafo.translation.z = py - 1;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  fighter [n]->party = 2;
  n ++;
  fighter [n]->trafo.translation.x = px + 2;
  fighter [n]->trafo.translation.z = py - 1;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  fighter [n]->party = 2;
  n ++;
  fighter [n]->trafo.translation.x = px - 1.5;
  fighter [n]->trafo.translation.z = py - 4.5;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  fighter [n]->party = 2;
  n ++;
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py + 6;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->currot.phi = 90;
  fighter [n]->maxrot.theta = 0;
  fighter [n]->party = 2;
  n ++;
  fighter [n]->trafo.translation.x = px - 6;
  fighter [n]->trafo.translation.z = py - 6;
  fighter [n]->target = fighter [0];
//  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->currot.phi = 0;
  fighter [n]->maxrot.theta = 0;
  fighter [n]->party = 2;
  n ++;
  fighter [n]->trafo.translation.x = px - 15;
  fighter [n]->trafo.translation.z = py + 20;
  fighter [n]->target = NULL;
//  fighter [n]->o = &model_flarak1;
  fighter [n]->newinit (FLARAK_AIR1, 0, 300);
  fighter [n]->party = 2;
  n ++;
  fighter [n]->trafo.translation.x = px + 0;
  fighter [n]->trafo.translation.z = py + 20;
  fighter [n]->target = NULL;
//  fighter [n]->o = &model_flarak1;
  fighter [n]->newinit (FLARAK_AIR1, 0, 300);
  fighter [n]->party = 2;
  n ++;
  fighter [n]->trafo.translation.x = px + 15;
  fighter [n]->trafo.translation.z = py + 20;
  fighter [n]->target = NULL;
//  fighter [n]->o = &model_flarak1;
  fighter [n]->newinit (FLARAK_AIR1, 0, 300);
  fighter [n]->party = 2;

  state = 0;
  laststate = 0;
  texttimer = 0;
}

int MissionTeamBase1::processtimer (Uint32 dt)
{
//  bool b = false;
  int i;
  if (texttimer >= 200 * timestep) texttimer = 0;
  if (texttimer > 0) texttimer += dt;
  timer += dt;

  if (fplayer->id == FIGHTER_FALCON)
  {
    fighter [2]->target = fighter [1];
  }
  else
  {
    fighter [2]->target = fighter [0];
    fighter [1]->target = fighter [3];
  }

  bool testb1 = false, testb2 = false;
  for (i = 4; i <= 29; i ++)
  {
    if (fighter [i]->active)
    {
      if (fighter [i]->party == 1)
        testb1 = true;
      if (fighter [i]->party == 2)
        testb2 = true;
    }
  }
  if (!testb1) return 2;
  if (!testb2) return 1;
  
  for (i = 0; i <= 3; i ++)
  {
    if (!fighter [i]->active && fighter [i]->explode >= 35 * timestep)
    {
      fighter [i]->explode = 0;
      int temp = fighter [i]->stat.fighterkills;
      int tempid = fighter [i]->id;
      fighter [i]->init ();
      if (i == 0)
      {
        playerInit ();
      }
      else
      {
        fighter [i]->newinit (tempid, 0, 200);
      }
      fighter [i]->party = i / 2 + 1;
      fighter [i]->shield = fighter [i]->maxshield;
      fighter [i]->immunity = 50 * timestep;
      fighter [i]->activate ();
//      fighter [i]->killed = false;
      fighter [i]->stat.fighterkills = temp;
      fighter [i]->stat.killed = false;
      if (i <= 1)
      {
        fighter [i]->trafo.translation.x = team1x;
        fighter [i]->trafo.translation.z = team1y;
      }
      else
      {
        fighter [i]->trafo.translation.x = team2x;
        fighter [i]->trafo.translation.z = team2y;
      }
      camera = 0;
    }
  }
  return 0;
}

void MissionTeamBase1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif
