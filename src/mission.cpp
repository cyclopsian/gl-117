/*
    GL-117
    Copyright 2001, 2002 Thomas A. Drexl aka heptargon

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

/* This file contains all mission data and definitions. */

#ifndef IS_MISSION_H

#include "mission.h"

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
  selfighter [2] = FIGHTER_HAWK2;
  selfighters = 2;
  wantfighter = 0;
  selweapons = 3;
  selweapon [0] = MISSILE_AIR1;
  selweapon [1] = MISSILE_GROUND1;
  selweapon [2] = MISSILE_DF1;
  wantweapon = 0;
  textcolor.setColor (255, 255, 0, 128);
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
  fplayer->ai = false;
  for (i = 0; i < missiletypes; i ++)
    fplayer->missiles [i] = 0;
  for (i = 0; i < missileracks; i ++)
    fplayer->missilerack [i] = -1;
  if (selweapon [wantweapon] == MISSILE_AIR1)
  {
    if (selfighter [wantfighter] == FIGHTER_FALCON)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 2; fplayer->missilerackn [2] = 2; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 6; fplayer->missilerack [2] = 6; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == FIGHTER_HAWK)
    { fplayer->missilerackn [0] = 3; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 3;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 6; fplayer->missilerack [2] = 6; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == FIGHTER_HAWK2)
    { fplayer->missilerackn [0] = 3; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 3;
      fplayer->missilerack [0] = 0; fplayer->missilerack [1] = 6; fplayer->missilerack [2] = 6; fplayer->missilerack [3] = 0; }
    if (selfighter [wantfighter] == FIGHTER_PHOENIX)
    { fplayer->missilerackn [0] = 3; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 3;
      fplayer->missilerack [0] = 2; fplayer->missilerack [1] = 7; fplayer->missilerack [2] = 7; fplayer->missilerack [3] = 2; }
    if (selfighter [wantfighter] == FIGHTER_REDARROW)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
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
      fplayer->missilerack [0] = 5; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 5; }
    if (selfighter [wantfighter] == FIGHTER_HAWK)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 3; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 3; }
    if (selfighter [wantfighter] == FIGHTER_HAWK2)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 4; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 4; }
    if (selfighter [wantfighter] == FIGHTER_PHOENIX)
    { fplayer->missilerackn [0] = 2; fplayer->missilerackn [1] = 3; fplayer->missilerackn [2] = 3; fplayer->missilerackn [3] = 2;
      fplayer->missilerack [0] = 4; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 4; }
    if (selfighter [wantfighter] == FIGHTER_REDARROW)
    { fplayer->missilerackn [0] = 1; fplayer->missilerackn [1] = 1; fplayer->missilerackn [2] = 1; fplayer->missilerackn [3] = 1;
      fplayer->missilerack [0] = 5; fplayer->missilerack [1] = 5; fplayer->missilerack [2] = 5; fplayer->missilerack [3] = 5; }
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
  aiobj->ai = true;
}

void Mission::init ()
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
        if (b) p->tp [i]->fighterkills += fighter [i + 1]->fighterkills;
        else p->tp [i]->flyMission (myrandom (5));
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
  int score = timebonus + shieldbonus + fighterkills * 20 + shipkills * 12 + tankkills * 10 + otherkills * 5 + points / 20;
  if (difficulty == 0) score -= 25;
  else if (difficulty == 1) score += 25;
  else if (difficulty == 2) score += 75;
  if (missionstate == 2) // if failed
  {
    score /= 4;
    score -= 100;
  }
  return score;
}

int Mission::getScore (int missionstate)
{
  int shieldbonus = fplayer->shield * 100 / fplayer->maxshield;
  int timebonus = 0;
  if (timer < maxtime)
    timebonus = (maxtime - timer) * 100 / maxtime;
  checkScore (missionstate, timebonus, fplayer->fighterkills, fplayer->shipkills, fplayer->tankkills, fplayer->otherkills, shieldbonus, fplayer->points);
  return getScore (missionstate, timebonus, fplayer->fighterkills, fplayer->shipkills, fplayer->tankkills, fplayer->otherkills, shieldbonus, fplayer->points);
}



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
  l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
  fplayer = fighter [0];
  fplayer->tl->x = 220;
  fplayer->tl->z = -30;
  fplayer->target = fighter [2];
  fplayer->newinit (FIGHTER_FALCON, 1, 0);
  fighter [1]->target = fighter [3];
  fighter [1]->o = &model_fig;
  fighter [1]->tl->x = 49;
  fighter [1]->tl->z = -30;
  fighter [1]->newinit (FIGHTER_FALCON, 1, 0);
  for (i = 2; i <= 8; i ++)
  {
    fighter [i]->target = fighter [myrandom (2)];
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



MissionTutorial1::MissionTutorial1 ()
{
  id = MISSION_TUTORIAL;
  strcpy (name, "TUTORIAL");
  alliedfighters = 1;
  strcpy (briefing, "WELCOME TO THE EAGLE SQUADRON! BEFORE FLYING ANY MISSION YOU SHOULD PLAY THIS TUTORIAL. LEARN TO HANDLE YOUR FIGHTER AND THE BASIC CONTROLS LIKE TARGETING OR FIRING A MISSILE.");
  autoLFBriefing ();
  maxtime = 2500 * timestep;
  heading = 210;
}

void MissionTutorial1::start ()
{
  day = 1;
  clouds = 2;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_LOW_ALPINE, NULL);
//    l = new GLLandscape (space, LANDSCAPE_CANYON, NULL);
  playerInit ();
  fplayer->tl->x = 220;
  fplayer->tl->z = -30;
  loop = false;
}

int MissionTutorial1::processtimer (Uint32 dt)
{
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
    return 2;
  if (!fighter [1]->active && fighter [1]->explode >= 35 && !fighter [2]->active && fighter [2]->explode >= 35)
    return 1;
  return 0;
}

void MissionTutorial1::draw ()
{
  char buf [250], buf2 [10];
  int timeroff = 100 * timestep, timerdelay = 300 * timestep, timerlag = 20 * timestep;
  if (timer >= 0 && timer <= timeroff - 20)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
  else if (timer > timeroff && timer <= timeroff + timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "LEAVE THE JOYSTICK CENTERED", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "THE FIGHTER SHOULD FLY STRAIGHT ON", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "IF NOT, RECALIBRATE YOUR JOYSTICK", &textcolor);
    }
    else if (controls == CONTROLS_MOUSE || controls == CONTROLS_MOUSE_REVERSE || controls == CONTROLS_MOUSE_EXP)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE MENU OPTIONS/CONTROLS LETS YOU", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "DETERMINE THE INPUT DEVICE.", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "CURRENTLY, THIS IS THE MOUSE", &textcolor);
    }
    else if (controls == CONTROLS_KEYBOARD)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE MENU OPTIONS/CONTROLS LETS YOU", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "DETERMINE THE INPUT DEVICE.", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "CURRENTLY, THIS IS THE KEYBOARD", &textcolor);
    }
