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



MissionDogfight2::MissionDogfight2 ()
{
  id = MISSION_DOGFIGHT2;
  strcpy (name, "DOGFIGHT");
  strcpy (briefing, "SOME ENEMY FIGHTERS ARE ON THEIR WAY ATTACKING ONE OF OUR OUTPOSTS. STOP THEM BEFORE THEY CAN REACH IT!");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 5000 * timestep;
  alliedpilot [0] = PILOT_PRIMETIME;
//  alliedpilot [1] = PILOT_SHADOW;
}

void MissionDogfight2::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 40;
  heading = 200;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->tl.x = 0;
  fplayer->tl.z = 100;
  alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
  fighter [1]->tl.x = 5;
  fighter [1]->tl.z = 105;
  for (i = 2; i <= 6; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [math.random (2)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl.x = -i * 10;
    fighter [i]->tl.z = -i * 10;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 10);
  }
  for (i = 7; i <= 8; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [math.random (2)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl.x = -i * 10 - 100;
    fighter [i]->tl.z = -i * 10 - 100;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 20);
    fighter [i]->deactivate ();
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

int MissionDogfight2::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 7; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionDogfight2::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
  if (timer >= 1000 * timestep && state == 0)
  {
    state ++;
    fighter [7]->activate ();
    fighter [8]->activate ();
    fighter [7]->tl.x = fplayer->tl.x + 50;
    fighter [7]->tl.z = fplayer->tl.z + 50;
    fighter [7]->tl.y = l->getHeight (fighter [7]->tl.x, fighter [7]->tl.z) + 10;
    fighter [8]->tl.x = fplayer->tl.x + 60;
    fighter [8]->tl.z = fplayer->tl.z + 60;
    fighter [8]->tl.y = l->getHeight (fighter [8]->tl.x, fighter [8]->tl.z) + 10;
  }
  if (timer >= 1000 * timestep && timer <= 1200 * timestep)
    font1->drawTextCentered (0, 7, -3, "MORE ENEMIES ENTERING THE REGION", textcolor);
}

#endif