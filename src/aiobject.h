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

/* This file includes all AI objects instancing models. */

#ifndef IS_AIOBJECT_H
#define IS_AIOBJECT_H

#include "common.h"

#include "model.h"
#include "effects.h"

#define CANNON1 0
#define CANNON2 99
#define MISSILE1 100
#define MISSILE_AIR1 100
#define MISSILE_AIR2 101
#define MISSILE_AIR3 102
#define MISSILE_GROUND1 103
#define MISSILE_GROUND2 104
#define MISSILE_DF1 105
#define MISSILE_MINE1 180
#define MISSILE2 199
#define FIGHTER1 200
#define FIGHTER_FALCON 200
#define FIGHTER_SWALLOW 201
#define FIGHTER_HAWK 202
#define FIGHTER_HAWK2 203
#define FIGHTER_BUZZARD 204
#define FIGHTER_CROW 205
#define FIGHTER_PHOENIX 206
#define FIGHTER_REDARROW 207
#define FIGHTER_BLACKBIRD 208
#define FIGHTER_TRANSPORT 280
#define FIGHTER2 299
#define TANK1 700
#define TANK_AIR1 700
#define TANK_GROUND1 710
#define TANK_PICKUP1 780
#define TANK_TRUCK1 790
#define TANK2 799
#define SHIP1 800
#define SHIP_CRUISER 800
#define SHIP_DESTROYER1 810
#define SHIP2 899
#define FLAK1 1000
#define FLAK_AIR1 1000
#define FLARAK_AIR1 1010
#define FLAK2 1099
#define STATIC 10000
#define STATIC_TENT1 10000
#define STATIC_TENT4 10003
#define STATIC_CONTAINER1 10100
#define STATIC_HALL1 10200
#define STATIC_HALL2 10201
#define STATIC_OILRIG1 10300
#define STATIC_COMPLEX1 10301
#define STATIC_RADAR1 10302
#define STATIC_BASE1 10303
#define ASTEROID 50

//extern GLLandscape *l;

class DynamicObj : public CSpaceObj
{
  public:
  int id; // object type: FLARAK_AIR1, STATIC_TENT1, FIGHTER_SWALLOW, ...
//  bool controls;
  bool active; // deactivated means no AI, no collision control and so on
  // easymodel is the ancient core of the game ;-)
  // true means that thetha will directly alter phi! Computer AI uses this model!
  // false means the realistic model with ailerons, elevator, rudder
  bool easymodel;
  int ttl; // time to live: cannon and missiles will only live a short time, missiles will sink when ttl<=0
  int immunity; // immunity means the object cannot collide with others, needed for shooting missiles/cannon
  int impact; // this value will be subtracted from the other objects shield when colliding
  // Imagine a carthesian coordinate system in the landscape, the y-axis pointing up
  float phi; // angle in x-z plane (polar coordinates)
  float gamma; // orthogonal angle (polar coordinates)
  float theta; // roll angle of the fighter!
  float speed; // current thrust, not the speed itself!
  float realspeed; // the current speed, we only want to calculate this once per time step
  float forcex, forcez, forcey; // the force vectors (orthogonal, should be clear)
  float braking; // far away from reality: this factorizes the last speed vector with the current conditions (see move method)
  float manoeverability; // how fast a fighter can alter its direction
  float nimbility; // how fast a fighter responds to alterations of recXXX
  float maxspeed; // maximum throttle value
  float rectheta; // roll angle the fighter/object wants to reach
  float recspeed; // throttle the fighter/object wants to reach
  float recheight; // height above ground the fighter wants to reach
  float recgamma; // elevation the fighter wants to reach
  float maxtheta; // a maximum roll angle the object may achieve, easymodel only!
  float maxgamma; // a maximum elevation the object may achieve, easymodel only!
  float aileroneffect; // number between 1.0 and -0.5, as diving should be less
  float ruddereffect;
  float rolleffect;
  int party; // usually 0 for enemies, 1 for allieds
  int points, fighterkills, shipkills, tankkills, otherkills; // statistics
  int sink; // ships will not explode but sink
  bool killed; // statistics
  Space *space; // in which space is this object, there is only one ;-)
  DynamicObj *source; // missiles must keep track of the object they have been fired from -> statistics
  char net [100];
//  GLLandscape *l;
  
  int shield, maxshield; // current and initial/maximum shield

  int net_write ();
  void net_read ();
  void activate ();
  void deactivate ();
  void dinit ();
  DynamicObj ();
  DynamicObj (Space *space2, CModel *o2, float zoom2);
  void speedUp ();
  void speedDown ();
  float distance (DynamicObj *target);
  // check whether the object is exploding or sinking and deactivate if necessary
  void checkExplosion ();
  // check the objects shield value and explode/sink if necessary
  void checkShield ();
  // check whether the object collides on the ground and alter gamma and y-translation
  void crashGround ();
  // check for collision, simplified model, each model is surrounded by a cube
  // this works pretty well, but we must use more than one model for complex models or scenes
  void collide (DynamicObj *d); // d must be the medium (laser, missile)
  void setExplosion (float maxzoom, int len);
  void setBlackSmoke (float maxzoom, int len);
  // return heading difference towards enemy
  int getAngle (DynamicObj *o);
  // check for a looping, this is tricky :-)
  bool checkLooping ();
  void move ();
};

const int missiletypes = 6;

class AIObj : public DynamicObj
{
  protected:
  public:
  bool ai;
  bool autofire;
  DynamicObj *target;
  int acttype;
  int intelligence, aggressivity, precision;
  int manoevertheta, manoeverheight, manoeverspeed;
  int idle;
  int firemissilettl;
  int missiletype; // only relevant for the player
  int missiles [missiletypes];
  int aw; // aktueller Winkel zum Ziel
  int score;
  float dtheta, dgamma;
  float disttarget; // aktuelle Distanz zum Ziel
  CSmoke *smoke;

  void aiinit ();
  void newinit (int id, int party, int intelligence, int precision, int aggressivity);
  void newinit (int id, int party, int intelligence);
  AIObj ();
  AIObj (Space *space2, CModel *o2, float zoom2);
  ~AIObj ();
  void initValues (DynamicObj *dobj, float phi);
  void fireCannon (DynamicObj *laser, float phi);
  void fireCannon (DynamicObj **laser, float phi);
  void fireCannon (DynamicObj **laser);
  void fireMissile2 (int id, AIObj *missile, AIObj *target);
  int nextMissile (int from);
  bool haveMissile (int id);
  void decreaseMissile (int id);
  void fireMissile (int id, AIObj **missile, AIObj *target);
  void fireMissile (int id, AIObj **missile);
  void fireMissileAir (AIObj **missile, AIObj *target);
  void fireMissileGround (AIObj **missile);
  void targetNearestEnemy (AIObj **f);
  void targetNext (AIObj **f);
  void targetPrevious (AIObj **f);
  void aiAction (AIObj **f, AIObj **m, DynamicObj **c);
};

#endif