/*      if (!loop) fplayer->gamma += 2;
    else fplayer->gamma -= 2;
    if (fplayer->checkLooping ()) loop = !loop;*/
  }
  else if (timer > timeroff + timerdelay && timer <= timeroff + 2 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "MOVING THE JOYSTICK LEFT OR RIGHT", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "WILL AFFECT THE AILERON", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THUS YOU WILL ONLY ROLL", &textcolor);
    }
    else if (controls == CONTROLS_MOUSE)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE MOUSE INTERFACE IS VERY EASY", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "ALWAYS POINT TO WHERE YOU WANT TO FLY", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "YOU'LL HAVE TO MOVE THE MOUSE PERMANENTLY", &textcolor);
    }
    else if (controls == CONTROLS_KEYBOARD)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE KEYBOARD INTERFACE IS NOT SO EASY", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "USING THE MOUSE IS STRONLY RECOMMENDED", &textcolor);
    }
    else if (controls == CONTROLS_MOUSE_REVERSE)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE REVERTED MOUSE IS FOR REAL GAMERS ONLY", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "THE STANDARD MOUSE INTERFACE MAY BE", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "EASIER TO LEARN", &textcolor);
    }
    else if (controls == CONTROLS_MOUSE_EXP)
    {
      font1->drawTextCentered (0, 7, -2.5, "MOUSE RELATIVE IS A LOT OF WORK", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "ONLY THE RELATIVE MOUSE COORDINATES (MOVING)", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "WILL HAVE AN EFFECT", &textcolor);
    }
  }
  else if (timer > timeroff + 2 * timerdelay && timer <= timeroff + 3 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "MOVING UP OR DOWN WILL AFFECT", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "THE ELEVATOR", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "YOU'LL START TO FLY A LOOP", &textcolor);
    }
    else if (controls == CONTROLS_MOUSE || controls == CONTROLS_KEYBOARD || controls == CONTROLS_MOUSE_REVERSE || controls == CONTROLS_MOUSE_EXP)
    {
      font1->drawTextCentered (0, 7, -2.5, "MOVING RIGHT OR LEFT WILL AFFECT THE AILERON", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "MOVING UP OR DOWN WILL AFFECT THE ELEVATOR", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THUS YOU CAN FLY ROLLS AND LOOPS", &textcolor);
    }
  }
  else if (timer > timeroff + 3 * timerdelay && timer <= timeroff + 4 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "IF YOUR JOYSTICK HAS A SO-CALLED \"RUDDER\",", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "YOU MAY ALTER THE FIGHTER'S RUDDER", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "YOU'LL SLIGHTLY FLY TO THE LEFT OR RIGHT", &textcolor);
    }
    else if (controls == CONTROLS_MOUSE || controls == CONTROLS_MOUSE_REVERSE || controls == CONTROLS_MOUSE_EXP)
    {
      font1->drawTextCentered (0, 7, -2.5, "MOVING THE MOUSE SLIGHTLY LEFT OR RIGHT", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "WILL AFFECT THE RUDDER.", &textcolor);
    }
    else if (controls == CONTROLS_KEYBOARD)
    {
      font1->drawTextCentered (0, 7, -2.5, "TO ALTER THE RUDDER", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "USE THE KEYS PGUP AND PGDN", &textcolor);
    }
  }
  else if (timer > timeroff + 4 * timerdelay && timer <= timeroff + 5 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "YOUR JOYSTICK HAS ALSO A THROTTLE", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "TO SPEEDUP OR DECELERATE", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THERE IS NO WAY TO ENTIRELY STOP", &textcolor);
    }
    else
    {
      font1->drawTextCentered (0, 7, -2.5, "USE THE KEYS '1' THROUGH '9' TO ALTER", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "YOUR THROTTLE (SPEED)", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THERE IS NO WAY TO ENTIRELY STOP", &textcolor);
    }
  }
  else if (timer > timeroff + 5 * timerdelay && timer <= timeroff + 6 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "THE PREDEFINED JOYSTICK BUTTONS:", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "FIRE CANNON, DROP CHAFF/FLARE,", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "FIRE MISSILE, CHOOSE MISSILE", &textcolor);
    }
    else if (controls == CONTROLS_MOUSE || controls == CONTROLS_MOUSE_REVERSE || controls == CONTROLS_MOUSE_EXP)
    {
      font1->drawTextCentered (0, 7, -2.5, "LEFT MOUSE BUTTON: FIRE CANNON", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "RIGHT MOUSE BUTTON: FIRE MISSILE", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "BUT YOU WILL ALSO HAVE TO USE THE KEYBOARD", &textcolor);
    }
    else if (controls == CONTROLS_KEYBOARD)
    {
      font1->drawTextCentered (0, 6, -2.5, "NOW, LETS HAVE A LOOK AT SOME IMPORTANT KEYS", &textcolor);
    }
  }
  else if (timer > timeroff + 6 * timerdelay && timer <= timeroff + 7 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "A SUMMARY OF COMMANDS CAN BE FOUND IN THE MENU", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "\"ESC\" WILL SHOW THE MENU", &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "AND RETURN BACK TO THE ACTION", &textcolor);
    }
    else
    {
      getKeyString (key_firecannon, buf2);
      sprintf (buf, "%s: FIRE CANNON (UNLIMITED AMMO)", buf2);
      font1->drawTextCentered (0, 7, -2.5, buf, &textcolor);
      getKeyString (key_selectmissile, buf2);
      sprintf (buf, "%s: CHOOSE MISSILE", buf2);
      font1->drawTextCentered (0, 6, -2.5, buf, &textcolor);
      getKeyString (key_firemissile, buf2);
      sprintf (buf, "%s: FIRE MISSILE", buf2);
      font1->drawTextCentered (0, 5, -2.5, buf, &textcolor);
    }
  }
  else if (timer > timeroff + 7 * timerdelay && timer <= timeroff + 8 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "LOOK AT THE RADAR ON THE BOTTOM OF YOUR SCREEN", &textcolor);
    font1->drawTextCentered (0, 6, -2.5, "THERE ARE ENEMIES REPRESENTED BY A YELLOW POINT (TARGETED)", &textcolor);
    font1->drawTextCentered (0, 5, -2.5, "AND A RED POINT (NOT TARGETED)", &textcolor);
  }
  else if (timer > timeroff + 8 * timerdelay && timer <= timeroff + 9 * timerdelay - timerlag)
  {
    if (controls == CONTROLS_JOYSTICK)
    {
      font1->drawTextCentered (0, 7, -2.5, "YOU MAY TARGET AN ENEMY USING THE COOLIE HAT,", &textcolor);
      getKeyString (key_targetnearest, buf2);
      sprintf (buf, "OR PRESSING '%s' ON THE KEYBOARD", buf2);
      font1->drawTextCentered (0, 6, -2.5, buf, &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THEN APPROACH!", &textcolor);
    }
    else
    {
      font1->drawTextCentered (0, 7, -2.5, "YOU MAY TARGET AN ENEMY PRESSING", &textcolor);
      getKeyString (key_targetnearest, buf2);
      sprintf (buf, "OR PRESSING '%s' ON THE KEYBOARD", buf2);
      font1->drawTextCentered (0, 6, -2.5, buf, &textcolor);
      font1->drawTextCentered (0, 5, -2.5, "THEN APPROACH!", &textcolor);
    }
  }
  else if (timer > timeroff + 9 * timerdelay && timer <= timeroff + 10 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "SHOOT THE TARGETS TO COMPLETE", &textcolor);
    font1->drawTextCentered (0, 6, -2.5, "THIS TUTORIAL SUCCESSFULLY", &textcolor);
  }
  if (timer >= timeroff + 7 * timerdelay - timerlag / 2 && state == 0)
  {
    state ++;
    fighter [1]->activate ();
    fighter [1]->target = fighter [0];
    fighter [1]->o = &model_figt;
    fighter [1]->newinit (FIGHTER_TRANSPORT, 0, 200);
    fighter [1]->tl->x = fplayer->tl->x - 30;
    fighter [1]->tl->z = fplayer->tl->z - 30;
    fighter [2]->activate ();
    fighter [2]->target = fighter [0];
    fighter [2]->o = &model_figt;
    fighter [2]->newinit (FIGHTER_TRANSPORT, 0, 200);
    fighter [2]->tl->x = fplayer->tl->x + 30;
    fighter [2]->tl->z = fplayer->tl->z + 30;
  }
}



MissionDogfight1::MissionDogfight1 ()
{
  id = MISSION_DOGFIGHT;
  strcpy (name, "DOGFIGHT");
  strcpy (briefing, "HERE IS THE SECOND TASK: PRACTICE YOUR DOGFIGHT SKILLS.");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 5000 * timestep;
}
  
