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

#include "AiObj.h"
#include "logging/Logging.h"
#include "gllandscape/GlLandscape.h"
#include "game/globals.h"
#include "math/Math.h"



// disabled
int DynamicObj::net_write ()
{
//  net [0] = '.';
  int z = 1;
/*  memcpy (&net [z], &tl.x, sizeof (&tl.x));
  z += sizeof (&tl.x);
  memcpy (&net [z], &tl.y, sizeof (&tl.y));
  z += sizeof (&tl.y);
  memcpy (&net [z], &tl.z, sizeof (&tl.z));
  z += sizeof (&tl.z);
  memcpy (&net [z], &phi, sizeof (&phi));
  z += sizeof (&phi);
  memcpy (&net [z], &theta, sizeof (&theta));
  z += sizeof (&theta);
  memcpy (&net [z], &gamma, sizeof (&gamma));
  z += sizeof (&gamma);*/
  return z;
}

// disabled
void DynamicObj::net_read ()
{
  int z = 1;
/*  memcpy (&tl.x, &net [z], sizeof (&tl.x));
  z += sizeof (&tl.x);
  memcpy (&tl.y, &net [z], sizeof (&tl.y));
  z += sizeof (&tl.y);
  memcpy (&tl.z, &net [z], sizeof (&tl.z));
  z += sizeof (&tl.z);
  memcpy (&phi, &net [z], sizeof (&phi));
  z += sizeof (&phi);
  memcpy (&theta, &net [z], sizeof (&theta));
  z += sizeof (&theta);
  memcpy (&gamma, &net [z], sizeof (&gamma));
  z += sizeof (&gamma);*/
}

void DynamicObj::activate ()
{
  active = true;
  draw = true;
}

void DynamicObj::deactivate ()
{
  active = false;
  draw = false;
}

void DynamicObj::dinit ()
{
  rot.gamma = 90;
  currot.phi = 0; currot.theta = 0; currot.gamma = 180;
  recrot.theta = 0;
  tl.z = 0; tl.x = 0;
  force.x = 0; force.z = 0; force.y = 0;
  maxthrust = 0.3; braking = 0/*0.99*/; manoeverability = 0.5;
  thrust = maxthrust; recthrust = thrust; recheight = 5.0;
  ttl = -1;
  shield = 0.01F; maxshield = 0.01F;
  immunity = 0;
  recrot.gamma = 180;
  id = CANNON1;
  impact = 7;
  source = NULL;
  stat.points = 0;
  party = 0;
  easymodel = 1; // easy model
  elevatoreffect = 0;
  ruddereffect = 0;
  rolleffect = 0;
  maxrot.gamma = 70;
  maxrot.theta = 90;
  currot.gamma = 180;
  currot.theta = 0;
  explode = 0;
  sink = 0;
  nimbility = 1.0;
  stat.fighterkills = 0;
  stat.shipkills = 0;
  stat.tankkills = 0;
  stat.otherkills = 0;
  stat.killed = false;
  realism = false;
  acc.x = acc.y = acc.z = 0;
}

DynamicObj::DynamicObj ()
{
  dinit ();
}

DynamicObj::DynamicObj (Space *space2, Model3d *o2, float zoom2)
{
  this->space = space2;
  o = o2;
  zoom = zoom2;
  dinit ();
  space->addObject (this);
}

void DynamicObj::thrustUp ()
{
  recthrust += maxthrust / 12;
  if (recthrust > maxthrust) recthrust = maxthrust;
}

void DynamicObj::thrustDown ()
{
  recthrust -= maxthrust / 12;
  if (recthrust < maxthrust / 2) recthrust = maxthrust / 2;
}

float DynamicObj::distance (DynamicObj *target)
{
  float dx = target->tl.x - tl.x;
  float dz = target->tl.z - tl.z;
  float dy = target->tl.y - tl.y;
  return sqrt (dx * dx + dz * dz + dy * dy);
}

float DynamicObj::distanceXZ (DynamicObj *target)
{
  float dx = target->tl.x - tl.x;
  float dz = target->tl.z - tl.z;
  return sqrt (dx * dx + dz * dz);
}

