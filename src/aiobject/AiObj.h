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

/* This file includes all AI objects instancing models. */

#ifndef IS_AIOBJECT_H
#define IS_AIOBJECT_H

#include "configuration/Configuration.h" // ok
#include "model3d/Model3d.h" // ok
#include "effects/Effects.h" // ok

// id values of objects
// non-AI air objects
#define CANNON1 0
#define ASTEROID 50
#define FLARE1 80
#define CHAFF1 85
#define CANNON2 99
// missiles
#define MISSILE1 100
#define MISSILE_AIR1 100
#define MISSILE_AIR2 101
#define MISSILE_AIR3 102
#define MISSILE_GROUND1 103
#define MISSILE_GROUND2 104
#define MISSILE_DF1 105
#define MISSILE_FF1 106
#define MISSILE_FF2 107
#define MISSILE_MINE1 180
#define MISSILE2 199
// air units
#define AIR 200
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
#define FIGHTER_STORM 209
#define FIGHTER_PILOTED2 249
#define FIGHTER_TRANSPORT 280
#define FIGHTER_TRANSPORT2 281
#define FIGHTER2 299
// moving ground units from here
#define MOVING_GROUND 500
#define TANK1 700
#define TANK_AIR1 700
#define TANK_GROUND1 710
#define TANK_TRSAM1 711
#define TANK_PICKUP1 780
#define TANK_TRUCK1 790
#define TANK_TRUCK2 791
#define TANK2 799
// moving water units from here
#define MOVING_WATER 800
#define SHIP1 800
#define SHIP_CRUISER 800
#define SHIP_DESTROYER1 810
#define SHIP2 899
// static ground units from here
#define STATIC_GROUND 1000
#define FLAK1 1000
#define FLAK_AIR1 1000
#define FLARAK_AIR1 1010
#define FLAK2 1099
// passive static units from here
#define STATIC_PASSIVE 10000
#define STATIC_TENT1 10000
#define STATIC_TENT4 10003
#define STATIC_CONTAINER1 10100
#define STATIC_HALL1 10200
#define STATIC_HALL2 10201
#define STATIC_OILRIG1 10300
#define STATIC_COMPLEX1 10301
#define STATIC_RADAR1 10302
#define STATIC_BASE1 10303
#define STATIC_DEPOT1 10304
#define STATIC_BARRIER1 10400

class ObjectStatistics
{
  public:

    int points;       ///< hitpoints damage to enemies
    int fighterkills; ///< number of fighter shots
    int shipkills;    ///< number of ships sunk
    int tankkills;    ///< number of tank shots
    int otherkills;   ///< number of other objects (buildings) shot
    bool killed;      ///< this object is already deactivated
};

/**
* This class represents a dynamic object in space without any AI methods.
*/
class DynamicObj : public SpaceObj
{
  public:

    int id;          ///< object type: FLARAK_AIR1, STATIC_TENT1, FIGHTER_SWALLOW, ...
  //  bool controls;
    bool active;  ///< deactivated means no AI, no collision control and so on
    /// easymodel==1 is the ancient core of the game ;-)
    /// 1 means that theta will directly alter phi! Computer AI uses this model!
    /// 2 means the realistic model with ailerons, elevator, rudder
    int easymodel;
    int ttl;         ///< time to live: cannon and missiles will only live a short time, missiles will sink when ttl<=0
    int immunity;    ///< immunity means the object cannot collide with others, needed for shooting missiles/cannon
    float impact;    ///< this value will be subtracted from the other objects shield when colliding
    /// Imagine a carthesian coordinate system in the landscape, the y-axis pointing up
    Rotation currot; ///< current rotation, sets rot of SpaceObj, TODO: merge with rot
    Rotation recrot; ///< recommended rotation, recrot.phi is not used
    Rotation maxrot; ///< only used for easymodel calculations, no use of maxrot.phi
//    float phi;       ///< angle in x-z plane (polar coordinates)
//    float gamma;     ///< orthogonal angle (polar coordinates)
//    float theta;     ///< roll angle of the fighter!
    float thrust;    ///< current thrust, not the speed itself!
    float realspeed; ///< the current speed, we only want to calculate this once per time step
    Vector3 force;   ///< the force vector
    float braking;   ///< far away from reality: this factorizes the last speed vector with the current conditions (see move method)
    float manoeverability; ///< how fast a fighter can alter its direction
    float nimbility; ///< how fast a fighter responds to alterations of recXXX (recommended XXX)
    float maxthrust; ///< maximum throttle value
//    float rectheta;  ///< roll angle the fighter/object wants to reach
    float recthrust; ///< throttle the fighter/object wants to reach
    float recheight; ///< height above ground the fighter wants to reach
//    float recgamma;  ///< elevation the fighter wants to reach
//    float maxtheta;  ///< a maximum roll angle the object may achieve, easymodel==1 only!
//    float maxgamma;  ///< a maximum elevation the object may achieve, easymodel==1 only!
    float elevatoreffect; ///< number between 1.0 and -0.5, as diving should be less
    float ruddereffect;
    float rolleffect;
    float recelevatoreffect; ///< number between 1.0 and -0.5, as diving should be less
    float recruddereffect;
    float recrolleffect;
    int sink;         ///< ships will not explode but sink
    int party;        ///< usually 0 for enemies, 1 for allies
    ObjectStatistics stat; ///< objects statistics like number of kills, etc.
    Space *space;    ///< in which space is this object, there is only one ;-)
    DynamicObj *source; ///< missiles must keep track of the object they have been fired from -> statistics
    int bomber;      ///< act as bomber and prefer groud targets
  //  char net [100];
    int realism;
    Vector3 acc;     ///< acceleration
    float shield, maxshield; ///< current and initial/maximum shield