void MissionDogfight1::start ()
{
  int i;
  day = 1;
  clouds = 2;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 25;
  heading = 220;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->tl->x = 20;
  fplayer->tl->z = 70;
  for (i = 1; i <= 6; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_fige;
    fighter [i]->tl->x = -i * 10;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400);
    fighter [i]->deactivate ();
  }
  fighter [1]->activate ();
  state = 0;
  laststate = 0;
  texttimer = 0;
}

int MissionDogfight1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  if (texttimer >= 200 * timestep) texttimer = 0;
  if (texttimer > 0) texttimer += dt;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 6; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  state ++;
  if (state == 1)
  {
    for (i = 2; i <= 3; i ++)
    {
      fighter [i]->activate ();
      fighter [i]->tl->x = fplayer->tl->x + 50 + 10 * i;
      fighter [i]->tl->z = fplayer->tl->z + 50 + 10 * i;
      fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 15;
    }
    return 0;
  }
  else if (state == 2)
  {
    for (i = 4; i <= 6; i ++)
    {
      fighter [i]->activate ();
      fighter [i]->tl->x = fplayer->tl->x + 50 + 10 * i;
      fighter [i]->tl->z = fplayer->tl->z + 50 + 10 * i;
      fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + 15;
    }
    return 0;
  }
  return 1;
}

void MissionDogfight1::draw ()
{
  char buf [250], buf2 [10], buf3 [10];
  int timeroff = 100 * timestep, timerdelay = 300 * timestep, timerlag = 20 * timestep;

  if (laststate != state)
  {
    texttimer = 1;
    laststate = state;
  }
  if (texttimer > 0)
  {
    if (state == 1)
    {
      font1->drawTextCentered (0, 7, -2.5, "THAT WAS EASY", &textcolor);
      font1->drawTextCentered (0, 6, -2.5, "BUT CAN YOU HANDLE TWO ENEMIES?", &textcolor);
      return;
    }
    if (state == 2)
    {
      font1->drawTextCentered (0, 6, -2.5, "OK, LET'S TRY THREE ENEMIES", &textcolor);
      return;
    }
  }

  if (timer >= 0 && timer <= timeroff - timerlag)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
  else if (timer >= timeroff && timer <= timeroff + timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "THERE IS AN ENEMY ATTACKING", &textcolor);
    getKeyString (key_selectmissile, buf2);
    sprintf (buf, "PRESS '%s' ON THE KEYBOARD TO", buf2);
    font1->drawTextCentered (0, 6, -2.5, buf, &textcolor);
    font1->drawTextCentered (0, 5, -2.5, "SELECT A MISSILE", &textcolor);
  }
  else if (timer >= timeroff + timerdelay && timer <= timeroff + 2 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "CHOOSE A FRIEND-FOE (FF) MISSILE", &textcolor);
    font1->drawTextCentered (0, 6, -2.5, "WHICH DETECTS ENEMIES BY RADAR", &textcolor);
    font1->drawTextCentered (0, 5, -2.5, "SHOOT AS EARLY AS POSSIBLE", &textcolor);
  }
  else if (timer >= timeroff + 2 * timerdelay && timer <= timeroff + 3 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "YOU CAN ONLY FIRE MISSILES WHEN", &textcolor);
    font1->drawTextCentered (0, 6, -2.5, "THE RECTANGULAR LOCK AROUND THE ENEMY", &textcolor);
    font1->drawTextCentered (0, 5, -2.5, "APPEARS YELLOW", &textcolor);
  }
  else if (timer >= timeroff + 3 * timerdelay && timer <= timeroff + 4 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "CHECK THE CHAFF/FLARE DISPLAYS", &textcolor);
    font1->drawTextCentered (0, 6, -2.5, "IF THEY START BLINKING, AN ENEMY HAS", &textcolor);
    font1->drawTextCentered (0, 5, -2.5, "FIRED A MISSILE TO GET YOU DOWN", &textcolor);
  }
  else if (timer >= timeroff + 4 * timerdelay && timer <= timeroff + 5 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "CHAFF IS A DECOY FOR RADAR SEEKING MISSILES", &textcolor);
    font1->drawTextCentered (0, 6, -2.5, "FLARES PROTECT FROM INFRARED MISSILES", &textcolor);
    getKeyString (key_dropchaff, buf2);
    getKeyString (key_dropflare, buf3);
    sprintf (buf, "KEYS: DROP CHAFF = '%s', DROP FLARE = '%s'", buf2, buf3);
    font1->drawTextCentered (0, 5, -2.5, buf, &textcolor);
  }
  else if (timer >= timeroff + 5 * timerdelay && timer <= timeroff + 6 * timerdelay - timerlag)
  {
    font1->drawTextCentered (0, 7, -2.5, "THE COUNTER MEASURE ONLY HAD AN EFFECT", &textcolor);
    font1->drawTextCentered (0, 6, -2.5, "IF THE DISPLAYS STOP BLINKING", &textcolor);
  }
}



MissionTransport::MissionTransport ()
{
  id = MISSION_TRANSPORT;
  strcpy (name, "TRANSPORT");
  strcpy (briefing, "EUROPE: THIS IS YOUR FIRST MISSION FOR THE EAGLE SQADRON. OUR SECRET SERVICE HAS CAUGHT TWO TRANSPORTS PROVIDING TERRORISTS WITH AMMUNITION. THEY ARE ONLY LIGHTLY GUARDED, NO PROBLEM TO TAKE THEM OUT WITH OUR SUPERIOR FIGHTERS.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 5000 * timestep;
  alliedpilot [0] = PILOT_PRIMETIME;
}

void MissionTransport::start ()
{
  int i;
  day = 1;
  clouds = 1;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 45;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_LOW_ALPINE, NULL);
  playerInit ();
  fplayer->tl->x = 0;
  fplayer->tl->z = 100;
  alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 5;
  fighter [1]->tl->z = 105;
  for (i = 2; i <= 3; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_fige;
    fighter [i]->tl->x = -i * 10;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400);
  }
  for (i = 4; i <= 5; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_figt;
    fighter [i]->tl->x = -i * 10;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->newinit (FIGHTER_TRANSPORT, 0, 200);
  }
}

int MissionTransport::processtimer (Uint32 dt)
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

void MissionTransport::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionConvoy::MissionConvoy ()
{
  id = MISSION_CONVOY;
  strcpy (name, "CONVOY");
  strcpy (briefing, "WE HAVE SPOTTED AN ENEMY CONVOY. THIS IS YOUR NEXT TARGET. BE CAREFUL, THERE ARE WIESEL TANKS WITH SURFACE-AIR CANNONS.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 5000 * timestep;
  alliedpilot [0] = PILOT_PRIMETIME;
}
  
void MissionConvoy::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 45;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_ALPINE_EROSION, NULL);
  int px, py;
  l->searchPlain (-1, -1, &px, &py);
  px = px - MAXX / 2;
  py = MAXX / 2 - py;
//    if (px < 0) px += MAXX;
//    if (py < 0) py += MAXX;
  playerInit ();
  fplayer->tl->x = px;
  fplayer->tl->z = py + 100;
  alliedInit (FIGHTER_HAWK, alliedpilot [0], fighter [1]); // always match together 0<->1, 1<->2 etc.
  fighter [1]->tl->x = px + 5;
  fighter [1]->tl->z = py + 105;
  for (i = 2; i <= 3; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_tank1;
    int phi = (i - 2) * 180;
    fighter [i]->newinit (TANK_AIR1, 0, 200);
    fighter [i]->tl->x = px + sine [phi] * 3.0;
    fighter [i]->tl->z = py + cosi [phi] * 3.0;
    fighter [i]->phi = 359 - phi;
    fighter [i]->thrust = 0;
    fighter [i]->maxthrust = 0;
  }
  for (i = 4; i <= 9; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    if (i == 6 || i == 9) fighter [i]->o = &model_pickup1;
    else fighter [i]->o = &model_truck1;
    fighter [i]->tl->x = px + 7.5 - i;
    fighter [i]->tl->z = py + 7.5 - i;
    if (i == 6 || i == 9) fighter [i]->newinit (TANK_PICKUP1, 0, 400);
    else fighter [i]->newinit (TANK_TRUCK1, 0, 400);
  }
}

