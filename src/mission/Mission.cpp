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



void Mission::autoLFBriefing ()
{
  int i, z = 0;
  for (i = 0; i < (int) strlen (briefing); i ++)
  {
    z ++;
    if (z > 50 && briefing [i] == ' ')
    {
      briefing [i] = '\n';
      z = 0;
    }
  }
}

Mission::Mission ()
{
  int i;
  timer = 0;
  for (i = 0; i < 3; i ++)
    selfighter [i] = 0;
  selfighter [0] = FIGHTER_FALCON;
  selfighter [1] = FIGHTER_HAWK;
  selfighter [2] = FIGHTER_STORM;
  selfighters = 3;
  wantfighter = 0;
  selweapons = 3;
  selweapon [0] = MISSILE_AIR1;
  selweapon [1] = MISSILE_GROUND1;
  selweapon [2] = MISSILE_DF1;
  wantweapon = 0;
  textcolor.set (255, 255, 0, 180);
  clouds = 0;
  heading = 180;
  state = 0;
}

void Mission::playerInit ()
{
  int i;
  fplayer = fighter [0];
  if (controls != 100)
    fplayer->easymodel = 2;
  fplayer->target = NULL;
  fplayer->newinit (selfighter [wantfighter], 1, 0);
  if (selfighter [wantfighter] == FIGHTER_FALCON) fplayer->o = &model_fig;
  else if (selfighter [wantfighter] == FIGHTER_HAWK) fplayer->o = &model_figb;
  else if (selfighter [wantfighter] == FIGHTER_HAWK2) fplayer->o = &model_figc;
  else if (selfighter [wantfighter] == FIGHTER_PHOENIX) fplayer->o = &model_figf;
  else if (selfighter [wantfighter] == FIGHTER_REDARROW) fplayer->o = &model_figg;
  else if (selfighter [wantfighter] == FIGHTER_BLACKBIRD) fplayer->o = &model_figh;
  else if (selfighter [wantfighter] == FIGHTER_STORM) fplayer->o = &model_figi;
  fplayer->ai = false;
  for (i = 0; i < missiletypes; i ++)
    fplayer->missiles [i] = 0;
  for (i = 0; i < missileracks; i ++)
    fplayer->missilerack [i] = -1;
  if (selweapon [wantweapon] == MISSILE_AIR1)
  {
    if (selfighter [wantfighter] == FIGHTER_FALCON)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 6; fplayer->missilerack [1] = 0; fplayer->missilerack [2] = 0; fplayer->missilerack [3] = 6; }
    if (selfighter [wantfighter] == FIGHTER_HAWK)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 6; fplayer->missilerack [1] = 0; fplayer->missilerack [2] = 0; fplayer->missilerack [3] = 6; }
    if (selfighter [wantfighter] == FIGHTER_STORM)
    { fplayer->missilerackn [0] = 3; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 3;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 6; fplayer->missilerack [2] = 6; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == FIGHTER_HAWK2)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 1; fplayer->missilerack [1] = 6; fplayer->missilerack [2] = 6; fplayer->missilerack [3] = 1; }
    if (selfighter [wantfighter] == FIGHTER_PHOENIX)
    { fplayer->missilerackn [0] = 3; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 3;
      fplayer->missilerack [0] = 2; fplayer->missilerack [1] = 7; fplayer->missilerack [2] = 7; fplayer->missilerack [3] = 2; }
    if (selfighter [wantfighter] == FIGHTER_REDARROW)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 7; fplayer->missilerack [1] = 1; fplayer->missilerack [2] = 1; fplayer->missilerack [3] = 7; }
  }
  if (selweapon [wantweapon] == MISSILE_GROUND1)
  {
    if (selfighter [wantfighter] == FIGHTER_FALCON)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 3; fplayer->missilerack [2] = 3; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == FIGHTER_HAWK)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 6; fplayer->missilerack [1] = 3; fplayer->missilerack [2] = 3; fplayer->missilerack [3] = 6; }
    if (selfighter [wantfighter] == FIGHTER_STORM)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 3; fplayer->missilerack [1] = 3; fplayer->missilerack [2] = 3; fplayer->missilerack [3] = 3; }
    if (selfighter [wantfighter] == FIGHTER_HAWK2)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 6; fplayer->missilerack [1] = 4; fplayer->missilerack [2] = 4; fplayer->missilerack [3] = 6; }
    if (selfighter [wantfighter] == FIGHTER_PHOENIX)
    { fplayer->missilerackn [0] = 3; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 3;
      fplayer->missilerack [0] = 4; fplayer->missilerack [1] = 4; fplayer->missilerack [2] = 4; fplayer->missilerack [3] = 4; }
    if (selfighter [wantfighter] == FIGHTER_REDARROW)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 1; fplayer->missilerack [1] = 4; fplayer->missilerack [2] = 4; fplayer->missilerack [3] = 1; }
  }
  if (selweapon [wantweapon] == MISSILE_DF1)
  {
    if (selfighter [wantfighter] == FIGHTER_FALCON)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 1; fplayer->missilerackn [2] = 1; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == FIGHTER_HAWK)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == FIGHTER_STORM)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 3; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 3; }
    if (selfighter [wantfighter] == FIGHTER_HAWK2)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 1; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 1; }
    if (selfighter [wantfighter] == FIGHTER_PHOENIX)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 4; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 4; }
    if (selfighter [wantfighter] == FIGHTER_REDARROW)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 1; fplayer->missilerackn [2] = 1; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 1; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 1; }
  }
  fplayer->missileCount ();
}