    DynamicObj ();
    DynamicObj (Space *space2, Model3d *o2, float zoom2);
    virtual ~DynamicObj ();

    void activate ();
    void deactivate ();
    void dinit ();
    void thrustUp ();
    void thrustDown ();
    float distance (DynamicObj *target);
    float distanceXZ (DynamicObj *target);
    /// check whether the object is exploding or sinking and deactivate if necessary
    void checkExplosion (Uint32 dt);
    /// check the objects shield value and explode/sink if necessary
    void checkShield ();
    /// check whether the object collides on the ground and alter gamma and y-translation
    void crashGround (Uint32 dt);
    /// check for collision, simplified model, each model is surrounded by a cube
    /// this works pretty well, but we must use more than one model for complex models or scenes
    void collide (DynamicObj *d, Uint32 dt); // d must be the medium (laser, missile)
    void setExplosion (float maxzoom, int len);
    void setBlackSmoke (float maxzoom, int len);
    /// return heading difference towards enemy
    int getAngle (DynamicObj *o);
    /// return elevation difference towards enemy
    int getAngleH (DynamicObj *o);
    /// check for a looping, this is tricky :-)
    bool checkLooping ();
    void move (Uint32 dt, float camphi, float camgamma);
};

const int missiletypes = 8;
const int missileracks = 6;

/**
* This class represents an object with artificial intelligence.
* Differences between fighters, missiles... should be made be splitting this class into
* multiple subclasses. Currently everything is implemented using if-clauses.
*/
class AIObj : public DynamicObj
{
  protected:

  public:

    bool ai;            ///< AI on/off
    bool autofire;      ///< cannon fire on/off
    DynamicObj *target; ///< targeted object
    int acttype;        ///< object is doing some action (Immelmann, Loop, ... not yet implemented)
    /// three intellience characteristics which make up a pilot: 0 = best, 400 = worst
    int intelligence;   ///< valid for every AI object: manoevers, fire rate (tanks), ...
    int aggressivity;   ///< valid for fighters: fly low, stay near and behind enemy
    int precision;      ///< valid for fighters: heading calculation
    /// manoevers disable any other AI consideration
    int manoevertheta, manoeverheight, manoeverthrust;
    int idle;           ///< counter how long AI object does the same thing (to change direction)
    int firemissilettl; ///< minimum time to wait between shooting missiles
    int firecannonttl;  ///< minimum time to wait between shooting cannon
    int smokettl;       ///< minimum time to wait between setting smoke elements
    int missiletype;    ///< only relevant for the player, describes type: AAM, AGM, DF
    int missiles [missiletypes]; ///< number of missiles of each type
    int missilerack [missileracks]; ///< number of missile racks
    int missilerackn [missileracks]; ///< number of missile racks
    float aw;           ///< current heading difference to target
    int score;          ///< final score
    float dtheta, dgamma; ///< theta/gamma alteration (smooth piloting)
    float disttarget;   ///< current distance to target
    int flares;
    int chaffs;
    int fireflarettl;
    int firechaffttl;
    int ammo;
    int ttf;            ///< time to fire missile, targeting mechanism
    Smoke *smoke;       ///< bright smoke behind the object (fighter&missiles)
    Uint32 timer;
    int statfirepower;  ///< firepower (missiles) statistics, number of stars
    bool dualshot;      ///< one or two cannons?
    int manoeverstate;  ///< changes to realistic manoevers and turns off easymodel

    AIObj ();
    AIObj (Space *space2, Model3d *o2, float zoom2);
    virtual ~AIObj ();