int MissionConvoy::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 9; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionConvoy::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionDogfight2::MissionDogfight2 ()
{
  id = MISSION_DOGFIGHT2;
  strcpy (name, "DOGFIGHT");
  strcpy (briefing, "SOME ENEMY FIGHTERS ARE ON THEIR WAY ATTACKING ONE OF OUR OUTPOSTS. STOP THEM BEFORE THEY CAN REACH IT!");
  autoLFBriefing ();
  alliedfighters = 3;
  maxtime = 5000 * timestep;
  alliedpilot [0] = PILOT_PRIMETIME;
  alliedpilot [1] = PILOT_SHADOW;
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
  l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->tl->x = 0;
  fplayer->tl->z = 100;
  alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 5;
  fighter [1]->tl->z = 105;
  alliedInit (FIGHTER_FALCON, alliedpilot [1], fighter [2]);
  fighter [2]->tl->x = 10;
  fighter [2]->tl->z = 110;
  for (i = 3; i <= 6; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_fige;
    fighter [i]->tl->x = -i * 10;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 20);
  }
  for (i = 7; i <= 8; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_fige;
    fighter [i]->tl->x = -i * 10;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 20);
    fighter [i]->deactivate ();
  }
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
  for (i = 0; i <= 8; i ++)
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
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
  if (timer >= 1000 * timestep && state == 0)
  {
    state ++;
    fighter [7]->activate ();
    fighter [8]->activate ();
    fighter [7]->tl->x = fplayer->tl->x + 50;
    fighter [7]->tl->z = fplayer->tl->z + 50;
    fighter [7]->tl->y = l->getHeight (fighter [7]->tl->x, fighter [7]->tl->z) + 10;
    fighter [8]->tl->x = fplayer->tl->x + 60;
    fighter [8]->tl->z = fplayer->tl->z + 60;
    fighter [8]->tl->y = l->getHeight (fighter [8]->tl->x, fighter [8]->tl->z) + 10;
  }
  if (timer >= 1000 * timestep && timer <= 1200 * timestep)
    font1->drawTextCentered (0, 7, -3, "MORE ENEMIES ENTERING THE REGION", &textcolor);
}



MissionAirBattle::MissionAirBattle ()
{
  id = MISSION_AIRBATTLE;
  strcpy (name, "AIR BATTLE");
  strcpy (briefing, "WE ARE BEING ATTACKED BY A MASS OF CROWS. THE SQUADRON IS ORDERED TO DEFEND.");
  autoLFBriefing ();
  alliedfighters = 7;
  maxtime = 6000 * timestep;
  alliedpilot [0] = PILOT_PRIMETIME;
  alliedpilot [1] = PILOT_SHADOW;
  alliedpilot [2] = PILOT_FIREBIRD;
  alliedpilot [3] = PILOT_HEPTARGON;
  alliedpilot [4] = PILOT_LARA;
  alliedpilot [5] = PILOT_DRDOOM;
}

void MissionAirBattle::start ()
{
  int i;
  day = 1;
  clouds = 1;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 45;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->tl->x = 0;
  fplayer->tl->z = 100;
  for (i = 1; i <= 6; i ++)
  {
    alliedInit (FIGHTER_FALCON, alliedpilot [i - 1], fighter [i]);
    if (i % 2)
      fighter [i]->tl->x = ((i + 1) / 2) * 5;
    else
      fighter [i]->tl->x = -((i + 1) / 2) * 5;
    fighter [i]->tl->z = 100 + i * 5;
    fighter [i]->target = fighter [7 + i];
  }
  for (i = 7; i <= 22; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (7)];
    fighter [i]->o = &model_fige;
    if (i <= 13)
    {
      fighter [i]->tl->x = -i * 5;
      fighter [i]->tl->z = -i * 5;
    }
    else
    {
      fighter [i]->tl->x = -i * 8 - 150;
      fighter [i]->tl->z = -i * 8 - 150;
    }
    fighter [i]->newinit (FIGHTER_CROW, 0, 450 - i * 14);
  }
}

int MissionAirBattle::processtimer (Uint32 dt)
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

void MissionAirBattle::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionGround1::MissionGround1 ()
{
  id = MISSION_SADEFENSE;
  strcpy (name, "SA DEFENSE");
  strcpy (briefing, "OUR ENEMY HAS BUILT UP A BASE AND SOME AIR DEFENCES AROUND. THE PILOTS OF OUR SQUADRON HAVE TO TAKE OUT THESE AIR DEFENCES.");
  autoLFBriefing ();
  alliedfighters = 2;
  alliedpilot [0] = PILOT_PRIMETIME;
  maxtime = 3000 * timestep;
}

void MissionGround1::start ()
{
  int i;
  day = 0;
  clouds = 3;
  weather = WEATHER_THUNDERSTORM;
  camera = 0;
  sungamma = 40;
  heading = 200;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_ALPINE_NOLAKE, NULL);
  int px, py;
  l->searchPlain (1, 1, &px, &py);
  l->gauss (px, py);
  px = px - MAXX / 2;
  py = MAXX / 2 - py;
  playerInit ();
  fplayer->tl->x = px + 10;
  fplayer->tl->z = py + 80;
  fplayer->target = fighter [2];
  alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = px + 20;
  fighter [1]->tl->z = py + 90;
  fighter [1]->target = fighter [2];
  for (i = 2; i <= 4; i ++)
  {
    fighter [i]->tl->x = px - 3 + i * 3;
    fighter [i]->tl->z = py + 2;
    fighter [i]->target = fighter [0];
    if (i == 2)
    {
      fighter [i]->o = &model_flarak1;
      fighter [i]->newinit (FLARAK_AIR1, 0, 220);
    }
    else
    {
      fighter [i]->o = &model_flak1;
      fighter [i]->newinit (FLAK_AIR1, 0, 200);
    }
  }
  l->searchPlain (2, 1, &px, &py);
  l->gauss (px, py);
  px = px - MAXX / 2;
  py = MAXX / 2 - py;
  for (i = 5; i <= 6; i ++)
  {
    fighter [i]->tl->x = px - 3 + i * 3;
    fighter [i]->tl->z = py + 2;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_flak1;
    fighter [i]->newinit (FLAK_AIR1, 0, 200);
  }
  fighter [7]->tl->x = px + 1;
  fighter [7]->tl->z = py - 1;
  fighter [7]->o = &model_tent1;
  fighter [7]->newinit (STATIC_TENT1, 0, 200);
  fighter [8]->tl->x = px - 1;
  fighter [8]->tl->z = py - 1;
  fighter [8]->o = &model_tent1;
  fighter [8]->newinit (STATIC_TENT1, 0, 200);
}

int MissionGround1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 6; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionGround1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionScout::MissionScout ()
{
  id = MISSION_SCOUT;
  strcpy (name, "SCOUTS");
  strcpy (briefing, "ENEMY SCOUTS ARE APPROACHING THIS REGION. BE CARFUL, THEY ARE FLYING BUZZARDS AND SHOULD BE CONSIDERED EQUAL TO OUR FALCONS.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 3000 * timestep;
  alliedpilot [0] = PILOT_PRIMETIME;
}

void MissionScout::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 22;
  heading = 250;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
  playerInit ();
  fplayer->tl->x = 0;
  fplayer->tl->z = 100;
  alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 5;
  fighter [1]->tl->z = 105;
  for (i = 2; i <= 3; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (2)];
    fighter [i]->o = &model_figd;
    fighter [i]->tl->x = -i * 10;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->newinit (FIGHTER_BUZZARD, 0, 100 + myrandom (50));
  }
}