// check whether the object is exploding or sinking and deactivate if necessary
void DynamicObj::checkExplosion (Uint32 dt)
{
  if (explode > 0)
  {
    if (explode == 1)
    {
      ttl = -1;
      if (id == STATIC_CONTAINER1 || id == STATIC_RADAR1 || id == STATIC_COMPLEX1)
      {
        setExplosion (1.5, 40 * timestep);
        setBlackSmoke (3.0, 80 * timestep);
      }
      else if (id == STATIC_OILRIG1)
      {
        setExplosion (3.0, 40 * timestep);
        setBlackSmoke (5.5, 80 * timestep);
      }
      else if (id == STATIC_TENT1)
      {
      }
      else if (id == TANK1)
      {
      }
      else
      {
        float zoom2 = zoom * 2;
        if (zoom2 > 2) zoom2 = 2;
        setExplosion (zoom2, 35 * timestep);
        setBlackSmoke (1.0, 60 * timestep);
      }
    }
    if (id >= STATIC_GROUND || (id >= MOVING_GROUND && id <= MOVING_WATER))
    {
      if (explode >= 25 * timestep && ttl == -1)
      {
        setExplosion (zoom * 2, 35 * timestep);
        setBlackSmoke (1.0, 60 * timestep);
        ttl = -2;
      }
      if (explode >= 30 * timestep && ttl == -2)
      {
        setExplosion (zoom * 2, 35 * timestep);
        setBlackSmoke (1.0, 60 * timestep);
        ttl = -3;
      }
    }
    if (explode >= 35 * timestep)
    {
      deactivate ();
      ttl = -1;
      explode += dt; // must be > 35*timestep to end mission
      if (id >= STATIC_GROUND || (id >= MOVING_GROUND && id <= MOVING_WATER))
      {
        explode = 0;
        draw = true;
        id = STATIC_PASSIVE;
        shield = 100000;
        o = &model_rubble1;
        zoom *= 0.7F;
        if (zoom > 1) zoom = 1;
        tl.y = l->getExactHeight (tl.x, tl.z) + zoom / 4;
      }
    }
    else
    {
      explode += dt;
    }
  }
  if (sink)
  {
    sink += dt;
    if (sink > 100 * timestep)
    {
      deactivate (); ttl = -1;
    }
  }
}

// check the objects shield value and explode/sink if necessary
void DynamicObj::checkShield ()
{
  if (shield <= 0)
  {
    shield = 0;
    if (explode <= 0)
    {
      if (id >= MISSILE1 && id <= MISSILE2)
      { explode = 1; active = false; }
      if (id >= FIGHTER1 && id <= FIGHTER2)
      { explode = 1; active = false; }
      if (id >= TANK1 && id <= TANK2)
      { explode = 1; active = false; }
      if (id >= FLAK1 && id <= FLAK2)
      { explode = 1; active = false; }
      if (id >= STATIC_PASSIVE)
      { explode = 1; active = false; }
    }
    if (sink <= 0)
      if (id >= SHIP1 && id <= SHIP2)
      { sink = 1; }
  }
}

// check whether the object collides on the ground and alter gamma and y-translation
void DynamicObj::crashGround (Uint32 dt)
{
  if (id >= MOVING_GROUND)
    return;
  float height = tl.y - l->getExactHeight (tl.x, tl.z);
  if (height < zoom)
  {
    tl.y -= (height - zoom);
    currot.gamma += 10;
    if (shield > 0)
    {
      if (id >= MISSILE1 && id <= MISSILE2)
      {
        setExplosion (1.2, 30 * timestep);
        setBlackSmoke (1.2, 30 * timestep);
      }
      if (id >= FIGHTER1 && id <= FIGHTER2)
      {
        setExplosion (0.2, 25 * timestep);
        setBlackSmoke (0.5, 25 * timestep);
      }
    }
    if (id >= CANNON1 && id <= CANNON2)
      deactivate ();
    float decfac = 3.0F;
    if (this == (DynamicObj *) fplayer && game == GAME_PLAY)
    {
      if (difficulty == 1) decfac = 6.0F;
      else if (difficulty == 2) decfac = 15.0F;
    }
    if (realism && this == (DynamicObj *) fplayer && game == GAME_PLAY)
	  shield = -1;
	else
	  shield -= decfac * dt / timestep;
  }
  // restrict to a maximum height, we want an action game!!! a little bit more now 50 -> 80
  if (height > 80) tl.y = l->getHeight (tl.x, tl.z) + 80;
}

