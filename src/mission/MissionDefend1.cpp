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
#include "util/Math.h"
#include "configuration/Configuration.h"
#include "gllandscape/GlLandscape.h"

#include <stdio.h>
#include <string.h>



MissionDefend1::MissionDefend1 ()
{
  id = MISSION_DEFEND1;
  strcpy (name, "DEFEND SAM");
  strcpy (briefing, "THERE ARE NEW TERRORIST ACTIVITIES IN NORTHERN AFRICA. OUR SQUARDRON HAS TO MOVE THERE AND OUR FIRST MISSION IS THE DEFENDANCE OF SOME SAM LAUNCHERS AGAINST SUPERIOR TANKS. HOWEVER, OUR OLD HAWKS HAVE BEEN REPLACED.");
  autoLFBriefing ();
  alliedfighters = 2;
  alliedpilot [0] = PILOT_SHADOW;
  maxtime = 3500 * timestep;
  selfighter [1] = Hawk2Descriptor;
}

void MissionDefend1::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 65;
  heading = 200;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_DESERT, NULL);
  playerInit ();
  fplayer->trafo.translation.x = 10;
  fplayer->trafo.translation.z = 90;
  fplayer->target = fighter [7];
  alliedInit (Hawk2Descriptor, alliedpilot [0], 1);
  fighter [1]->trafo.translation.x = 20;
  fighter [1]->trafo.translation.z = 100;
  fighter [1]->target = fighter [8];
  for (i = 2; i <= 6; i ++)
  {
    fighter [i]->trafo.translation.x = 20 + i * 5;
    fighter [i]->trafo.translation.z = 10;
    if (i == 3 || i == 4 || i == 5)
    {
//      fighter [i]->o = &model_flarak1;
      fighter [i]->newinit (SamDescriptor, 0, 400);
      fighter [i]->target = fighter [0];
    }
    else
    {
//      fighter [i]->o = &model_flak1;
      fighter [i]->newinit (SacDescriptor, 0, 400);
      fighter [i]->target = fighter [0];
    }
    fighter [i]->party = 1;
  }
  for (i = 7; i <= 10; i ++)
  {
    int off = 35;
    if (difficulty == 0) off = 55;
    else off = 15;
    fighter [i]->trafo.translation.x = i * 5 - 50;
    fighter [i]->trafo.translation.z = -i * 5 - off;
//    fighter [i]->o = &model_tank2;
    fighter [i]->newinit (PantherDescriptor, 0, 300);
    fighter [i]->target = fighter [i - 4];
  }
  for (i = 11; i <= 15; i ++)
  {
    int off = 40;
    if (difficulty == 0) off = 55;
    else off = 25;
    fighter [i]->trafo.translation.x = i * 5 - 50;
    fighter [i]->trafo.translation.z = -i * 5 - off * 2;
    if (i == 12)
    {
      fighter [i]->newinit (WieselDescriptor, 0, 300);
//      fighter [i]->o = &model_tank1;
    }
    else
    {
      fighter [i]->newinit (PantherDescriptor, 0, 300);
//      fighter [i]->o = &model_tank2;
    }
    fighter [i]->target = fighter [i - 4];
    fighter [i]->deactivate ();
  }
  invertZ (); // only invert if NO objects are mapped to flat ground
}

int MissionDefend1::processtimer (Uint32 dt)
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
  if (!fighter [3]->active || !fighter [4]->active || !fighter [5]->active)
    return 2;
  if (b) return 0;
  return 1;
}

void MissionDefend1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
  if (timer >= 600 * timestep && state == 0)
  {
    state ++;
    fighter [11]->activate ();
    fighter [12]->activate ();
    fighter [13]->activate ();
    fighter [14]->activate ();
    fighter [15]->activate ();
  }
  if (timer >= 600 * timestep && timer <= 800 * timestep)
    font1->drawTextCentered (0, 7, -3, "MORE TANKS ARE ATTACKING", textcolor);
}

#endif