int MissionScout::processtimer (Uint32 dt)
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

void MissionScout::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



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
  maxtime = 6000 * timestep;
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
  l = new GLLandscape (space, LANDSCAPE_ALPINE_EROSION, NULL);
  int px, py;
  l->searchPlain (-1, -1, &px, &py);
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
  px = px - MAXX / 2;
  py = MAXX / 2 - py;
  playerInit ();
  fplayer->tl->x = px + 10;
  fplayer->tl->z = py + 100;
  fplayer->target = fighter [4];
  for (i = 1; i <= 3; i ++)
  {
    alliedInit (FIGHTER_FALCON, alliedpilot [i - 1], fighter [i]);
    fighter [i]->tl->x = px + 10 + i * 5;
    fighter [i]->tl->z = py + 100 + i * 5;
  }
  int n = 4;
  fighter [n]->tl->x = px - 4;
  fighter [n]->tl->z = py;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall2;
  fighter [n]->newinit (STATIC_HALL2, 0, 400);
  n ++;
  fighter [n]->tl->x = px + 2;
  fighter [n]->tl->z = py + 5;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall2;
  fighter [n]->newinit (STATIC_HALL2, 0, 400);
  n ++;
  fighter [n]->tl->x = px;
  fighter [n]->tl->z = py - 1;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  n ++;
  fighter [n]->tl->x = px + 2;
  fighter [n]->tl->z = py - 1;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  n ++;
  fighter [n]->tl->x = px + 4;
  fighter [n]->tl->z = py - 1;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_hall1;
  fighter [n]->newinit (STATIC_HALL1, 0, 400);
  n ++;
  fighter [n]->tl->x = px - 1.5;
  fighter [n]->tl->z = py - 4.5;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl->x = px + 1.5;
  fighter [n]->tl->z = py - 4.5;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl->x = px - 3;
  fighter [n]->tl->z = py + 4;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl->x = px - 5.5;
  fighter [n]->tl->z = py + 4;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl->x = px - 4;
  fighter [n]->tl->z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_tent4;
  fighter [n]->newinit (STATIC_TENT4, 0, 400);
  n ++;
  fighter [n]->tl->x = px - 6;
  fighter [n]->tl->z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->phi = 90;
  fighter [n]->maxtheta = 0;
  n ++;
  fighter [n]->tl->x = px - 6;
  fighter [n]->tl->z = py - 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->phi = 0;
  fighter [n]->maxtheta = 0;
  n ++;
  fighter [n]->tl->x = px + 6;
  fighter [n]->tl->z = py - 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->phi = 270;
  fighter [n]->maxtheta = 0;
  n ++;
  fighter [n]->tl->x = px + 6;
  fighter [n]->tl->z = py + 6;
  fighter [n]->target = fighter [0];
  fighter [n]->o = &model_flak1;
  fighter [n]->newinit (FLAK1, 0, 200);
  fighter [n]->phi = 180;
  fighter [n]->maxtheta = 0;
  n ++;
  for (i = n; i < n + 6; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (4)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl->x = px + i * 5 - 60;
    fighter [i]->tl->z = py + i * 5 - 60;
    fighter [i]->newinit (FIGHTER_CROW, 0, 180 + myrandom (200));
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
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionDefend1::MissionDefend1 ()
{
  id = MISSION_DEFEND1;
  strcpy (name, "DEFEND SAM");
  strcpy (briefing, "THERE ARE NEW TERRORIST ACTIVITIES IN NORTHERN AFRICA. OUR SQUARDRON HAS TO MOVE THERE AND OUR FIRST MISSION IS THE DEFENDANCE OF SOME SAM LAUNCHERS AGAINST SUPERIOR TANKS. HOWEVER, OUR OLD HAWKS HAVE BEEN REPLACED.");
  autoLFBriefing ();
  alliedfighters = 2;
  alliedpilot [0] = PILOT_SHADOW;
  maxtime = 3000 * timestep;
  selfighter [1] = FIGHTER_HAWK2;
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
  l = new GLLandscape (space, LANDSCAPE_DESERT, NULL);
  playerInit ();
  fplayer->tl->x = 10;
  fplayer->tl->z = 90;
  fplayer->target = fighter [7];
  alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 20;
  fighter [1]->tl->z = 100;
  fighter [1]->target = fighter [8];
  for (i = 2; i <= 6; i ++)
  {
    fighter [i]->tl->x = 20 + i * 5;
    fighter [i]->tl->z = 10;
    if (i == 3 || i == 4 || i == 5)
    {
      fighter [i]->o = &model_flarak1;
      fighter [i]->newinit (FLARAK_AIR1, 0, 400);
      fighter [i]->target = fighter [0];
    }
    else
    {
      fighter [i]->o = &model_flak1;
      fighter [i]->newinit (FLAK_AIR1, 0, 400);
      fighter [i]->target = fighter [0];
    }
    fighter [i]->party = 1;
  }
  for (i = 7; i <= 10; i ++)
  {
    int off = 35;
    if (difficulty == 0) off = 55;
    else off = 15;
    fighter [i]->tl->x = i * 5 - 50;
    fighter [i]->tl->z = -i * 5 - off;
    fighter [i]->o = &model_tank2;
    fighter [i]->newinit (TANK_GROUND1, 0, 300);
    fighter [i]->target = fighter [i - 4];
  }
  for (i = 11; i <= 13; i ++)
  {
    int off = 40;
    if (difficulty == 0) off = 55;
    else off = 25;
    fighter [i]->tl->x = i * 5 - 50;
    fighter [i]->tl->z = -i * 5 - off * 2;
    if (i == 12)
    {
      fighter [i]->newinit (TANK_AIR1, 0, 300);
      fighter [i]->o = &model_tank1;
    }
    else
    {
      fighter [i]->newinit (TANK_GROUND1, 0, 300);
      fighter [i]->o = &model_tank2;
    }
    fighter [i]->target = fighter [i - 4];
    fighter [i]->deactivate ();
  }
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
  for (i = 0; i <= 13; i ++)
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
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
  if (timer >= 600 * timestep && state == 0)
  {
    state ++;
    fighter [11]->activate ();
    fighter [12]->activate ();
    fighter [13]->activate ();
  }
  if (timer >= 600 * timestep && timer <= 800 * timestep)
    font1->drawTextCentered (0, 7, -3, "MORE TANKS ARE ATTACKING", &textcolor);
}



MissionDogfight3::MissionDogfight3 ()
{
  id = MISSION_DOGFIGHT3;
  strcpy (name, "DOGFIGHT");
  strcpy (briefing, "ENEMY FIGHTERS HAVE BEEN SIGHTED. YOU HAVE TO INTERCEPT THEM IMMEDIATELY. EXPECT BOMBERS WITHIN THEIR WING.");
  autoLFBriefing ();
  alliedfighters = 4;
  maxtime = 3000 * timestep;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_PRIMETIME;
  alliedpilot [1] = PILOT_SHADOW;
  alliedpilot [2] = PILOT_LARA;
}

void MissionDogfight3::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 20;
  heading = 300;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_DESERT, NULL);
  playerInit ();
  fplayer->tl->x = 0;
  fplayer->tl->z = 100;
  alliedInit (FIGHTER_FALCON, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 5;
  fighter [1]->tl->z = 105;
  alliedInit (FIGHTER_FALCON, alliedpilot [1], fighter [2]);
  fighter [2]->tl->x = 10;
  fighter [2]->tl->z = 110;
  alliedInit (FIGHTER_FALCON, alliedpilot [2], fighter [3]);
  fighter [3]->tl->x = 15;
  fighter [3]->tl->z = 115;
  for (i = 4; i <= 8; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (4)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl->x = -i * 10;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 25);
  }
  for (i = 9; i <= 11; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (4)];
    fighter [i]->o = &model_figa;
    fighter [i]->tl->x = -i * 10;
    fighter [i]->tl->z = -i * 10;
    fighter [i]->newinit (FIGHTER_SWALLOW, 0, 400 - i * 25);
    fighter [i]->deactivate ();
  }
}