    void aiinit ();     ///< initialize variables
    void missileCount ();
    void newinit (int id, int party, int intelligence, int precision, int aggressivity); ///< init new AI object
    void newinit (int id, int party, int intelligence); ///< init new AI object (esp. non-fighter)
    void initValues (DynamicObj *dobj, float phi); ///< init values to shoot cannon or missile
    void fireCannon (DynamicObj *laser, float phi);
    void fireCannon (DynamicObj **laser, float phi);
    void fireCannon (DynamicObj **laser);
    void fireMissile2 (int id, AIObj *missile, AIObj *target);
    int firstMissile ();           ///< select first missile type
    int nextMissile (int from);    ///< select next missile type (cyclic)
    bool haveMissile (int id);     ///< missile of type id left?
    bool haveMissile ();           ///< missile of type missiletype left?
    void decreaseMissile (int id); ///< decrease missiles by one
    bool fireMissile (int id, AIObj **missile, AIObj *target);
    bool fireMissile (AIObj **missile, AIObj *target);
    bool fireMissile (int id, AIObj **missile);
    bool fireMissile (AIObj **missile);
    bool fireMissileAir (AIObj **missile, AIObj *target);
    bool selectMissileAir (AIObj **missile);
    bool fireMissileAirFF (AIObj **missile, AIObj *target);
    bool selectMissileAirFF (AIObj **missile);
    bool fireMissileGround (AIObj **missile);
    bool selectMissileGround (AIObj **missile);
    void targetNearestGroundEnemy (AIObj **f);
    void targetNearestEnemy (AIObj **f);
    void targetNextEnemy (AIObj **f);
    void targetLockingEnemy (AIObj **f);
    void targetNext (AIObj **f);
    void targetPrevious (AIObj **f);
    void setSmoke (Uint32 dt);
    void selectNewTarget (AIObj **f);
    void selectTarget (AIObj **f);
    void checkTtl (Uint32);
    float getMinimumHeight ();
    void easyPiloting (Uint32 dt);
    void limitRotation ();
    void estimateTargetPosition (float *dx2, float *dz2);
    void estimateHeading (float dx2, float dz2);
    void estimateFighterHeading (AIObj *fi);
    void decreaseManoeverCounter (Uint32 dt);
    int getFireRate ();
    virtual void aiAction (Uint32 dt, AIObj **f, AIObj **m, DynamicObj **c, DynamicObj **flare, DynamicObj **chaff, float camphi, float camgamma); // core AI method
};

class Missile : public AIObj
{
  public:
    Missile ();
    Missile (Space *space2, Model3d *o2, float zoom2);
    virtual ~Missile ();

    virtual void aiAction (Uint32 dt, AIObj **f, AIObj **m, DynamicObj **c, DynamicObj **flare, DynamicObj **chaff, float camphi, float camgamma);
};

class Tank : public AIObj
{
  public:
    Tank ();
    Tank (Space *space2, Model3d *o2, float zoom2);
    virtual ~Tank ();

    virtual void aiAction (Uint32 dt, AIObj **f, AIObj **m, DynamicObj **c, DynamicObj **flare, DynamicObj **chaff, float camphi, float camgamma);
};

class StaticAa : public AIObj
{
  public:
    StaticAa ();
    StaticAa (Space *space2, Model3d *o2, float zoom2);
    virtual ~StaticAa ();

    virtual void aiAction (Uint32 dt, AIObj **f, AIObj **m, DynamicObj **c, DynamicObj **flare, DynamicObj **chaff, float camphi, float camgamma);
};

class StaticPassive : public AIObj
{
  public:
    StaticPassive ();
    StaticPassive (Space *space2, Model3d *o2, float zoom2);
    virtual ~StaticPassive ();

    virtual void aiAction (Uint32 dt, AIObj **f, AIObj **m, DynamicObj **c, DynamicObj **flare, DynamicObj **chaff, float camphi, float camgamma);
};

class Ship : public AIObj
{
  public:
    Ship ();
    Ship (Space *space2, Model3d *o2, float zoom2);
    virtual ~Ship ();

    virtual void aiAction (Uint32 dt, AIObj **f, AIObj **m, DynamicObj **c, DynamicObj **flare, DynamicObj **chaff, float camphi, float camgamma);
};

class Fighter : public AIObj
{
  public:
    Fighter ();
    Fighter (Space *space2, Model3d *o2, float zoom2);
    virtual ~Fighter ();

    void fireFlare2 (DynamicObj *flare);
    void fireChaff2 (DynamicObj *chaff);
    bool fireFlare (DynamicObj **flare, AIObj **missile);
    bool fireChaff (DynamicObj **chaff, AIObj **missile);
    bool performManoevers (float myheight);
    virtual void placeMissiles ();
    virtual void aiAction (Uint32 dt, AIObj **f, AIObj **m, DynamicObj **c, DynamicObj **flare, DynamicObj **chaff, float camphi, float camgamma);
};

#endif
