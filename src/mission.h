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

#ifndef IS_MISSION_H
#define IS_MISSION_H

#include <stdio.h>
#include <string.h>

/*#include "common.h"
#include "model.h"
#include "aiobject.h"*/
#include "main.h"

#define MISSION_DEMO 0
#define MISSION_TUTORIAL 1
#define MISSION_DOGFIGHT 2
#define MISSION_MOON 5
#define MISSION_CANYON 6
#define MISSION_TRANSPORT 10
#define MISSION_CONVOY 11
#define MISSION_DOGFIGHT2 12
#define MISSION_AIRBATTLE 13
#define MISSION_SADEFENSE 14
#define MISSION_SCOUT 15
#define MISSION_BASE 16
#define MISSION_DEFEND1 20
#define MISSION_DOGFIGHT3 21
#define MISSION_TANK1 22
#define MISSION_SHIP1 25
#define MISSION_SHIP2 26
#define MISSION_SHIP3 27
#define MISSION_CANYON1 30
#define MISSION_CANYON2 31
#define MISSION_CANYON3 32
#define MISSION_MOON1 33
#define MISSION_MOON2 34
#define MISSION_MOON3 35
#define MISSION_MULTIPLAYER 1000
#define MISSION_MULTIPLAYER_DOGFIGHT 1000

class Mission
{
  protected:
  void autoLFBriefing ();

  public:
  int timer;
  int maxtime;
  int alliedfighters;
  char briefing [1000];
  char name [64];
  int id;
  int selfighter [3];
  int selfighters, wantfighter;
  int selweapon [3];
  int selweapons, wantweapon;
  int alliedpilot [10];
  CColor textcolor;
  int difficulty;
  int heading;

  Mission ();
  void playerInit ();
  void alliedInit (int fighterid, int pilotid, AIObj *aiobj);
  void init ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
  void checkScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points);
  int getScore (int missionstate, int timebonus, int fighterkills, int shipkills, int tankkills, int otherkills, int shieldbonus, int points);
  int getScore (int missionstate);
};

class MissionDemo1 : public Mission
{
  public:
  MissionDemo1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionTutorial1 : public Mission
{
  private:
  bool loop;

  public:
  MissionTutorial1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionDogfight1 : public Mission
{
  public:
  int state, laststate, texttimer;
  MissionDogfight1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionTransport : public Mission
{
  public:
  MissionTransport ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionConvoy : public Mission
{
  public:
  MissionConvoy ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionDogfight2 : public Mission
{
  public:
  MissionDogfight2 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionAirBattle : public Mission
{
  public:
  MissionAirBattle ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionGround1 : public Mission
{
  public:
  MissionGround1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionScout : public Mission
{
  public:
  MissionScout ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionBase : public Mission
{
  public:
  MissionBase ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionDefend1 : public Mission
{
  public:
  MissionDefend1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionDogfight3 : public Mission
{
  public:
  MissionDogfight3 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionTank1 : public Mission
{
  public:
  MissionTank1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionShip1 : public Mission
{
  public:
  MissionShip1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionShip2 : public Mission
{
  public:
  MissionShip2 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionShip3 : public Mission
{
  public:
  MissionShip3 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionCanyon1 : public Mission
{
  public:
  MissionCanyon1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionCanyon2 : public Mission
{
  public:
  MissionCanyon2 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionCanyon3 : public Mission
{
  public:
  MissionCanyon3 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionMoonDefense1 : public Mission
{
  public:
  MissionMoonDefense1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionMoonDogfight1 : public Mission
{
  public:
  MissionMoonDogfight1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionMoonBase1 : public Mission
{
  public:
  MissionMoonBase1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionMoon1 : public Mission
{
  public:
  MissionMoon1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

class MissionMultiDogfight1 : public Mission
{
  public:
  MissionMultiDogfight1 ();
  virtual void start ();
  virtual int processtimer ();
  virtual void draw ();
};

#endif