int MissionDogfight3::processtimer (Uint32 dt)
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
  if (b) return 0;
  return 1;
}

void MissionDogfight3::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
  if (timer >= 800 * timestep && state == 0)
  {
    state = 1;
    fighter [9]->activate ();
    fighter [10]->activate ();
    fighter [11]->activate ();
    fighter [9]->tl->x = fplayer->tl->x + 55;
    fighter [9]->tl->z = fplayer->tl->z + 55;
    fighter [9]->tl->y = l->getHeight (fighter [9]->tl->x, fighter [9]->tl->z) + 10;
    fighter [10]->tl->x = fplayer->tl->x + 60;
    fighter [10]->tl->z = fplayer->tl->z + 60;
    fighter [10]->tl->y = l->getHeight (fighter [10]->tl->x, fighter [10]->tl->z) + 10;
    fighter [11]->tl->x = fplayer->tl->x + 65;
    fighter [11]->tl->z = fplayer->tl->z + 65;
    fighter [11]->tl->y = l->getHeight (fighter [11]->tl->x, fighter [11]->tl->z) + 10;
  }
  if (timer >= 800 * timestep && timer <= 1000 * timestep)
    font1->drawTextCentered (0, 7, -3, "BOMBERS ARE ENTERING THE REGION", &textcolor);
}



MissionTank1::MissionTank1 ()
{
  id = MISSION_TANK1;
  strcpy (name, "TANKS");
  strcpy (briefing, "THE ENEMY IS GATHERING LOTS OF TANKS. FLY AN ASSAUT AND DESTROY THEM.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 3500 * timestep;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_SHADOW;
}

void MissionTank1::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 40;
  heading = 280;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_DESERT, NULL);
  playerInit ();
  fplayer->tl->x = 0;
  fplayer->tl->z = 100;
  alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 5;
  fighter [1]->tl->z = 105;
  for (i = 2; i <= 7; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (2)];
    fighter [i]->o = &model_tank2;
    fighter [i]->tl->x = -i * 4;
    fighter [i]->tl->z = -i * 4;
    fighter [i]->newinit (TANK_GROUND1, 0, 400 - i * 25);
    fighter [i]->maxthrust = 0;
    fighter [i]->thrust = 0;
  }
  for (i = 8; i <= 10; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (2)];
    fighter [i]->o = &model_tank1;
    fighter [i]->tl->x = i * 3;
    fighter [i]->tl->z = i * 3;
    fighter [i]->newinit (TANK_AIR1, 0, 80);
  }
  for (i = 11; i <= 13; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_container1;
    fighter [i]->tl->x = i * 3;
    fighter [i]->tl->z = 60;
    fighter [i]->newinit (STATIC_CONTAINER1, 0, 0);
  }
}

int MissionTank1::processtimer (Uint32 dt)
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
  if (b) return 0;
  return 1;
}

void MissionTank1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionShip1::MissionShip1 ()
{
  id = MISSION_SHIP1;
  strcpy (name, "DESTROYER");
  strcpy (briefing, "OUR SQUAD HAS BEEN ORDERED INTO THE MEDITERRANEAN. YOUR FIRST GOAL ARE SOME DESTROYERS, GUARDED BY CROWS.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 3500 * timestep;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_SHADOW;
}

void MissionShip1::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_SEA, NULL);
  playerInit ();
  fplayer->tl->x = 80;
  fplayer->tl->z = 0;
  fplayer->phi = 90;
  alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 85;
  fighter [1]->tl->z = 5;
  fighter [1]->phi = 90;
  fighter [1]->target = fighter [6];
  for (i = 2; i <= 3; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (2)];
    fighter [i]->o = &model_ship2;
    fighter [i]->tl->x = -i * 4;
    fighter [i]->tl->z = -i * 4;
    fighter [i]->newinit (SHIP_DESTROYER1, 0, 50);
    fighter [i]->maxthrust = 0;
    fighter [i]->thrust = 0;
  }
  for (i = 4; i <= 8; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (2)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl->x = -i * 10;
    fighter [i]->tl->z = 0;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 20);
  }
}

int MissionShip1::processtimer (Uint32 dt)
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
  if (b) return 0;
  return 1;
}

void MissionShip1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionShip2::MissionShip2 ()
{
  id = MISSION_SHIP2;
  strcpy (name, "OILRIG");
  strcpy (briefing, "AN OILRIG IS BEING ATTACKED BY ENEMY FORCES. DEFEND! OUR NEW SUPERIOR FIGHTER GL-15 IS NOW AVAILABLE.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 2500 * timestep;
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_SHADOW;
}

void MissionShip2::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 15;
  heading = 270;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_SEA, NULL);
  playerInit ();
  fplayer->tl->x = 80;
  fplayer->tl->z = 0;
  fplayer->phi = 90;
  alliedInit (FIGHTER_REDARROW, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 85;
  fighter [1]->tl->z = 5;
  fighter [1]->phi = 90;
  fighter [1]->target = fighter [6];
  fighter [2]->o = &model_oilrig;
  fighter [2]->tl->x = 20;
  fighter [2]->tl->z = 0;
  fighter [2]->newinit (STATIC_OILRIG1, 0, 0);
  fighter [2]->maxthrust = 0;
  fighter [2]->thrust = 0;
  fighter [2]->party = 1;
  for (i = 3; i <= 5; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (2)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl->x = -50 - i * 10;
    fighter [i]->tl->z = 0;
    fighter [i]->newinit (FIGHTER_CROW, 0, 300 - i * 10);
  }
  for (i = 6; i <= 7; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [2];
    fighter [i]->o = &model_figa;
    fighter [i]->tl->x = -80 - i * 10;
    fighter [i]->tl->z = 0;
    fighter [i]->newinit (FIGHTER_SWALLOW, 0, 150);
  }
}

int MissionShip2::processtimer (Uint32 dt)
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
  if (!fighter [2]->active)
    return 2;
  if (b) return 0;
  return 1;
}

void MissionShip2::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionShip3::MissionShip3 ()
{
  id = MISSION_SHIP3;
  strcpy (name, "CRUISER ATTACK");
  strcpy (briefing, "THE CRUISER C-15 ARAKON HAS ENTERED NEUTRAL TERRITORY WATERS AND MUST BE ELIMINATED.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 2500 * timestep;
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_SHADOW;
}

void MissionShip3::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_SEA, NULL);
  playerInit ();
  fplayer->tl->x = 80;
  fplayer->tl->z = 0;
  fplayer->phi = 90;
  alliedInit (FIGHTER_HAWK2, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 85;
  fighter [1]->tl->z = 5;
  fighter [1]->phi = 90;
  fighter [1]->target = fighter [6];
  fighter [2]->party = 0;
  fighter [2]->target = fighter [0];
  fighter [2]->o = &model_ship1;
  fighter [2]->tl->x = -20;
  fighter [2]->tl->z = 0;
  fighter [2]->newinit (SHIP_CRUISER, 0, 200);
  fighter [2]->maxthrust = 0;
  fighter [2]->thrust = 0;
  for (i = 4; i <= 7; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (2)];
    fighter [i]->o = &model_fige;
    fighter [i]->tl->x = -i * 5;
    fighter [i]->tl->z = 0;
    fighter [i]->newinit (FIGHTER_CROW, 0, 400 - i * 10);
  }
}

int MissionShip3::processtimer (Uint32 dt)
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
  if (b) return 0;
  return 1;
}

