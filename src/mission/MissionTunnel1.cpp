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



MissionTunnel1::MissionTunnel1 ()
{
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_HAWK2;
  id = MISSION_TUNNEL1;
  strcpy (name, "TUNNEL");
  strcpy (briefing, "THE TERRAIN TO GET NEAR THE ENEMY'S HQ IS GUARDED BY ELITE BUZZARD FIGHTERS. BUT THERE IS A TRENCH WE CAN PROTUDE TO GET NEAR THEIR BASE WITHOUT BEING SIGHTED. FLY AHEAD TO SCOUT THE TRENCH. JUST MAKE YOUR WAY THROUGH!");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 5000 * timestep;
}

void MissionTunnel1::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 60;
  if (l != NULL) delete l;
  l = new GlLandscape (LANDSCAPE_CANYON_TRENCH, NULL);
  playerInit ();
  fplayer->tl.x = 256;
  fplayer->tl.z = 256;
  fplayer->currot.phi = 90;
  fplayer->target = fighter [6];
  for (i = 1; i <= 9; i ++)
  {
    int ix = (i / 2) * 8 - 200 + 256;
    int iy = (i & 1) * 4 - 2 + 256;
    fighter [i]->tl.x = ix;
    fighter [i]->tl.z = iy;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_flak1;
    fighter [i]->newinit (FLAK_AIR1, 0, 200);
  }
  fighter [i]->tl.x = -200 + 256;
  fighter [i]->tl.z = 0 + 256;
  fighter [i]->target = fighter [0];
  fighter [i]->o = &model_flarak1;
  fighter [i]->newinit (FLARAK_AIR1, 0, 200);
  for (i = 11; i <= 15; i ++)
  {
    int ix = (i - 11) * 10 - 100 + 256;
    int iy = (i % 2) * 3 - 3 + 256;
    fighter [i]->tl.x = ix;
    fighter [i]->tl.z = iy;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_flak1;
    fighter [i]->newinit (FLAK_AIR1, 0, 300);
  }
  fighter [i]->tl.x = -250 + 256;
  fighter [i]->tl.z = -2 + 256;
  fighter [i]->target = fighter [0];
  fighter [i]->o = &model_flarak1;
  fighter [i]->newinit (FLARAK_AIR1, 0, 200);
  i ++;
  fighter [i]->tl.x = -250 + 256;
  fighter [i]->tl.z = 2 + 256;
  fighter [i]->target = fighter [0];
  fighter [i]->o = &model_flarak1;
  fighter [i]->newinit (FLARAK_AIR1, 0, 200);
  i ++;
  fighter [i]->tl.x = -350 + 256;
  fighter [i]->tl.z = 0 + 256;
  fighter [i]->o = &model_barrier1;
  fighter [i]->newinit (STATIC_BARRIER1, 0, 100);
  for (i = 19; i < 26; i ++)
  {
    fighter [i]->newinit (FIGHTER_BUZZARD, 0, i * 8);
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_figd;
    fighter [i]->tl.x = 256;
    fighter [i]->tl.z = 256;
    fighter [i]->deactivate ();
  }
}

int MissionTunnel1::processtimer (Uint32 dt)
{
//  bool b = false;
  int i;
  if (timer <= 0) fplayer->tl.y = l->getHeight (fplayer->tl.x, fplayer->tl.z) + 5;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
//  if (timer > 40 * timestep)
  {
    if (fplayer->tl.y - l->getHeight (fplayer->tl.x, fplayer->tl.z) > 15)
    {
      if (!fighter [24]->active && fighter [24]->shield > 0)
      {
        for (i = 19; i < 26; i ++)
        {
          fighter [i]->activate ();
          fighter [i]->tl.x = fplayer->tl.x - 80 - (i - 18) * 8;
          fighter [i]->tl.z = fplayer->tl.z - 20;
          fighter [i]->tl.y = l->getHeight (fighter [i]->tl.x, fighter [i]->tl.z) + 25;
        }
      }
    }
  }
  if (fplayer->tl.x < -450 + 256)
    return 1;
  return 0;
}

void MissionTunnel1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, textcolor);
  }
}

#endif