// check for collision, simplified model, each model is surrounded by a cube
// this works pretty well, but we must use more than one model for complex models or scenes
void DynamicObj::collide (DynamicObj *d, Uint32 dt) // d must be the medium (laser, missile)
{
  if (immunity > 0 || d->immunity > 0) return;
  if (explode > 0 || sink > 0) return;
  
  bool collide = false;
  if (tl.x + o->cube.x >= d->tl.x - d->o->cube.x && tl.x - o->cube.x <= d->tl.x + d->o->cube.x &&
      tl.y + o->cube.y >= d->tl.y - d->o->cube.y && tl.y - o->cube.y <= d->tl.y + d->o->cube.y &&
      tl.z + o->cube.z >= d->tl.z - d->o->cube.z && tl.z - o->cube.z <= d->tl.z + d->o->cube.z)
  {
    collide = true;
  }

  if (collide)
  {
    if (this == (DynamicObj *) fplayer && game == GAME_PLAY && realism && d->id >= AIR && d->id < MOVING_GROUND)
	  {
      shield = -1.0F; // player collision vs another plane in SIM mode, boom
	    d->shield = -1.0F;
	  }
    if (id < STATIC_PASSIVE || (id >= STATIC_PASSIVE && d->id >= MISSILE1 && d->id <= MISSILE2))
      shield -= (float) d->impact;
    else
      shield -= 2.0F;
    d->shield -= (float) impact;
    if (d->source != NULL && active) // only for missiles/cannons
    {
      if (d->source->party != party) // calculate points
      {
        if (maxshield < 2000)
          d->source->stat.points += (int) impact; // extra points for shooting an enemy object
      }
      else
      {
        d->source->stat.points -= (int) impact; // subtract points for shooting an own object
      }

      if (shield <= 0)
        if (d->source->party != party && active && draw && !stat.killed)
          if (d->source->id >= FIGHTER1 && d->source->id <= FIGHTER2)
          {
            stat.killed = true;
            if (id >= FIGHTER1 && id <= FIGHTER2)
              d->source->stat.fighterkills ++;
            else if (id >= SHIP1 && id <= SHIP2)
              d->source->stat.shipkills ++;
            else if ((id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
              d->source->stat.tankkills ++;
            else
              d->source->stat.otherkills ++;
          }
    }
    setExplosion (0.2, 20 * timestep);
    setBlackSmoke (0.5, 30 * timestep);
  }
}

void DynamicObj::setExplosion (float maxzoom, int len)
{
  int i;
  for (i = 0; i < maxexplosion; i ++) // search a free explosion instance
    if (explosion [i]->ttl <= 0)
      break;
  if (i >= maxexplosion) i = 0;
  explosion [i]->setExplosion (tl.x, tl.y, tl.z, force.x, force.y, force.z, maxzoom, len);
}

void DynamicObj::setBlackSmoke (float maxzoom, int len)
{
  int i;
  for (i = 0; i < maxblacksmoke; i ++) // search a free blacksmoke instance
    if (blacksmoke [i]->ttl <= 0)
      break;
  if (i >= maxblacksmoke) i = 0;
  blacksmoke [i]->setBlackSmoke (tl.x, tl.y, tl.z, currot.phi, maxzoom, len);
}

// return heading difference towards enemy
int DynamicObj::getAngle (DynamicObj *o)
{
  float dx2 = o->tl.x - tl.x, dz2 = o->tl.z - tl.z;
  int a, w = (int) currot.phi;
  if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;

  a = (int) (atan (dx2 / dz2) * 180 / PI);
  if (dz2 > 0)
  {
    if (dx2 > 0) a -= 180;
    else a += 180;
  }
  int aw = a - w;
  if (aw < -180) aw += 360;
  if (aw > 180) aw -= 360;
  return aw;
}

// return elevation difference towards enemy
int DynamicObj::getAngleH (DynamicObj *o)
{
  float disttarget = distance (o);
  return (int) (atan ((o->tl.y - tl.y) / disttarget) * 180 / PI - (currot.gamma - 180));
}

// check for a looping, this is tricky :-)
bool DynamicObj::checkLooping ()
{
  if (currot.gamma > 270)
  {
    currot.gamma = 540 - currot.gamma;
    currot.theta += 180;
    currot.phi += 180;
    recrot.theta += 180;
    if (currot.theta >= 360) currot.theta -= 360;
    if (recrot.theta >= 360) recrot.theta -= 360;
    if (currot.phi >= 360) currot.phi -= 360;
    return true;
  }
  else if (currot.gamma < 90)
  {
    currot.gamma = 180 - currot.gamma;
    currot.theta += 180;
    currot.phi += 180;
    recrot.theta += 180;
    if (currot.theta >= 360) currot.theta -= 360;
    if (recrot.theta >= 360) recrot.theta -= 360;
    if (currot.phi >= 360) currot.phi -= 360;
    return true;
  }
  return false;
}

// discrete movement, called about timestep times per second, timer-dependant, currently without extra thread (GLUT)!
void DynamicObj::move (Uint32 dt, float camphi, float camgamma)
{
  if (dt <= 0) return;
  if (realspeed <= 0) realspeed = 1.0F;

  float brakepower = 1.0F;
  float timefac = (float) dt / (float) timestep;

  checkExplosion (dt); // check if this object is exploding
  if (sink > 0) // only ships (they will not explode)
  {
    tl.y -= 0.02 * timefac; // sink down
    currot.gamma = recrot.gamma = 180.0 + 0.5 * (float) sink / timestep; // change angle when sinking
    return; // and exit move()
  }
  if (!active && !draw) return; // exit if not active

  if (id >= STATIC_PASSIVE) // only buildings, static objects
  {
    if (id == STATIC_TENT1) currot.theta = 178;
    // set the correct angles to diplay
    rot.set ((short) (90 + currot.gamma - 180), (short) currot.theta + 180, (short) -currot.phi);
    checkShield ();
    return; // and exit this function
  }

  if (id == FLARE1) // only flares
  {
    tl.y -= 0.04 * timefac; // fall down (gravity, constant)
    zoom = 0.12F + 0.03F * sin ((float) ttl / (float) timestep / 15); // blink (high frequency)
    currot.phi = camphi; // angles to viewer (player)
    currot.theta = 0;
    currot.gamma = camgamma;
  }

  if (id == CHAFF1) // only chaff
  {
    tl.y -= 0.04 * timefac; // fall down (gravity, constant)
    zoom = 0.12F + 0.01F * (80 * timestep - ttl) / timestep; // spread out
    currot.phi = camphi; // angles to viewer (player)
    currot.theta = 0;
    currot.gamma = camgamma;
  }

  // check maximum gamma
  if (easymodel == 1)
  {
    if (currot.gamma > 180 + maxrot.gamma) currot.gamma = 180 + maxrot.gamma;
    else if (currot.gamma < 180 - maxrot.gamma) currot.gamma = 180 - maxrot.gamma;
  }
  else if (easymodel == 2) // otherwise check for value overflow due to loops
  {
    (void) checkLooping ();
  }

  // the core of directional alterations and force calculations:
  // easymodel==1 means to change heading due to roll angle
  // this may seem complete nonsense for fighters, but it is just a simplification!!!
  // angle / aileron = constant, thats enough for a simple AI
  if (easymodel == 1)
  {
    if (id >= MOVING_GROUND)
    {
      currot.phi += SIN(currot.theta) * manoeverability * 667 * timefac; //10.0 * maxthrust / div;
    }
    else
    {
      currot.phi += SIN(currot.theta) * manoeverability * (3.33 + 15.0 * realspeed) * timefac;
      currot.gamma -= fabs (SIN(currot.theta) * COS(currot.gamma) / realspeed / 20) * timefac; // realistic modification
      if (currot.gamma < 180 - maxrot.gamma) currot.gamma = 180 - maxrot.gamma;
    }
  }
  else if (easymodel == 2) // now this is much more general, however simplified:
  {
    int vz = 1;
    if (currot.gamma < 90 || currot.gamma > 270)
      vz = -1;
    // change heading and elevation due to ailerons and rudder
    if (maxthrust + thrust <= -0.00001 || maxthrust + thrust >= 0.00001)
    {
      currot.phi += vz * SIN(currot.theta) * elevatoreffect * manoeverability * (3.33 + 15.0 * realspeed) * timefac;
      currot.gamma += COS(currot.theta) * elevatoreffect * manoeverability * (3.33 + 15.0 * realspeed) * timefac;
      currot.phi += -vz * COS(currot.theta) * ruddereffect * manoeverability * (0.66 + 3.0 * realspeed) * timefac;
      currot.gamma += SIN(currot.theta) * ruddereffect * manoeverability * (0.66 + 3.0 * realspeed) * timefac;
      if (!realism)
        currot.gamma -= fabs (SIN(currot.theta) * COS(currot.gamma) / realspeed / 20) * timefac; // realistic modification
    }
    // change roll due to roll ;-)
    if (rolleffect)
    {
      currot.theta += rolleffect * (nimbility * (1.0 + realspeed)) * timefac * 5.0F;
      recrot.theta = currot.theta;
    }
  }
  if (currot.phi < 0) currot.phi += 360.0; // validate heading
  else if (currot.phi >= 360.0) currot.phi -= 360.0;

  if (easymodel == 1) // easy model restrictions
  {
    if (recrot.theta > maxrot.theta) recrot.theta = maxrot.theta;
    else if (recrot.theta < -maxrot.theta) recrot.theta = -maxrot.theta;
    if (recrot.gamma > 180 + maxrot.gamma) recrot.gamma = 180 + maxrot.gamma;
    else if (recrot.gamma < 180 - maxrot.gamma) recrot.gamma = 180 - maxrot.gamma;
  }
  else if (easymodel == 2)
  {
    if (currot.theta < -180 && recrot.theta < -180)
    { recrot.theta += 360; currot.theta += 360; }
    else if (currot.theta >= 180 && recrot.theta >= 180)
    { recrot.theta -= 360; currot.theta -= 360; }
  }

  if (recthrust > maxthrust) // check maximum throttle
    recthrust = maxthrust;
  
  float throttlechange = maxthrust / 200 * timefac;
  if (recthrust > thrust + throttlechange) // alter throttle effect slowly
  {
    thrust += throttlechange;
  }
  else if (recthrust < thrust - throttlechange)
  {
    thrust -= throttlechange;
  }

  // PHYSICS (simplified model)

  Vector3 vaxis, uaxis, utemp, utemp2, utemp3;
  float gammaup, phiup, thetaup;

  bool stop;
  float gravityforce;

  if (id <= CANNON2)
  {
    tl.x += force.x * timefac; // add our vector to the translation
    tl.z += force.z * timefac;
    tl.y += force.y * timefac;
    goto cannondone; // jump down to decrease ttl and test collision
  }

  // axis pointing through the fighter's nose
  vaxis.set (COS(currot.gamma) * SIN(currot.phi), SIN(currot.gamma), COS(currot.gamma) * COS(currot.phi));

  if (realism)
  {

    // axis pointing upwards through the cockpit
    gammaup = currot.gamma + 90;
    thetaup = -currot.theta;
    phiup = currot.phi;
    uaxis.set (COS(gammaup) * SIN(phiup), SIN(gammaup), COS(gammaup) * COS(phiup)); // upward axis (theta = 0)
    // now rotate around vaxis using theta
    utemp.set (uaxis);
    utemp.mul (COS(thetaup));
    utemp2.set (vaxis);
    utemp2.mul ((1 - COS(thetaup)) * uaxis.dotproduct (vaxis));
    utemp3.set (uaxis);
    utemp3.crossproduct (vaxis);
    utemp3.mul (SIN(thetaup));
    utemp.add (utemp2);
    utemp.add (utemp3);
    uaxis.set (utemp);

  }

  realspeed = sqrt (force.x * force.x + force.z * force.z + force.y * force.y);

  if (realism) // sim model
  {

    // add drag force
    braking = (fabs (ruddereffect) + fabs (elevatoreffect) * 4 + fabs (rolleffect)) * realspeed / 50;
    brakepower = pow (0.93 - braking, timefac);

    acc.x *= brakepower;
    acc.y *= brakepower;
    acc.z *= brakepower;

    // add throttle force
    acc.z += thrust * vaxis.z * 0.3 * timefac;
    acc.x += thrust * vaxis.x * 0.3 * timefac;
    acc.y -= thrust * vaxis.y * 0.3 * timefac;
    // add elevation force
    acc.z += thrust * uaxis.z * 0.067 * timefac;
    acc.x += thrust * uaxis.x * 0.067 * timefac;
    acc.y -= thrust * uaxis.y * 0.067 * timefac;
    // add gravity force
    acc.y -= 0.015 * timefac;
    // add our vector to the translation
    float stepfac = 0.24;
    tl.x += acc.x * timefac * stepfac;
    tl.z += acc.z * timefac * stepfac;
    tl.y += acc.y * timefac * stepfac;
    float scalef = 1.1;
    force.x = acc.x * stepfac * scalef;
    force.y = acc.y * stepfac * scalef;
    force.z = acc.z * stepfac * scalef;

  }
  else // action model
  {

    // and correct the speedvector

    force.z = vaxis.z * realspeed;
    force.x = vaxis.x * realspeed;
    force.y = -vaxis.y * realspeed;

    // add throttle force

    force.z += thrust * vaxis.z * 0.01 * timefac; //0.03 and braking=0.97 by try and error
    force.x += thrust * vaxis.x * 0.01 * timefac;
    force.y -= thrust * vaxis.y * 0.01 * timefac;

    gravityforce = sqrt (realspeed) * vaxis.y * 0.0012 * timefac;
    force.z += gravityforce * vaxis.z;
    force.x += gravityforce * vaxis.x;
    force.y -= gravityforce * vaxis.y;

    // drag force simulated by adjusting the vector

    if (easymodel == 2)
      braking = (fabs (ruddereffect) + fabs (elevatoreffect)) * realspeed / 50;
    else
      braking = (fabs (currot.theta / 45)) * realspeed / 50;
    brakepower = pow (0.9915 - braking, timefac);
    force.x *= brakepower; force.z *= brakepower; force.y *= brakepower;

  }

  stop = false;
  if (id >= TANK1 && id <= TANK2) // tanks cannot climb steep faces
  {
    float newy = l->getExactHeight (tl.x + force.x, tl.z + force.z) + zoom / 2;
    if (fabs (newy - tl.y) > 0.05)
      stop = true;
    else if (fabs (newy - tl.y) > 2)
      stop = false;
  }

  if (!realism)
    if (!stop)
    {
      tl.x += force.x * timefac; // add our vector to the translation
      tl.z += force.z * timefac;
      tl.y += force.y * timefac;
    }

  // calculate the objects real thrust only once
  realspeed = sqrt (force.x * force.x + force.z * force.z + force.y * force.y);

  // objects moving on the ground should always change their elevation due to the surface
  if (id >= TANK1 && id <= TANK2 && thrust > 0 && !stop)
  {
    float newy = l->getExactHeight (tl.x, tl.z) + zoom / 2;
    float dy = newy - tl.y + force.y;
    float dx = fabs (force.x) + fabs (force.z);
    float gamma2 = 0;
    if (fabs (dx) > 0.0001)
      gamma2 = atan (dy / dx);
    currot.gamma = 180.0 + 180.0 / PI * gamma2;
    tl.y = newy;
  }

  if (id != ASTEROID)
  {
    // set angles to correctly display the object
    rot.set ((short) (90 + currot.gamma - 180), (short) currot.theta + 180, (short) -currot.phi);
  }
  else // asteroids should rotate around their center of weight, as we must not change theta/gamma, we do this here
  {
    ttl -= dt; // we use the ttl value as timer, for other methods ttl<0 is the same as ttl=-1
    if (ttl <= -360 * timestep) ttl = -1;
    int rot1 = (int) (sin ((zoom - 1.3) * 8) * 4);
    int rot2 = (int) (cos ((zoom - 1.3) * 8) * 4);
    rot.set ((short) (90 + currot.gamma + ttl * rot1 / timestep - 180), (short) currot.theta + ttl * rot2 / timestep + 180, (short) -currot.phi);
  }

cannondone:;
  if (ttl > 0)
  {
    ttl -= dt; // decrease time to live
    if (ttl <= 0)
    {
      ttl = -1;
      if (id >= MISSILE1 && id <= MISSILE2) recheight = -10; // missiles drop
      else deactivate (); // cannon shots vanish
    }
  }
  checkShield (); // check shield issues
  crashGround (dt); // check ground collision
  if (immunity > 0) immunity -= dt; // decrease immunity
}

#endif