void MissionShip3::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



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
  l = new GLLandscape (space, LANDSCAPE_CANYON, NULL);
  l->searchPlain (-1, -1, &px, &py);
  px = px - MAXX / 2;
  py = MAXX / 2 - py;
  playerInit ();
  fplayer->tl->x = px + 130;
  fplayer->tl->z = py + 130;
  fplayer->phi = 45;
  fplayer->target = fighter [1];
  fighter [1]->o = &model_egg;
  fighter [1]->newinit (STATIC_COMPLEX1, 0, 0);
  fighter [1]->tl->x = px + 1;
  fighter [1]->tl->z = py + 1;
  fighter [1]->maxthrust = 0;
  fighter [1]->thrust = 0;
  for (i = 2; i <= 4; i ++)
  {
    fighter [i]->o = &model_radar;
    fighter [i]->newinit (STATIC_RADAR1, 0, 0);
    fighter [i]->tl->x = px - 2 - (i - 2) * 2;
    fighter [i]->tl->z = py - 2 - (i - 2) * 2;
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
    fighter [i]->tl->x = px - 15 - i * 3;
    fighter [i]->tl->z = py - 15 - i * 3;
    fighter [i]->deactivate ();
  }
  for (i = 11; i <= 12; i ++)
  {
    fighter [i]->o = &model_flarak1;
    fighter [i]->target = fighter [0];
    fighter [i]->newinit (FLARAK_AIR1, 0, 200);
    fighter [i]->tl->x = px + 4;
    fighter [i]->tl->z = py + i * 3 - 30;
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
  if (fplayer->tl->y >= fighter [1]->tl->y && dist (fplayer->tl->x - fighter [1]->tl->x, fplayer->tl->z - fighter [1]->tl->z) < 150)
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
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



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
  maxtime = 6000 * timestep;
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
  l = new GLLandscape (space, LANDSCAPE_CANYON, NULL);
  playerInit ();
  fplayer->tl->x = 100;
  fplayer->tl->z = 100;
  fplayer->phi = 45;
  fplayer->target = fighter [5];
  for (i = 1; i <= 4; i ++)
  {
    alliedInit (FIGHTER_REDARROW, alliedpilot [i - 1], fighter [i]);
    if (i % 2)
      fighter [i]->tl->x = 100 + ((i + 1) / 2) * 5;
    else
      fighter [i]->tl->x = 100 - ((i + 1) / 2) * 5;
    fighter [i]->tl->z = 100 + i * 5;
    fighter [i]->target = fighter [5 + i];
    fighter [i]->phi = 45;
  }
  for (i = 5; i <= 22; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (5)];
    if (i <= 15)
    {
      fighter [i]->tl->x = -i * 5;
      fighter [i]->tl->z = -i * 5;
      fighter [i]->o = &model_fige;
      fighter [i]->newinit (FIGHTER_CROW, 0, myrandom (120) + 250);
    }
    else if (i <= 19)
    {
      fighter [i]->tl->x = -i * 8 - 160;
      fighter [i]->tl->z = -i * 8 - 160;
      fighter [i]->o = &model_figd;
      fighter [i]->newinit (FIGHTER_BUZZARD, 0, myrandom (120) + 150);
    }
    else
    {
      fighter [i]->tl->x = -i * 8 - 240;
      fighter [i]->tl->z = -i * 8 - 240;
      fighter [i]->o = &model_figa;
      fighter [i]->newinit (FIGHTER_SWALLOW, 0, myrandom (120) + 200);
    }
  }
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
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionCanyon3::MissionCanyon3 ()
{
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_HAWK2;
  alliedpilot [0] = PILOT_MATRIX;
  alliedpilot [1] = PILOT_PRIMETIME;
  alliedpilot [2] = PILOT_FIREBIRD;
  alliedpilot [3] = PILOT_LARA;
  id = MISSION_CANYON3;
  strcpy (name, "MAIN BASE");
  strcpy (briefing, "THERE IS ONE MAIN BASE IN THE CANYON, CONNECTED TO A BASE ON THE MOON. WE MUST TAKE OUT THIS BASE FIRST!");
  autoLFBriefing ();
  alliedfighters = 4;
  maxtime = 3000 * timestep;
}

void MissionCanyon3::start ()
{
  int i, px, py;
  day = 0;
  clouds = 3;
  weather = WEATHER_THUNDERSTORM;
  camera = 0;
  sungamma = 45;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_CANYON, NULL);
  l->searchPlain (-1, -1, &px, &py);
  px = px - MAXX / 2;
  py = MAXX / 2 - py;
  playerInit ();
  fplayer->tl->x = px + 100;
  fplayer->tl->z = py + 100;
  fplayer->phi = 45;
  fplayer->target = fighter [5];
  for (i = 1; i <= 4; i ++)
  {
    alliedInit (FIGHTER_REDARROW, alliedpilot [i - 1], fighter [i]);
    if (i % 2)
      fighter [i]->tl->x = px + 100 + ((i + 1) / 2) * 5;
    else
      fighter [i]->tl->x = px + 100 - ((i + 1) / 2) * 5;
    fighter [i]->tl->z = py + 100 + i * 5;
    fighter [i]->target = fighter [5 + i];
    fighter [i]->phi = 45;
  }
  fighter [5]->o = &model_egg;
  fighter [5]->newinit (STATIC_COMPLEX1, 0, 0);
  fighter [5]->tl->x = px;
  fighter [5]->tl->z = py;
  fighter [5]->maxthrust = 0;
  fighter [5]->thrust = 0;
  for (i = 6; i <= 9; i ++)
  {
    fighter [i]->o = &model_hall1;
    fighter [i]->newinit (STATIC_HALL1, 0, 100);
    fighter [i]->maxthrust = 0;
    fighter [i]->thrust = 0;
  }
  fighter [6]->tl->x = px + 3;
  fighter [6]->tl->z = py + 3;
  fighter [7]->tl->x = px - 3;
  fighter [7]->tl->z = py + 3;
  fighter [8]->tl->x = px - 3;
  fighter [8]->tl->z = py - 3;
  fighter [9]->tl->x = px + 3;
  fighter [9]->tl->z = py - 3;
  for (i = 10; i <= 19; i ++)
  {
    fighter [i]->party = 0;
    fighter [i]->target = fighter [myrandom (5)];
    fighter [i]->phi = 180;
    if (i <= 15)
    {
      fighter [i]->o = &model_figb;
      fighter [i]->newinit (FIGHTER_CROW, 0, i * 20);
    }
    else
    {
      fighter [i]->o = &model_figd;
      fighter [i]->newinit (FIGHTER_BUZZARD, 0, i * 15);
    }
    fighter [i]->tl->x = px - i * 3;
    fighter [i]->tl->z = py - i * 3;
  }
}

int MissionCanyon3::processtimer (Uint32 dt)
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

void MissionCanyon3::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionMoonDefense1::MissionMoonDefense1 ()
{
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_PHOENIX;
  alliedpilot [0] = PILOT_LARA;
  id = MISSION_MOON1;
  strcpy (name, "TURRETS");
  strcpy (briefing, "THE DESTINY OF OUR FUTURE IS SOMEWHERE ON THE MOON. OUR NEW BOMBER GL-117 WITH ITS SUPERIOR SHIELDS SHOULD BE EXTREMELY HELPFUL. AT FIRST, WE HAVE TO TAKE OUT THE TURRETS.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 4000 * timestep;
}

void MissionMoonDefense1::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_MOON, NULL);
  playerInit ();
  fplayer->tl->x = 100;
  fplayer->tl->z = 100;
  fplayer->phi = 45;
  fplayer->target = fighter [6];
  alliedInit (FIGHTER_PHOENIX, alliedpilot [0], fighter [1]);
  fighter [1]->tl->x = 105;
  fighter [1]->tl->z = 105;
  fighter [1]->target = fighter [6];
  fighter [1]->phi = 45;
  for (i = 2; i <= 10; i ++)
  {
    int ix = (i - 2) % 3;
    int iy = (i - 2) / 3;
    fighter [i]->tl->x = ix * 10;
    fighter [i]->tl->z = iy * 10;
    fighter [i]->target = fighter [myrandom (2)];
    fighter [i]->o = &model_flak1;
    fighter [i]->newinit (FLAK_AIR1, 0, 200);
  }
  for (i = 11; i <= 20; i ++)
  {
    int ix = (i - 11) % 3;
    int iy = (i - 11) / 3;
    fighter [i]->tl->x = ix * 20 + 40;
    fighter [i]->tl->z = iy * 20 + 40;
    fighter [i]->target = fighter [myrandom (2)];
    fighter [i]->o = &model_mine1;
    fighter [i]->newinit (MISSILE_MINE1, 0, 220);
  }
}