void Mission::alliedInit (int fighterid, int pilotid, AIObj *aiobj)
{
  Pilot *p = pilots->pilot [pilots->aktpilot];
  aiobj->easymodel = 1;
  aiobj->target = NULL;
  int intelligence = p->tp [pilotid]->intelligence;
  int precision = p->tp [pilotid]->precision;
  int aggressivity = p->tp [pilotid]->aggressivity;
  aiobj->newinit (fighterid, 1, intelligence, precision, aggressivity);
  if (fighterid == FIGHTER_FALCON) aiobj->o = &model_fig;
  else if (fighterid == FIGHTER_HAWK) aiobj->o = &model_figb;
  else if (fighterid == FIGHTER_HAWK2) aiobj->o = &model_figc;
  else if (fighterid == FIGHTER_PHOENIX) aiobj->o = &model_figf;
  else if (fighterid == FIGHTER_REDARROW) aiobj->o = &model_figg;
  else if (fighterid == FIGHTER_STORM) aiobj->o = &model_figi;
  aiobj->ai = true;
}

/*void Mission::init ()
{
}

void Mission::start ()
{
}

int Mission::processtimer (Uint32 dt)
{
  return 0;
}

void Mission::draw ()
{
}*/

// invert y values of all fighters for this mission, only possible if no static buildings are placed on flat ground
// missions were designed to fly towards the sun but since v1.0 the rendering coords have been mapped to the OpenGL coords
// therefore a bug was introduced in v1.0 showing the sun from the wrong side
// however this function should let the player fly into the sun in a couple of missions
void Mission::invertZ ()
{
  int i;
  for (i = 0; i < maxfighter; i ++)
  {
    fighter [i]->tl.z = -fighter [i]->tl.z;
    fighter [i]->phi += 180;
  }
}

void Mission::checkScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points)
{
  Pilot *p = pilots->pilot [pilots->aktpilot];
  int i, i2;
  int oldscore = p->mission_score [id]; // getScore (p->mission_state [id], p->mission_time [id], p->mission_fighterkills [id], p->mission_shipkills [id], p->mission_tankkills [id], p->mission_otherkills [id], p->mission_shield [id], p->mission_points [id]);
  int newscore = getScore (missionstate, timebonus, fighterkills, shipkills, tankkills, otherkills, shieldbonus, points);
  if (newscore > oldscore)
  {
    if (missionstate != p->mission_state [id])
    {
      for (i = 0; i < 10; i ++)
      {
        bool b = false;
        for (i2 = 0; i2 < alliedfighters - 1; i2 ++)
        {
          if (i == alliedpilot [i2])
          { b = true; break; }
        }
        if (b)
        {
          p->tp [i]->fighterkills += fighter [i + 1]->fighterkills;
        }
        else
        {
          if (id >= MISSION_CAMPAIGN1 && id <= MISSION_CAMPAIGN2)
            p->tp [i]->flyMission (math.random (4));
        }
      }
    }
    p->mission_state [id] = missionstate;
    p->mission_time [id] = timebonus;
    p->mission_fighterkills [id] = fighterkills;
    p->mission_shipkills [id] = shipkills;
    p->mission_tankkills [id] = tankkills;
    p->mission_otherkills [id] = otherkills;
    p->mission_shield [id] = shieldbonus;
    p->mission_points [id] = points;
    p->mission_score [id] = newscore;
    p->save ();
  }
}

int Mission::getScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points)
{
  if (points > 2000) points = 2000;
  int score = timebonus + shieldbonus + fighterkills * 20 + shipkills * 12 + tankkills * 10 + otherkills * 5/* + points / 20*/;
  if (difficulty == 0) score -= 25;
  else if (difficulty == 1) score += 25;
  else if (difficulty == 2) score += 75;
  if (!fplayer->realism) score -= 25;
  else score += 25;
  if (missionstate == 2) // if failed
  {
    score = 0;
  }
  return score;
}

int Mission::getScore (int missionstate)
{
  int shieldbonus = (int) (fplayer->shield * 100 / fplayer->maxshield);
  int timebonus = 0;
  if (timer < maxtime)
    timebonus = (maxtime - timer) * 100 / maxtime;
  checkScore (missionstate, timebonus, fplayer->fighterkills, fplayer->shipkills, fplayer->tankkills, fplayer->otherkills, shieldbonus, fplayer->points);
  return getScore (missionstate, timebonus, fplayer->fighterkills, fplayer->shipkills, fplayer->tankkills, fplayer->otherkills, shieldbonus, fplayer->points);
}

#endif
