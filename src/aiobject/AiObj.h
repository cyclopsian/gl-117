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

#define GLUT_BUILDING_LIB 1

#include "configuration/Configuration.h" // ok
#include "model3d/Model3d.h" // ok
#include "effects/Effects.h" // ok

class UnitDescriptor
{
  public:
    int id;
    std::string name;
    std::string displayedName;
    
    UnitDescriptor ()
    {
      this->id = -1;
      this->name = "";
      this->displayedName = "";
    }

    UnitDescriptor (int id)
    {
      this->id = id;
      this->name = "";
      this->displayedName = "";
    }

    UnitDescriptor (int id, const std::string &name, const std::string &displayedName)
    {
      this->id = id;
      this->name = name;
      this->displayedName = displayedName;
    }
    
    bool operator == (const UnitDescriptor &desc) const
    {
      return id == desc.id;
    }

    bool operator < (const UnitDescriptor &desc) const
    {
      return id < desc.id;
    }

    bool operator <= (const UnitDescriptor &desc) const
    {
      return id <= desc.id;
    }

    bool operator > (const UnitDescriptor &desc) const
    {
      return id > desc.id;
    }

    bool operator >= (const UnitDescriptor &desc) const
    {
      return id >= desc.id;
    }

    bool operator != (const UnitDescriptor &desc) const
    {
      return id != desc.id;
    }

    int operator + (const UnitDescriptor &desc) const
    {
      return id + desc.id;
    }

    int operator + (int value) const
    {
      return id + value;
    }

    int operator - (const UnitDescriptor &desc) const
    {
      return id - desc.id;
    }

    int operator - (int value) const
    {
      return id - value;
    }
};

// id values of objects
// non-AI air objects
extern UnitDescriptor CannonBeginDescriptor;
extern UnitDescriptor Cannon1Descriptor;
extern UnitDescriptor Cannon1bDescriptor;
extern UnitDescriptor Cannon2Descriptor;
extern UnitDescriptor Cannon2bDescriptor;
extern UnitDescriptor AsteroidDescriptor;
extern UnitDescriptor FlareDescriptor;
extern UnitDescriptor ChaffDescriptor;
extern UnitDescriptor CannonEndDescriptor;
// missiles
extern UnitDescriptor MissileBeginDescriptor;
extern UnitDescriptor AamHs1Descriptor;
extern UnitDescriptor AamHs2Descriptor;
extern UnitDescriptor AamHs3Descriptor;
extern UnitDescriptor Agm1Descriptor;
extern UnitDescriptor Agm2Descriptor;
extern UnitDescriptor DfmDescriptor;
extern UnitDescriptor AamFf1Descriptor;
extern UnitDescriptor AamFf2Descriptor;
extern UnitDescriptor MineDescriptor;
extern UnitDescriptor MissileEndDescriptor;
// air units
extern UnitDescriptor AirBeginDescriptor;
extern UnitDescriptor FighterBeginDescriptor;
extern UnitDescriptor FalconDescriptor;
extern UnitDescriptor SwallowDescriptor;
extern UnitDescriptor HawkDescriptor;
extern UnitDescriptor Hawk2Descriptor;
extern UnitDescriptor BuzzardDescriptor;
extern UnitDescriptor CrowDescriptor;
extern UnitDescriptor PhoenixDescriptor;
extern UnitDescriptor RedArrowDescriptor;
extern UnitDescriptor BlackBirdDescriptor;
extern UnitDescriptor StormDescriptor;
extern UnitDescriptor FighterEndDescriptor;
extern UnitDescriptor TransportDescriptor;
extern UnitDescriptor Transport2Descriptor;
extern UnitDescriptor AirEndDescriptor;
// moving ground units from here
extern UnitDescriptor MovingGroundBeginDescriptor;
extern UnitDescriptor TankBeginDescriptor;
extern UnitDescriptor WieselDescriptor;
extern UnitDescriptor PantherDescriptor;
extern UnitDescriptor MobileSamDescriptor;
extern UnitDescriptor PickupDescriptor;
extern UnitDescriptor Pickup2Descriptor;
extern UnitDescriptor TruckDescriptor;
extern UnitDescriptor Truck2Descriptor;
extern UnitDescriptor TankEndDescriptor;
// moving water units from here
extern UnitDescriptor WaterBeginDescriptor;
extern UnitDescriptor ShipBeginDescriptor;
extern UnitDescriptor CruiserDescriptor;
extern UnitDescriptor LightDestroyerDescriptor;
extern UnitDescriptor ShipEndDescriptor;
// static ground units from here
extern UnitDescriptor StaticGroundBeginDescriptor;
extern UnitDescriptor AntiAircraftBeginDescriptor;
extern UnitDescriptor SacDescriptor;
extern UnitDescriptor SamDescriptor;
extern UnitDescriptor AntiAircraftEndDescriptor;
// passive static units from here
extern UnitDescriptor StaticPassiveBeginDescriptor;
extern UnitDescriptor TentDescriptor;
extern UnitDescriptor BigTentDescriptor;
extern UnitDescriptor ContainerDescriptor;
extern UnitDescriptor HallDescriptor;
extern UnitDescriptor Hall2Descriptor;
extern UnitDescriptor OilrigDescriptor;
extern UnitDescriptor ComplexDescriptor;
extern UnitDescriptor RadarDescriptor;
extern UnitDescriptor MoonBaseDescriptor;
extern UnitDescriptor DepotDescriptor;
extern UnitDescriptor LaserBarrierDescriptor;
extern UnitDescriptor RubbleDescriptor;
extern UnitDescriptor HouseDescriptor;



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

    UnitDescriptor id;          ///< object type: SamDescriptor, TentDescriptor, SwallowDescriptor, ...
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
    virtual void init ();
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

    virtual void init ();     ///< initialize variables
    void missileCount ();
    void newinit (const UnitDescriptor &id, int party, int intelligence, int precision, int aggressivity); ///< init new AI object
    void newinit (const UnitDescriptor & id, int party, int intelligence); ///< init new AI object (esp. non-fighter)
    void initValues (DynamicObj *dobj, float phi); ///< init values to shoot cannon or missile
    void fireCannon (DynamicObj *laser, float phi);
    void fireCannon (DynamicObj **laser, float phi);
    void fireCannon (DynamicObj **laser);
    void fireMissile2 (const UnitDescriptor &id, AIObj *missile, AIObj *target);
    int firstMissile ();           ///< select first missile type
    int nextMissile (int from);    ///< select next missile type (cyclic)
    bool haveMissile (const UnitDescriptor &id);     ///< missile of type id left?
    bool haveMissile ();           ///< missile of type missiletype left?
    void decreaseMissile (const UnitDescriptor &id); ///< decrease missiles by one
    bool fireMissile (const UnitDescriptor &id, AIObj **missile, AIObj *target);
    bool fireMissile (AIObj **missile, AIObj *target);
    bool fireMissile (const UnitDescriptor &id, AIObj **missile);
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