int MissionMoonDefense1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 10; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionMoonDefense1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionMoonDogfight1::MissionMoonDogfight1 ()
{
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_PHOENIX;
  alliedpilot [0] = PILOT_MATRIX;
  id = MISSION_MOON2;
  strcpy (name, "ELITE DOGFIGHT");
  strcpy (briefing, "ENEMY FIGHTERS HAVE APPEARED IN THIS REGION.");
  autoLFBriefing ();
  alliedfighters = 2;
  maxtime = 2500 * timestep;
}

void MissionMoonDogfight1::start ()
{
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_MOON, NULL);
  playerInit ();
  fplayer->tl->x = -100;
  fplayer->tl->z = -100;
  fplayer->phi = 200;
  fplayer->target = fighter [2];
  fighter [1]->tl->x = -110;
  fighter [1]->tl->z = -110;
  fighter [1]->phi = 200;
  fighter [1]->target = fighter [3];
  alliedInit (FIGHTER_REDARROW, alliedpilot [0], fighter [1]);
  fighter [2]->tl->x = 0;
  fighter [2]->tl->z = 0;
  fighter [2]->target = fighter [0];
  fighter [2]->phi = 200;
  fighter [2]->o = &model_figh;
  fighter [2]->newinit (FIGHTER_BLACKBIRD, 0, 150);
  fighter [3]->tl->x = 10;
  fighter [3]->tl->z = 10;
  fighter [3]->target = fighter [1];
  fighter [3]->phi = 200;
  fighter [3]->o = &model_figh;
  fighter [3]->newinit (FIGHTER_BLACKBIRD, 0, 200);
  fighter [4]->tl->x = 200;
  fighter [4]->tl->z = 200;
  fighter [4]->target = fighter [0];
  fighter [4]->phi = 200;
  fighter [4]->o = &model_figh;
  fighter [4]->newinit (FIGHTER_BLACKBIRD, 0, 40);
  fighter [5]->tl->x = 210;
  fighter [5]->tl->z = 210;
  fighter [5]->target = fighter [1];
  fighter [5]->phi = 200;
  fighter [5]->o = &model_figh;
  fighter [5]->newinit (FIGHTER_BLACKBIRD, 0, 140);
}

int MissionMoonDogfight1::processtimer (Uint32 dt)
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
  if (b) return 0;
  return 1;
}

void MissionMoonDogfight1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionMoonBase1::MissionMoonBase1 ()
{
  selfighter [0] = FIGHTER_REDARROW;
  selfighter [1] = FIGHTER_PHOENIX;
  id = MISSION_MOON3;
  strcpy (name, "SNEAKING");
  strcpy (briefing, "TIME TO ATTACK. THE BASE IS HEAVILY GUARDED BY BLACKBIRDS. USE AN ASTEROID FIELD TO GET NEAR THE BASE AND DESTROY IT.");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 3500 * timestep;
}

void MissionMoonBase1::start ()
{
  int i;
  day = 0;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_FLAT_MOON, NULL);
  playerInit ();
  fplayer->tl->x = -20;
  fplayer->tl->z = -40;
  fplayer->phi = 200;
  fplayer->target = fighter [29];
  for (i = 1; i < 21; i ++)
  {
    fighter [i]->tl->x = cosi [(i * 140) % 360] * 4.5;
    fighter [i]->tl->z = i * 0.6;
    fighter [i]->tl->y = l->getHeight (fighter [i]->tl->x, fighter [i]->tl->z) + sine [(i * 160) % 360] * 4.0 + 25.0;
    fighter [i]->target = fighter [0];
    fighter [i]->phi = 200;
    fighter [i]->o = &model_aster1;
    fighter [i]->newinit (ASTEROID, 0, 150);
    fighter [i]->party = 0;
  }
  for (i = 21; i < 29; i ++)
  {
    fighter [i]->tl->x = (i - 20) * 4;
    fighter [i]->tl->z = 210;
    fighter [i]->target = fighter [0];
    fighter [i]->phi = 50;
    fighter [i]->o = &model_figh;
    fighter [i]->newinit (FIGHTER_BLACKBIRD, 0, 100);
  }
  fighter [29]->tl->x = 50 + difficulty * 15;
  fighter [29]->tl->z = 180 + difficulty * 30;
  fighter [29]->o = &model_base1;
  fighter [29]->newinit (STATIC_BASE1, 0, 100);
}

int MissionMoonBase1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 29; i <= 29; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionMoonBase1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionMoon1::MissionMoon1 ()
{
  id = MISSION_MOON;
  strcpy (name, "MOON");
  strcpy (briefing, "THIS IS JUST A SIMPLE DEMO MISSION");
  autoLFBriefing ();
  alliedfighters = 1;
  maxtime = 2000 * timestep;
}

void MissionMoon1::start ()
{
  day = 0;
  clouds = 0;
  weather = WEATHER_CLOUDY;
  camera = 0;
  sungamma = 50;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_MOON, NULL);
  playerInit ();
  fplayer->tl->x = 50;
  fplayer->tl->z = 50;
  fighter [1]->target = fighter [0];
  fighter [1]->o = &model_figa;
  fighter [1]->newinit (FIGHTER_SWALLOW, 0, 200);
  fighter [2]->target = fighter [0];
  fighter [2]->o = &model_figa;
  fighter [2]->newinit (FIGHTER_SWALLOW, 0, 150);
}

int MissionMoon1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 4; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionMoon1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 4, -2, name, &textcolor);
  }
}



MissionMultiDogfight1::MissionMultiDogfight1 ()
{
  id = MISSION_MULTIPLAYER_DOGFIGHT;
  strcpy (name, "MULTIPLAYER");
  strcpy (briefing, "THIS IS JUST A SIMPLE DEMO MISSION");
  autoLFBriefing ();
  alliedfighters = 2;
}

void MissionMultiDogfight1::start ()
{
  int i;
  day = 1;
  clouds = 0;
  weather = WEATHER_SUNNY;
  camera = 0;
  sungamma = 25;
  if (l != NULL) delete l;
  l = new GLLandscape (space, LANDSCAPE_ALPINE, NULL);
  for (i = 0; i <= 1; i ++)
  {
    fighter [i]->activate ();
    fighter [i]->ai = false;
    fighter [i]->party = i;
    fighter [i]->target = fighter [0];
    fighter [i]->o = &model_fig;
    fighter [i]->tl->x = 50 + i * 30;
    fighter [i]->tl->z = 100;
  }
  fighter[1]->ai = true;
  if (isserver)
    fplayer = fighter [0];
  else
    fplayer = fighter [1];
  if (controls != 100)
    fplayer->easymodel = 2;
}

int MissionMultiDogfight1::processtimer (Uint32 dt)
{
  bool b = false;
  int i;
  timer += dt;
  if (!fplayer->active && fplayer->explode >= 35 * timestep)
  {
    return 2;
  }
  for (i = 0; i <= 4; i ++)
  {
    if (fighter [i]->active)
      if (fighter [i]->party == 0)
        b = true;
  }
  if (b) return 0;
  return 1;
}

void MissionMultiDogfight1::draw ()
{
  if (timer >= 0 && timer <= 50 * timestep)
  {
    font1->drawTextCentered (0, 0, -1, "DOGFIGHT");
  }
}

#endif
