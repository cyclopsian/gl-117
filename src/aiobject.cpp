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

#include "aiobject.h"

#include "glland.h"
#include "main.h"

int DynamicObj::net_write ()
{
  net [0] = '.';
  int z = 1;
  memcpy (&net [z], &tl->x, sizeof (&tl->x));
  z += sizeof (&tl->x);
  memcpy (&net [z], &tl->y, sizeof (&tl->y));
  z += sizeof (&tl->y);
  memcpy (&net [z], &tl->z, sizeof (&tl->z));
  z += sizeof (&tl->z);
  memcpy (&net [z], &phi, sizeof (&phi));
  z += sizeof (&phi);
  memcpy (&net [z], &theta, sizeof (&theta));
  z += sizeof (&theta);
  memcpy (&net [z], &gamma, sizeof (&gamma));
  z += sizeof (&gamma);
  return z;
}

void DynamicObj::net_read ()
{
  int z = 1;
  memcpy (&tl->x, &net [z], sizeof (&tl->x));
  z += sizeof (&tl->x);
  memcpy (&tl->y, &net [z], sizeof (&tl->y));
  z += sizeof (&tl->y);
  memcpy (&tl->z, &net [z], sizeof (&tl->z));
  z += sizeof (&tl->z);
  memcpy (&phi, &net [z], sizeof (&phi));
  z += sizeof (&phi);
  memcpy (&theta, &net [z], sizeof (&theta));
  z += sizeof (&theta);
  memcpy (&gamma, &net [z], sizeof (&gamma));
  z += sizeof (&gamma);
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
  rot->a = 90;
  phi = 0; theta = 0; gamma = 180;
  rectheta = 0;
  tl->z = 0; tl->x = 0;
  forcex = 0; forcez = 0; forcey = 0;
  maxthrust = 0.3; braking = 0/*0.99*/; manoeverability = 0.5;
  thrust = maxthrust; recthrust = thrust; recheight = 5.0;// height = 5.0;
  ttl = -1;
  shield = 1; maxshield = 1;
  immunity = 0;
//    controls = false;
  recgamma = 180;
  id = CANNON1;
  impact = 7;
  source = NULL;
  points = 0;
  party = 0;
  easymodel = true; // easy model
  elevatoreffect = 0;
  ruddereffect = 0;
  rolleffect = 0;
  maxgamma = 80;
  maxtheta = 90;
  gamma = 180;
  theta = 0;
  explode = 0;
  sink = 0;
  nimbility = 1.0;
  fighterkills = 0;
  shipkills = 0;
  tankkills = 0;
  otherkills = 0;
  killed = false;
}

DynamicObj::DynamicObj ()
{
  dinit ();
}

DynamicObj::DynamicObj (Space *space2, CModel *o2, float zoom2)
{
  this->space = space2;
  o = o2;
  zoom = zoom2;
  dinit ();
  space->addObject (this);
}

/*  void toLeft ()
{
  rectheta += manoeverability * 4;
//  if (theta > 360.0) theta -= 360.0;
}

void toRight ()
{
  rectheta -= manoeverability * 4;
//    if (theta < -360.0) theta += 360.0;
}*/

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
  float dx = target->tl->x - tl->x;
  float dz = target->tl->z - tl->z;
  float dy = target->tl->y - tl->y;
  return sqrt (dx * dx + dz * dz + dy * dy);
}

// check whether the object is exploding or sinking and deactivate if necessary
void DynamicObj::checkExplosion (Uint32 dt)
{
  if (explode > 0)
  {
    if (explode == 1)
    {
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
        setExplosion (zoom * 2, 30 * timestep);
        setBlackSmoke (1.0, 60 * timestep);
      }
    }
    if (explode >= 35 * timestep)
    {
      deactivate (); ttl = -1;
    }
    explode += dt;
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
//    printf ("\nshield=%d", shield); fflush (stdout);
}

// check whether the object collides on the ground and alter gamma and y-translation
void DynamicObj::crashGround (Uint32 dt)
{
  if (id >= MOVING_GROUND)
    return;
  float height = tl->y - l->getExactHeight (tl->x, tl->z);
  if (height < zoom)
  {
    tl->y -= (height - zoom);
    gamma += 10;
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
    shield -= 1; // time independant
//      shield += (int) height - 2;
  }
  // restrict to a maximum height, we want an action game!!! a little bit more now 50 -> 80
  if (height > 80) tl->y = l->getHeight (tl->x, tl->z) + 80;
}

// check for collision, simplified model, each model is surrounded by a cube
// this works pretty well, but we must use more than one model for complex models or scenes
void DynamicObj::collide (DynamicObj *d, Uint32 dt) // d must be the medium (laser, missile)
{
  if (immunity > 0 || d->immunity > 0) return;
  if (explode > 0 || sink > 0) return;
  float z = d->zoom > zoom ? d->zoom : zoom;
  if ((id >= MISSILE1 && id <= MISSILE2) || (d->id >= MISSILE1 && d->id <= MISSILE2))
  {
    if (d->party != party)
      z *= 2.0; // missiles need not really hit the fighter, but will explode near it
  }
  if (id >= STATIC_PASSIVE)
    z *= 0.65;
  if ((id >= SHIP1 && id <= SHIP2) || (d->id >= SHIP1 && d->id <= SHIP2))
    z *= 0.3;
  if (id == ASTEROID)
    z *= 1.15;
/*    else if ((id >= CANNON1 && id <= CANNON2) || (d->id >= CANNON1 && d->id <= CANNON2))
    z *= 1.0;
  else if (id >= FLAK1 && id <= FLAK2)
    z *= 1.0;*/
  
  bool collide = false;
  if (id == STATIC_BARRIER1 || d->id == STATIC_BARRIER1)
  {
    if (tl->x >= d->tl->x - 1.0F && tl->x <= d->tl->x + 1.0F &&
        tl->y >= d->tl->y - 10 && tl->y <= d->tl->y + 10 &&
        tl->z >= d->tl->z - 10 && tl->z <= d->tl->z + 10)
    {
      collide = true;
    }
  }
  else
  {
    if (tl->x >= d->tl->x - z && tl->x <= d->tl->x + z &&
        tl->y >= d->tl->y - z && tl->y <= d->tl->y + z &&
        tl->z >= d->tl->z - z && tl->z <= d->tl->z + z)
    {
      collide = true;
    }
  }

  if (collide)
  {
    if (id < STATIC_PASSIVE || (id >= STATIC_PASSIVE && d->id >= MISSILE1 && d->id <= MISSILE2))
      shield -= (float) d->impact * dt / timestep;
    else
      shield -= 2.0F * dt / timestep;
    d->shield -= (float) impact * dt / timestep;
//      printf (" c(%d,%d)=>s(%d,%d)\n", id, d->id, shield, d->shield);
    if (d->source != NULL && active) // only for missiles/cannons
    {
      if (d->source->party != party) // calculate points
      {
        if (maxshield < 2000)
          d->source->points += (int) impact; // extra points for shooting an enemy object
      }
      else
      {
        d->source->points -= (int) impact; // subtract points for shooting an own object
      }

      if (shield <= 0)
        if (d->source->party != party && active && draw && !killed)
          if (d->source->id >= FIGHTER1 && d->source->id <= FIGHTER2)
          {
            killed = true;
            if (id >= FIGHTER1 && id <= FIGHTER2)
              d->source->fighterkills ++;
            else if (id >= SHIP1 && id <= SHIP2)
              d->source->shipkills ++;
            else if ((id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
              d->source->tankkills ++;
            else
              d->source->otherkills ++;
          }
    }
    setExplosion (0.2, 20 * timestep);
    setBlackSmoke (0.5, 30 * timestep);
//      printf ("collision"); fflush (stdout);
  }
}

void DynamicObj::setExplosion (float maxzoom, int len)
{
  int i;
  for (i = 0; i < maxexplosion; i ++) // search a free explosion instance
    if (explosion [i]->ttl <= 0)
      break;
  if (i >= maxexplosion) i = 0;
  explosion [i]->setExplosion (tl->x, tl->y, tl->z, maxzoom, len);
}

void DynamicObj::setBlackSmoke (float maxzoom, int len)
{
  int i;
  for (i = 0; i < maxblacksmoke; i ++) // search a free blacksmoke instance
    if (blacksmoke [i]->ttl <= 0)
      break;
  if (i >= maxblacksmoke) i = 0;
  blacksmoke [i]->setBlackSmoke (tl->x, tl->y, tl->z, phi, maxzoom, len);
}

// return heading difference towards enemy
int DynamicObj::getAngle (DynamicObj *o)
{
  float dx2 = o->tl->x - tl->x, dz2 = o->tl->z - tl->z;
  int a, w = (int) phi;
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
  return (int) (atan ((o->tl->y - tl->y) / disttarget) * 180 / PI - (gamma - 180));
}

// check for a looping, this is tricky :-)
bool DynamicObj::checkLooping ()
{
  if (gamma > 270)
  {
    gamma = 540 - gamma;
    theta += 180;
    phi += 180;
    rectheta += 180;
    if (theta >= 360) theta -= 360;
    if (rectheta >= 360) rectheta -= 360;
    if (phi >= 360) phi -= 360;
    return true;
  }
  else if (gamma < 90)
  {
    gamma = 180 - gamma;
    theta += 180;
    phi += 180;
    rectheta += 180;
    if (theta >= 360) theta -= 360;
    if (rectheta >= 360) rectheta -= 360;
    if (phi >= 360) phi -= 360;
    return true;
  }
  return false;
}

// discrete movement, called about timestep times per second, timer-dependant, currently without extra thread (GLUT)!
void DynamicObj::move (Uint32 dt)
{
  if (dt <= 0) return;
  if (realspeed <= 0) realspeed = 1.0F;

  float brakepower = 1.0F;
  float timefac = (float) dt / (float) timestep;

  checkExplosion (dt); // check if this object is exploding
  if (sink > 0) // only ships (they will not explode)
  {
    tl->y -= 0.02 * timefac; // sink down
    gamma = recgamma = 180.0 + 0.5 * (float) sink / timestep; // change angle when sinking
    return; // and exit move()
  }
  if (!active) return; // exit if not active

  if (id >= STATIC_PASSIVE) // only buildings, static objects
  {
    if (id == STATIC_TENT1) theta = 178;
    // set the correct angles to diplay
    rot->setAngles ((short) (90 + gamma - 180), (short) theta + 180, (short) -phi);
    checkShield ();
    return; // and exit this function
  }

  if (id == FLARE1) // only flares
  {
    tl->y -= 0.04 * timefac; // fall down (gravity, constant)
    zoom = 0.12F + 0.03F * sin ((float) ttl / (float) timestep / 15); // blink (high frequency)
    phi = camphi; // angles to viewer (player)
    theta = 0;
    gamma = camgamma;
  }

  if (id == CHAFF1) // only chaff
  {
    tl->y -= 0.04 * timefac; // fall down (gravity, constant)
    zoom = 0.12F + 0.01F * (80 * timestep - ttl) / timestep; // spread out
    phi = camphi; // angles to viewer (player)
    theta = 0;
    gamma = camgamma;
  }

  // check maximum gamma
  if (easymodel == 1)
  {
    if (gamma > 180 + maxgamma) gamma = 180 + maxgamma;
    else if (gamma < 180 - maxgamma) gamma = 180 - maxgamma;
  }
  else if (easymodel == 2) // otherwise check for value overflow due to loops
  {
    (void) checkLooping ();
  }

  // get a normalized theta, as our sine/cosi tables only reach from 0 to 359
  // however SIN/COS macro will call math.h functions, see common.h
/*  int theta0 = (int) theta;
  while (theta0 < 0) theta0 += 360;
  while (theta0 >= 360) theta0 -= 360;*/

  // the core of directional alterations and force calculations:
  // easymodel==1 means to change heading due to roll angle
  // this may seem complete nonsense for fighters, but it is just a simplification!!!
  // angle / aileron = constant, thats enough for a simple AI
  if (easymodel == 1)
  {
    if (id >= MOVING_GROUND)
    {
      phi += SIN(theta) * manoeverability * 667 * timefac; //10.0 * maxthrust / div;
    }
/*    if (id >= FLAK1 && id <= FLAK2)
    {
      phi += sine [theta0] * manoeverability * 10.0 * timefac;
    }*/
    else
    {
//      float div = maxthrust + thrust;
//      if (div == 0) div = 1;
      phi += SIN(theta) * manoeverability * 6.67 * timefac; //10.0 * maxthrust / div;
      gamma -= fabs (SIN(theta) * COS(gamma) / realspeed / 20) * timefac; // realistic modification
    }
  }
  else if (easymodel == 2) // now this is much more general, however simplified:
  {
    int vz = 1;
    if (gamma < 90 || gamma > 270)
      vz = -1;
    // change heading and elevation due to ailerons and rudder
    if (maxthrust + thrust <= -0.00001 || maxthrust + thrust >= 0.00001)
    {
/*
      phi += vz * SIN(theta0) * elevatoreffect * manoeverability * 6.67; // 10.0 * maxthrust / (maxthrust + thrust);
      gamma += COS(theta0) * elevatoreffect * manoeverability * 6.67; // 10.0 * maxthrust / (maxthrust + thrust);
      phi += -vz * COS(theta0) * ruddereffect * manoeverability * 1.33; // 2.0 * maxthrust / (maxthrust + thrust);
      gamma += SIN(theta0) * ruddereffect * manoeverability * 1.33; // 2.0 * maxthrust / (maxthrust + thrust);
      gamma -= sqrt (SIN(theta0) * SIN(theta0) * COS(gamma) * COS(gamma)); // realistic modification
*/
      phi += vz * SIN(theta) * elevatoreffect * manoeverability * (3.33 + 15.0 * realspeed) * timefac;
      gamma += COS(theta) * elevatoreffect * manoeverability * (3.33 + 15.0 * realspeed) * timefac;
      phi += -vz * COS(theta) * ruddereffect * manoeverability * (0.66 + 3.0 * realspeed) * timefac;
      gamma += SIN(theta) * ruddereffect * manoeverability * (0.66 + 3.0 * realspeed) * timefac;
      gamma -= fabs (SIN(theta) * COS(gamma) / realspeed / 20) * timefac; // realistic modification
    }
    // change roll due to roll ;-)
    if (rolleffect)
    {
//      rectheta += rolleffect;
//      rectheta += rolleffect * (0.5 + 1.5 * realspeed) * timefac;
      theta += rolleffect * (nimbility * (1.0 + realspeed)) * timefac * 5.0F;
      rectheta = theta;
    }
  }
  if (phi < 0) phi += 360.0; // validate heading
  else if (phi >= 360.0) phi -= 360.0;

  if (easymodel == 1) // easy model restrictions
  {
    if (rectheta > maxtheta) rectheta = maxtheta;
    else if (rectheta < -maxtheta) rectheta = -maxtheta;
    if (recgamma > 180 + maxgamma) recgamma = 180 + maxgamma;
    else if (recgamma < 180 - maxgamma) recgamma = 180 - maxgamma;
  }
  else if (easymodel == 2)
  {
    if (theta < -180 && rectheta < -180)
    { rectheta += 360; theta += 360; }
    else if (theta >= 180 && rectheta >= 180)
    { rectheta -= 360; theta -= 360; }
  }

  if (recthrust > maxthrust) // check maximum throttle
    recthrust = maxthrust;
  if (recthrust > thrust) // alter throttle effect slowly
    thrust += maxthrust / 60 * timefac;
  else if (recthrust < thrust)
    thrust -= maxthrust / 60 * timefac;

  // PHYSICS (simplified model)

  CVector3 vaxis, uaxis, utemp, utemp2, utemp3;
  float gammaup, phiup, thetaup;

  bool stop;
  float gravityforce;

  if (id <= CANNON2)
  {
    tl->x += forcex * timefac; // add our vector to the translation
    tl->z += forcez * timefac;
    tl->y += forcey * timefac;
    goto cannondone; // jump down to decrease ttl and test collision
  }

  // axis pointing through the fighter's nose
  vaxis.set (COS(gamma) * SIN(phi), SIN(gamma), COS(gamma) * COS(phi));

/*
  // axis pointing upwards through the cockpit
  gammaup = gamma + 90;
  thetaup = theta;
  phiup = phi;
  uaxis.set (COS(gammaup) * SIN(phiup), SIN(gammaup), COS(gammaup) * COS(phiup)); // upward axis (theta = 0)
  // now rotate around vaxis using theta
  utemp.take (&uaxis);
  utemp.mul (COS(thetaup));
  utemp2.take (&vaxis);
  utemp2.mul ((1 - COS(thetaup)) * uaxis.dotproduct (&vaxis));
  utemp3.take (&uaxis);
  utemp3.crossproduct (&vaxis);
  utemp3.mul (SIN(thetaup));
  utemp.add (&utemp2);
  utemp.add (&utemp3);
  uaxis.take (&utemp);
*/

  realspeed = sqrt (forcex * forcex + forcez * forcez + forcey * forcey);

/*
// Proposal:
  // add drag force
  forcex *= 0.1;
  forcey *= 0.1;
  forcez *= 0.1;
  // add throttle force
  forcez += thrust * vaxis.z * 0.02;
  forcex += thrust * vaxis.x * 0.02;
  forcey -= thrust * vaxis.y * 0.02;
  // add elevation force
  forcez -= thrust * uaxis.z * 0.006;
  forcex -= thrust * uaxis.x * 0.006;
  forcey -= thrust * uaxis.y * 0.006;
  // add gravity force
  forcey -= 0.0012;
  // add our vector to the translation
  tl->x += forcex * timefac * timefac * 30;
  tl->z += forcez * timefac * timefac * 30;
  tl->y += forcey * timefac * timefac * 30;
*/

  // and correct the speedvector

  forcez = vaxis.z * realspeed;
  forcex = vaxis.x * realspeed;
  forcey = -vaxis.y * realspeed;

  // add throttle force

  forcez += thrust * vaxis.z * 0.01 * timefac; //0.03 and braking=0.97 by try and error
  forcex += thrust * vaxis.x * 0.01 * timefac;
  forcey -= thrust * vaxis.y * 0.01 * timefac;

  gravityforce = sqrt (realspeed) * vaxis.y * 0.0012 * timefac;
 //   printf ("id: %d %f \n ",id, gravityforce); fflush (stdout);
  forcez += gravityforce * vaxis.z;
  forcex += gravityforce * vaxis.x;
  forcey -= gravityforce * vaxis.y;

  // drag force simulated by adjusting the vector

  braking = (fabs (ruddereffect) + fabs (elevatoreffect)) * realspeed / 50;
  brakepower = pow (0.9915 - braking, timefac);
  forcex *= brakepower; forcez *= brakepower; forcey *= brakepower;

  stop = false;
  if (id >= TANK1 && id <= TANK2) // tanks cannot climb steep faces
  {
    float newy = l->getExactHeight (tl->x + forcex, tl->z + forcez) + zoom / 2;
    if (fabs (newy - tl->y) > 0.05)
      stop = true;
    else if (fabs (newy - tl->y) > 2)
      stop = false;
  }
  if (!stop)
  {
    tl->x += forcex * timefac; // add our vector to the translation
    tl->z += forcez * timefac;
    tl->y += forcey * timefac;
  }

//  tl->y = l->getHeight (tl->x, tl->z)+5;
//  gamma = 180;

  // calculate the objects real thrust only once
  realspeed = sqrt (forcex * forcex + forcez * forcez + forcey * forcey);

//  if (id == 200)
//    printf ("%d: (%f, %f, %f), %f\n", dt, forcex, forcey, forcez, realspeed); fflush (stdout);

  // objects moving on the ground should always change their elevation due to the surface
  if (id >= TANK1 && id <= TANK2 && thrust > 0 && !stop)
  {
    float newy = l->getExactHeight (tl->x, tl->z) + zoom / 2;
    float dy = newy - tl->y + forcey;
    float dx = fabs (forcex) + fabs (forcez);
    float gamma2 = 0;
    if (fabs (dx) > 0.0001)
      gamma2 = atan (dy / dx);
    gamma = 180.0 + 180.0 / PI * gamma2;
    tl->y = newy;
  }

  if (id != ASTEROID)
  {
    // set angles to correctly display the object
    rot->setAngles ((short) (90 + gamma - 180), (short) theta + 180, (short) -phi);
  }
  else // asteroids should rotate around their center of weight, as we must not change theta/gamma, we do this here
  {
    ttl -= dt; // we use the ttl value as timer, for other methods ttl<0 is the same as ttl=-1
    if (ttl <= -360 * timestep) ttl = -1;
    int rot1 = (int) (sin ((zoom - 1.3) * 8) * 4);
    int rot2 = (int) (cos ((zoom - 1.3) * 8) * 4);
    rot->setAngles ((short) (90 + gamma + ttl * rot1 / timestep - 180), (short) theta + ttl * rot2 / timestep + 180, (short) -phi);
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



void AIObj::aiinit ()
{
  int i;
  acttype = 0;
  intelligence = 100;
  aggressivity = 100;
  precision = 100;
  shield = 10;
  ai = true;
  active = true;
  draw = true;
  target = NULL;
//    controls = true;
  dtheta = 0;
  dgamma = 0;
  id = MISSILE1;
  impact = 30;
  manoevertheta = 0;
  manoeverheight = 0;
  manoeverthrust = 0;
  idle = 0;
  smokettl = 0;
  firecannonttl = 0;
  firemissilettl = 0;
  fireflarettl = 0;
  firechaffttl = 0;
  flares = 0;
  aw = 0;
  source = NULL;
  points = 0;
  easymodel = 1;
  elevatoreffect = 0;
  ruddereffect = 0;
  gamma = 180;
  recgamma = 180;
  dgamma = 0;
  theta = 0;
  maxgamma = 80;
  maxtheta = 90;
  missiletype = 0;
  autofire = false;
  ttl = -1;
  ttf = 30 * timestep;
  score = -1;
  for (i = 0; i < missiletypes; i ++)
    missiles [i] = 0;
  for (i = 0; i < missileracks; i ++)
  {
    missilerack [i] = -1;
    missilerackn [i] = 0;
  }
  bomber = 0;
  timer = 0;
  ammo = -1;
}

void AIObj::missileCount ()
{
  if (id < FIGHTER1 || id > FIGHTER2) return;
  
  int i;
  for (i = 0; i < missiletypes; i ++)
    missiles [i] = 0;
  for (i = 0; i < missileracks; i ++)
  {
    if (missilerackn [i] > 0)
    {
      missiles [missilerack [i]] += missilerackn [i];
    }
  }
}

void AIObj::newinit (int id, int party, int intelligence, int precision, int aggressivity)
{
  int i;
  ai = true;
  this->id = id;
  this->party = party;
  activate ();
  for (i = 0; i < missileracks; i ++)
    missilerackn [i] = 0;
  ammo = -1;
  bomber = 0;
  if (id == FIGHTER_FALCON)
  {
    maxthrust = 0.31;
    nimbility = 0.8;
    manoeverability = 0.45;
    maxshield = 85;
    zoom = 0.35;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missilerackn [0] = 2; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 2;
    missilerack [0] = 0; missilerack [1] = 6; missilerack [2] = 6; missilerack [3] = 0;
    flares = 20;
    chaffs = 20;
  }
  else if (id == FIGHTER_SWALLOW)
  {
    maxthrust = 0.23;
    nimbility = 0.6;
    manoeverability = 0.32;
    maxshield = 110;
    zoom = 0.43;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missilerackn [0] = 2; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 2;
    missilerack [0] = 6; missilerack [1] = 3; missilerack [2] = 3; missilerack [3] = 6;
    flares = 20;
    chaffs = 20;
    bomber = 1;
  }
  else if (id == FIGHTER_HAWK)
  {
    maxthrust = 0.26;
    nimbility = 0.62;
    manoeverability = 0.35;
    maxshield = 120;
    zoom = 0.43;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missilerackn [0] = 1; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 3; missilerack [2] = 3; missilerack [3] = 6;
    flares = 20;
    chaffs = 20;
    bomber = 1;
  }
  else if (id == FIGHTER_HAWK2)
  {
    maxthrust = 0.28;
    nimbility = 0.65;
    manoeverability = 0.38;
    maxshield = 140;
    zoom = 0.45;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missilerackn [0] = 1; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 4; missilerack [2] = 4; missilerack [3] = 6;
    flares = 20;
    chaffs = 20;
    bomber = 1;
  }
  else if (id == FIGHTER_TRANSPORT)
  {
    maxthrust = 0.14;
    maxshield = 45;
    missiles [0] = 0;
    manoeverability = 0.1;
    impact = 5;
    zoom = 1.5;
    maxgamma = 20;
    maxtheta = 30;
    flares = 0;
    chaffs = 20;
    ammo = 0;
  }
  else if (id == FIGHTER_BUZZARD)
  {
    maxthrust = 0.31;
    nimbility = 0.76;
    manoeverability = 0.43;
    maxshield = 75;
    zoom = 0.44;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missilerackn [0] = 2; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 2;
    missilerack [0] = 0; missilerack [1] = 6; missilerack [2] = 6; missilerack [3] = 0;
    flares = 20;
    chaffs = 20;
  }
  else if (id == FIGHTER_CROW)
  {
    maxthrust = 0.23;
    nimbility = 0.62;
    manoeverability = 0.35;
    maxshield = 60;
    zoom = 0.41;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missilerackn [0] = 1; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 0; missilerack [2] = 0; missilerack [3] = 6;
    flares = 20;
    chaffs = 20;
  }
  else if (id == FIGHTER_PHOENIX)
  {
    maxthrust = 0.3;
    nimbility = 0.5;
    manoeverability = 0.25;
    maxshield = 180;
    zoom = 0.47;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missilerackn [0] = 3; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 3;
    missilerack [0] = 4; missilerack [1] = 4; missilerack [2] = 4; missilerack [3] = 4;
    flares = 25;
    chaffs = 25;
    bomber = 1;
  }
  else if (id == FIGHTER_REDARROW)
  {
    maxthrust = 0.33;
    nimbility = 1;
    manoeverability = 0.51;
    maxshield = 120;
    zoom = 0.4;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missilerackn [0] = 2; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 2;
    missilerack [0] = 7; missilerack [1] = 1; missilerack [2] = 1; missilerack [3] = 7;
    flares = 25;
    chaffs = 25;
  }
  else if (id == FIGHTER_BLACKBIRD)
  {
    maxthrust = 0.28;
    nimbility = 1.02;
    manoeverability = 0.5;
    maxshield = 85;
    zoom = 0.33;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missilerackn [0] = 2; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 2;
    missilerack [0] = 7; missilerack [1] = 1; missilerack [2] = 1; missilerack [3] = 7;
    flares = 25;
    chaffs = 25;
  }
  if (id >= FIGHTER1 && id <= FIGHTER2)
  {
    recthrust = maxthrust / 2.0;
    shield = maxshield;
    thrust = recthrust = maxthrust / 2;
    smoke->type = 1;
    impact = 2;
    forcez = recthrust;
    ammo = 1000;
  }

  if (id == FLAK_AIR1)
  {
    maxthrust = 0;
    thrust = 0;
    maxgamma = 0;
    maxtheta = 0.03;
    manoeverability = 12.0;
    shield = maxshield = 80;
    zoom = 0.35;
  }
  if (id == FLARAK_AIR1)
  {
    maxthrust = 0;
    thrust = 0;
    maxgamma = 0;
    maxtheta = 0.03;
    manoeverability = 6.0;
    shield = maxshield = 70;
    zoom = 0.3;
    missiles [6] = 100;
  }

  if (id == TANK_AIR1)
  {
    maxthrust = 0.04;
    thrust = 0;
    gamma = 180; theta = 0; phi = 0;
    maxgamma = 0;
    maxtheta = 0.03;
    manoeverability = 8.0;
    shield = maxshield = 160;
    zoom = 0.35;
  }
  else if (id == TANK_GROUND1)
  {
    maxthrust = 0.04;
    thrust = 0;
    gamma = 180; theta = 0; phi = 0;
    maxgamma = 0;
    maxtheta = 0.03;
    manoeverability = 8.0;
    shield = maxshield = 200;
    zoom = 0.4;
  }
  else if (id == TANK_PICKUP1)
  {
    maxthrust = 0;
    thrust = 0.02;
    maxgamma = 0;
    maxtheta = 0.03;
    manoeverability = 0;
    shield = maxshield = 30;
    zoom = 0.25;
  }
  else if (id == TANK_TRUCK1)
  {
    maxthrust = 0;
    thrust = 0.02;
    maxgamma = 0;
    maxtheta = 0.03;
    manoeverability = 0;
    shield = maxshield = 20;
    zoom = 0.45;
  }

  if (id == SHIP_CRUISER)
  {
    zoom = 4.0;
    maxthrust = 0.05;
    thrust = 0.05;
    maxgamma = 0;
    maxtheta = 0.03;
    manoeverability = 4.0;
    impact = 20;
    shield = maxshield = 5500;
    missiles [6] = 200;
  }
  else if (id == SHIP_DESTROYER1)
  {
    zoom = 2.0;
    maxthrust = 0.05;
    thrust = 0.05;
    maxgamma = 0;
    maxtheta = 0.03;
    manoeverability = 6.0;
    impact = 20;
    shield = maxshield = 2500;
  }

  if (id == MISSILE_AIR1)
  {
    intelligence = 100;
    maxthrust = 0.48;
    nimbility = 4.5; // old 2.2
    manoeverability = 2.5;
    ttl = 200 * timestep;
    impact = 35;
  }
  else if (id == MISSILE_AIR2)
  {
    intelligence = 50;
    maxthrust = 0.49;
    nimbility = 5.5; // old 3.5
    manoeverability = 3.0;
    ttl = 220 * timestep;
    impact = 45;
  }
  else if (id == MISSILE_AIR3)
  {
    intelligence = 0;
    maxthrust = 0.50;
    nimbility = 6.5;
    manoeverability = 3.5;
    ttl = 250 * timestep;
    impact = 55;
  }
  else if (id == MISSILE_GROUND1)
  {
    intelligence = 50;
    maxthrust = 0.64;
    nimbility = 1.2;
    manoeverability = 1.0;
    ai = true;
    ttl = 250 * timestep;
    impact = 280;
  }
  else if (id == MISSILE_GROUND2)
  {
    intelligence = 0;
    maxthrust = 0.68;
    nimbility = 1.5;
    manoeverability = 1.0;
    ai = true;
    ttl = 310 * timestep;
    impact = 400;
  }
  else if (id == MISSILE_DF1)
  {
    intelligence = 0;
    maxthrust = 0.65;
    nimbility = 0.0;
    manoeverability = 0.0;
    ai = true;
    ttl = 300 * timestep;
    impact = 900;
  }
  else if (id == MISSILE_FF1)
  {
    intelligence = 0;
    maxthrust = 0.54;
    nimbility = 4.0;
    manoeverability = 2.2;
    ttl = 250 * timestep;
    impact = 40;
  }
  else if (id == MISSILE_FF2)
  {
    intelligence = 0;
    maxthrust = 0.55;
    nimbility = 4.5;
    manoeverability = 2.5;
    ttl = 250 * timestep;
    impact = 50;
  }
  else if (id == MISSILE_MINE1)
  {
    intelligence = 0;
    maxthrust = 0.1;
    if (difficulty == 1) maxthrust = 0.15;
    else if (difficulty == 2) maxthrust = 0.2;
    nimbility = 1.5;
    manoeverability = 1.0;
    ai = true;
    ttl = -1;
    impact = 500;
    zoom = 0.3;
  }

  if (id >= STATIC_PASSIVE)
  {
    intelligence = 0;
    maxthrust = 0;
    nimbility = 0;
    manoeverability = 0;
    impact = 5;
    maxtheta = 0;
    maxgamma = 0;
  }
  if (id == STATIC_TENT1)
  {
    shield = maxshield = 80;
    zoom = 0.5;
  }
  if (id == STATIC_TENT4)
  {
    shield = maxshield = 160;
    zoom = 1.2;
  }
  if (id == STATIC_CONTAINER1)
  {
    shield = maxshield = 30;
    zoom = 1.0;
    impact = 20;
  }
  if (id == STATIC_HALL1)
  {
    shield = maxshield = 450;
    zoom = 1.8;
    impact = 20;
  }
  if (id == STATIC_HALL2)
  {
    shield = maxshield = 920;
    zoom = 2.5;
    impact = 20;
  }
  if (id == STATIC_OILRIG1)
  {
    shield = maxshield = 1000;
    zoom = 3.5;
    impact = 20;
  }
  if (id == STATIC_COMPLEX1)
  {
    shield = maxshield = 6500;
    zoom = 2.0;
    impact = 20;
  }
  if (id == STATIC_RADAR1)
  {
    shield = maxshield = 500;
    zoom = 1.3;
    impact = 20;
  }
  if (id == ASTEROID)
  {
    shield = maxshield = 100000;
    zoom = 0.01 * myrandom (60) + 1.0;
    impact = 5;
    thrust = 0.25;
    maxthrust = 0.25;
    forcez = 0.12;
    ai = false;
  }
  if (id == STATIC_BASE1)
  {
    shield = maxshield = 5500;
    zoom = 4.0;
    impact = 20;
  }
  if (id == STATIC_BARRIER1)
  {
    shield = maxshield = 1000;
    zoom = 12.0;
    impact = 2000;
  }

  if (difficulty == 0) // easy
  {
    intelligence = 400 - (400 - intelligence) * 1 / 2;
    precision = 400 - (400 - precision) * 1 / 2;
    aggressivity = 400 - (400 - aggressivity) * 1 / 2;
    if (party != 1 && shield > 10) // not player party
    {
      shield = shield * 8 / 10;
      maxshield = shield;
    }
  }
  else if (difficulty == 1) // normal
  {
    intelligence = 400 - (400 - intelligence) * 3 / 4;
    precision = 400 - (400 - precision) * 3 / 4;
    aggressivity = 400 - (400 - aggressivity) * 3 / 4;
  }
  else if (difficulty == 2) // hard
  {
    if (party != 1) // not player party
    {
      intelligence = intelligence * 4 / 5;
      precision = precision * 4 / 5;
      aggressivity = aggressivity * 4 / 5;
    }
  }

  this->intelligence = intelligence;
  this->precision = precision;
  this->aggressivity = aggressivity;
  missileCount ();
}

void AIObj::newinit (int id, int party, int intelligence)
{
  newinit (id, party, intelligence, intelligence, intelligence);
}

AIObj::AIObj ()
{
  o = NULL;
  zoom = 1.0;
  aiinit ();
  smoke = new CSmoke (0);
}

AIObj::AIObj (Space *space2, CModel *o2, float zoom2)
{
  this->space = space2;
  o = o2;
  zoom = zoom2;
  aiinit ();
  smoke = new CSmoke (0);
  space->addObject (this);
}

AIObj::~AIObj ()
{
  delete smoke;
}

void AIObj::initValues (DynamicObj *dobj, float phi)
{
  float fac = zoom;
  if (dobj->id == FLARE1 || dobj->id == CHAFF1) fac = -fac;
  // Hier bitte exakte Kugelkoordinaten auf Grund von gamma und theta verwenden!
  dobj->tl->x = tl->x + COS(gamma) * SIN(phi) * fac;
  dobj->tl->y = tl->y - SIN(gamma) * fac;
  if ((id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
    dobj->tl->y += fac;
  dobj->tl->z = tl->z + COS(gamma) * COS(phi) * fac;
  dobj->phi = phi;
  dobj->theta = 0;
  dobj->rectheta = dobj->theta;
  dobj->forcex = forcex;
  dobj->forcey = forcey;
  dobj->forcez = forcez;
  dobj->rot->setAngles ((short) (90 + dobj->gamma - 180), (short) dobj->theta + 180, (short) -dobj->phi);
}

void AIObj::fireCannon (DynamicObj *laser, float phi)
{
  if (firecannonttl > 0) return;
  if (ammo == 0) return;
  ammo --;
  laser->thrust = 0;
  laser->recthrust = laser->thrust;
  laser->manoeverability = 0.0;
  laser->maxthrust = 0;
  if (target != NULL && ai)
  {
    if (target->active)
    {
      laser->gamma = 180.0 + atan ((target->tl->y - tl->y) / distance (target)) * 180.0 / pitab;
/*      float timelaser = 0.0001;
      if (fabs (laser->realspeed - target->realspeed) > 0.0001)
        timelaser = distance (target) / (laser->realspeed - target->realspeed); // urks, should be target->thrust*(norm2(v_target||v_laser))
      laser->gamma = 180;
      if (fabs (timelaser) > 0.00001 && fabs (laser->realspeed) > 0.00001)
        laser->gamma = 180.0 + atan ((target->tl->y - tl->y) / timelaser / laser->realspeed) * 180.0 / pitab;*/
    }
  }
  else
    laser->gamma = gamma; // + 90.0;
  laser->party = party;
  laser->ttl = 65 * timestep;
  laser->shield = 1;
  laser->immunity = (int) (zoom * 12) * timestep;
  laser->source = this;
  laser->phi = phi;
  initValues (laser, phi);
  float fac = 0.47F;
  if (id >= FIGHTER1 && id <= FIGHTER2) fac = 0.37F;
  laser->forcex += COS(laser->gamma) * SIN(laser->phi) * fac;
  laser->forcey -= SIN(laser->gamma) * fac;
  laser->forcez += COS(laser->gamma) * COS(laser->phi) * fac;
  laser->activate ();
  firecannonttl += 45;
}

void AIObj::fireCannon (DynamicObj **laser, float phi)
{
  int i;
  if (firecannonttl > 0) return;
  if (ammo == 0) return;
  for (i = 0; i < maxlaser; i ++)
  {
    if (!laser [i]->active) break;
  }
//    printf ("i=%d, ", i); fflush (stdout);
  if (i < maxlaser)
  {
//      printf ("e%d ", i); fflush (stdout);
    fireCannon (laser [i], phi);
//      printf ("l%d ", i); fflush (stdout);
  }
}

void AIObj::fireCannon (DynamicObj **laser)
{
  if (firecannonttl > 0) return;
  if (ammo == 0) return;
  fireCannon (laser, phi);
}

void AIObj::fireMissile2 (int id, AIObj *missile, AIObj *target)
{
  char buf [STDSIZE];
  if (debuglevel == LOG_ALL)
  {
    sprintf (buf, "Missile: party=%d, id=%d", party, id);
    display (buf, LOG_ALL);
  }
  ttf = 50 * timestep;
  missile->dinit ();
  missile->aiinit ();
  missile->newinit (id, party, 0);
  initValues (missile, phi);
  missile->id = id;
  missile->explode = 0;
  missile->thrust = thrust + 0.0005;
  missile->recthrust = 0.5;
//    missile->gamma = gamma / 2.0 + 90.0;
  missile->gamma = gamma;
  missile->target = target;
  missile->recgamma = gamma;
  missile->shield = 1;
//    missile->recheight = 5;
  missile->party = party;
  missile->immunity = (25 + (int) (zoom * 6.0)) * timestep;
  missile->dtheta = 0;
  missile->dgamma = 0;
  missile->source = this;
  missile->activate ();
  if (id >= FIGHTER1 && id <= FIGHTER2)
  {
    missile->manoeverheight = 30 * timestep;
    missile->recheight = missile->tl->y - l->getHeight (missile->tl->x, missile->tl->z) - 4;
  }
}

void AIObj::fireFlare2 (DynamicObj *flare)
{
  char buf [STDSIZE];
  if (debuglevel == LOG_ALL)
  {
    sprintf (buf, "Flare: party=%d", party);
    display (buf, debuglevel);
  }
  flare->dinit ();
  flare->thrust = 0;
  flare->realspeed = 0;
  flare->recthrust = 0;
  flare->manoeverability = 0.0;
  flare->maxthrust = 1.0;
  flare->gamma = 0;
  flare->party = party;
  flare->ttl = 80 * timestep;
  flare->shield = 1;
  flare->immunity = (int) (zoom * 12) * timestep;
  flare->source = this;
  flare->phi = phi;
  flare->id = FLARE1;
  initValues (flare, phi);
  flare->activate ();
  flare->explode = 0;
}

void AIObj::fireChaff2 (DynamicObj *chaff)
{
  char buf [STDSIZE];
  if (debug == LOG_ALL)
  {
    sprintf (buf, "Chaff: party=%d", party);
    display (buf, LOG_ALL);
  }
  chaff->dinit ();
  chaff->thrust = 0;
  chaff->realspeed = 0;
  chaff->recthrust = 0;
  chaff->manoeverability = 0.0;
  chaff->maxthrust = 1.0;
  chaff->gamma = 0;
  chaff->party = party;
  chaff->ttl = 80 * timestep;
  chaff->shield = 1;
  chaff->immunity = (int) (zoom * 12) * timestep;
  chaff->source = this;
  chaff->phi = phi;
  chaff->id = CHAFF1;
  initValues (chaff, phi);
  chaff->activate ();
  chaff->explode = 0;
  chaff->zoom = 0.12F;
}

int AIObj::firstMissile ()
{
  int i = 0;
  while (!missiles [i])
  {
    i ++;
    if (i >= missiletypes) return 0;
  }
  ttf = 50 * timestep;
  return i;
}

int AIObj::nextMissile (int from)
{
  int i = from + 1;
  if (i >= missiletypes) i = 0;
  while (!missiles [i])
  {
    i ++;
    if (i >= missiletypes) i = 0;
    if (i == from) break;
  }
  ttf = 50 * timestep;
  return i;
}

bool AIObj::haveMissile (int id)
{
  char buf [STDSIZE];
  id -= MISSILE1;
  if (id < 0 || id >= missiletypes)
  {
    sprintf (buf, "Wrong missile ID in %s, line %s", __FILE__, __LINE__);
    display (buf, LOG_ERROR);
  }
  if (missiles [id] > 0)
    return true;
  return false;
}

bool AIObj::haveMissile () // due to missiletype
{
  if (missiles [missiletype] > 0)
    return true;
  return false;
}

void AIObj::decreaseMissile (int id)
{
  char buf [STDSIZE];
  int i;
  id -= MISSILE1;
  if (id < 0 || id >= missiletypes)
  {
    sprintf (buf, "Wrong missile ID in %s, line %s", __FILE__, __LINE__);
    display (buf, LOG_ERROR);
  }
  missiles [id] --;
  int ptrrack = 0, maxrack = 0;
  for (i = 0; i < missileracks; i ++)
    if (missilerack [i] == id)
      if (missilerackn [i] > maxrack)
      {
        ptrrack = i;
        maxrack = missilerackn [i];
      }
  if (maxrack > 0)
  {
    missilerackn [ptrrack] --;
    refscale [ptrrack * 3 + 2 - missilerackn [ptrrack]] = 0;
  }
}

bool AIObj::fireMissile (int id, AIObj **missile, AIObj *target)
{
  int i;
  if (!haveMissile (id)) return false;
  if (ttf > 0) return false;
  for (i = 0; i < maxmissile; i ++)
  {
    if (missile [i]->ttl <= 0) break;
  }
  if (i < maxmissile)
  {
    fireMissile2 (id, missile [i], target);
    decreaseMissile (id);
    firemissilettl = 20 * timestep;
    return true;
  }
  return false;
}

bool AIObj::fireMissile (AIObj **missile, AIObj *target)
{
  if (ttf > 0) return false;
  return fireMissile (missiletype + MISSILE1, missile, (AIObj *) target);
}

bool AIObj::fireMissile (int id, AIObj **missile)
{
  if (ttf > 0) return false;
  return fireMissile (id, missile, (AIObj *) target);
}

bool AIObj::fireMissile (AIObj **missile)
{
  if (ttf > 0) return false;
  return fireMissile (missiletype + MISSILE1, missile);
}

bool AIObj::fireFlare (DynamicObj **flare, AIObj **missile)
{
  int i, i2;
  if (flares <= 0) return false;
  if (fireflarettl > 0) return false;
  for (i = 0; i < maxflare; i ++)
  {
    if (flare [i]->ttl <= 0) break;
  }
  if (i < maxflare)
  {
    fireFlare2 (flare [i]);
    flares --;
    fireflarettl = 8 * timestep;
    for (i2 = 0; i2 < maxmissile; i2 ++)
    {
      if (missile [i2]->ttl > 0)
      {
        if (missile [i2]->id >= MISSILE_AIR1 && missile [i2]->id <= MISSILE_AIR3) // only heat seeking missiles
          if (missile [i2]->target == this) // only change target if angle is good
          {
            bool hit = false;
            if (easymodel == 1)
            {
              if (myrandom ((int) (theta + 20)) > 50) hit = true;
            }
            else if (easymodel == 2)
            {
              if (myrandom ((int) (fabs (elevatoreffect) * 90 + 20)) > 50) hit = true;
            }
            if (hit)
            {
              if (debuglevel == LOG_ALL)
              {
                display ("Missile to flare", LOG_ALL);
              }
              missile [i2]->target = flare [i];
            }
          }
      }
    }
    return true;
  }
  return false;
}

bool AIObj::fireChaff (DynamicObj **chaff, AIObj **missile)
{
  int i, i2;
  if (chaffs <= 0) return false;
  if (firechaffttl > 0) return false;
  for (i = 0; i < maxchaff; i ++)
  {
    if (chaff [i]->ttl <= 0) break;
  }
  if (i < maxchaff)
  {
    fireChaff2 (chaff [i]);
    chaffs --;
    firechaffttl = 8 * timestep;
    for (i2 = 0; i2 < maxmissile; i2 ++)
    {
      if (missile [i2]->ttl > 0)
      {
        if (missile [i2]->id > MISSILE_AIR3) // only radar seeking missiles
          if (missile [i2]->target == this) // only change target if angle is good
          {
            bool hit = false;
            if (easymodel == 1)
            {
              if (myrandom ((int) (theta + 20)) > 50) hit = true;
            }
            else if (easymodel == 2)
            {
              if (myrandom ((int) (fabs (elevatoreffect) * 90 + 20)) > 50) hit = true;
            }
            if (hit)
            {
              if (debuglevel == LOG_ALL)
              {
                display ("Missile to chaff", LOG_ALL);
              }
              missile [i2]->target = chaff [i];
            }
          }
      }
    }
    return true;
  }
  return false;
}

bool AIObj::fireMissileAir (AIObj **missile, AIObj *target)
{
  if (ttf > 0) return false;
  if (target->id >= MOVING_GROUND) return false;
  if (haveMissile (MISSILE_AIR3))
    return fireMissile (MISSILE_AIR3, missile, (AIObj *) target);
  else if (haveMissile (MISSILE_AIR2))
    return fireMissile (MISSILE_AIR2, missile, (AIObj *) target);
  else if (haveMissile (MISSILE_AIR1))
    return fireMissile (MISSILE_AIR1, missile, (AIObj *) target);
  return false;
}

bool AIObj::selectMissileAir (AIObj **missile)
{
  bool sel = false;
  if (haveMissile (MISSILE_AIR3)) { missiletype = MISSILE_AIR3 - MISSILE1; sel = true; }
  else if (haveMissile (MISSILE_AIR2)) { missiletype = MISSILE_AIR2 - MISSILE1; sel = true; }
  else if (haveMissile (MISSILE_AIR1)) { missiletype = MISSILE_AIR1 - MISSILE1; sel = true; }
  return sel;
}

bool AIObj::fireMissileAirFF (AIObj **missile, AIObj *target)
{
  if (ttf > 0) return false;
  if (target->id >= MOVING_GROUND) return false;
  if (haveMissile (MISSILE_FF2))
    return fireMissile (MISSILE_FF2, missile, (AIObj *) target);
  else if (haveMissile (MISSILE_FF1))
    return fireMissile (MISSILE_FF1, missile, (AIObj *) target);
  return false;
}

bool AIObj::selectMissileAirFF (AIObj **missile)
{
  bool sel = false;
  if (haveMissile (MISSILE_FF2)) { missiletype = MISSILE_FF2 - MISSILE1; sel = true; }
  else if (haveMissile (MISSILE_FF1)) { missiletype = MISSILE_FF1 - MISSILE1; sel = true; }
  return sel;
}

bool AIObj::fireMissileGround (AIObj **missile)
{
  if (ttf > 0) return false;
  if (target->id < MOVING_GROUND) return false;
  if (haveMissile (MISSILE_GROUND2))
    return fireMissile (MISSILE_GROUND2, missile, (AIObj *) target);
  else if (haveMissile (MISSILE_GROUND1))
    return fireMissile (MISSILE_GROUND1, missile, (AIObj *) target);
  return false;
}

bool AIObj::selectMissileGround (AIObj **missile)
{
  bool sel = false;
  if (haveMissile (MISSILE_GROUND2)) { missiletype = MISSILE_GROUND2 - MISSILE1; sel = true; }
  else if (haveMissile (MISSILE_GROUND1)) { missiletype = MISSILE_GROUND1 - MISSILE1; sel = true; }
  return sel;
}

void AIObj::targetNearestGroundEnemy (AIObj **f)
{
  int i;
  float d = 1E12; //10000 is too low
  ttf = 50 * timestep;
  for (i = 0; i < maxfighter; i ++)
  {
    if (this != f [i] && party != f [i]->party && f [i]->active)
    {
//      float d2 = distance (f [i]);
      float phi = getAngle (f [i]);
      float d2 = distance (f [i]) * (60 + fabs (phi)); // prefer enemies in front
      if (bomber)
        if (f [i]->id < MOVING_GROUND)
          d2 += 1E10; // only use this target if no ground targets exist
      if (d2 < d)
      {
        d = d2;
        target = f [i];
      }
    }
  }
  if (distance (target) > 200)
  { target = NULL; }
}

void AIObj::targetNearestEnemy (AIObj **f)
{
  int i;
  float d = 1E12; //10000 is too low
  ttf = 50 * timestep;
  for (i = 0; i < maxfighter; i ++)
  {
    if (this != f [i] && party != f [i]->party && f [i]->active)
    {
//      float d2 = distance (f [i]);
      float phi = getAngle (f [i]);
      float d2 = distance (f [i]) * (60 + fabs (phi)); // prefer enemies in front
      if (d2 < d)
      {
        d = d2;
        target = f [i];
      }
    }
  }
  if (!ai && target)
    if (distance (target) > 200)
      target = NULL;
}

void AIObj::targetLockingEnemy (AIObj **f)
{
  int i;
//    float d = 10000;
  ttf = 50 * timestep;
  if (target == NULL) target = f [0];
  for (i = 0; i < maxfighter; i ++)
    if (target == f [i])
      break;
  int z = 0;
  do
  {
    i ++;
    if (i >= maxfighter) { i = 0; z ++; }
  } while ((!f [i]->active || f [i]->party == party || f [i]->target != this || distance (f [i]) > 200) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

void AIObj::targetNext (AIObj **f)
{
  int i;
//    float d = 10000;
  ttf = 50 * timestep;
  if (target == NULL) target = f [0];
  for (i = 0; i < maxfighter; i ++)
    if (target == f [i])
      break;
  int z = 0;
  do
  {
    i ++;
    if (i >= maxfighter) i = 0;
    if (f [i] == this)
    { i ++; z ++; }
    if (i >= maxfighter) i = 0;
  } while ((!f [i]->active || distance (f [i]) > 200) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

void AIObj::targetPrevious (AIObj **f)
{
/*  int i, lastfighter = 0;
//    float d = 10000;
  if (target == NULL) target = f [0];
  for (i = maxfighter - 1; i >= 0; i --)
    if (f [i]->active)
    {
      lastfighter = i;
      break;
    }
  for (i = 0; i < maxfighter; i ++)
    if (target == f [i])
      break;
  i --;
  if (i >= 0) i = lastfighter;
  if (target == this) i --;
  if (i >= 0) i = lastfighter;
  target = f [i];*/
  int i;
//    float d = 10000;
  ttf = 50 * timestep;
  if (target == NULL) target = f [0];
  for (i = 0; i < maxfighter; i ++)
    if (target == f [i])
      break;
  int z = 0;
  do
  {
    i --;
    if (i < 0) i = maxfighter - 1;
    if (f [i] == this)
    { i --; z ++; }
    if (i < 0) i = maxfighter - 1;
  } while ((!f [i]->active || distance (f [i]) > 200) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

/*float AIObj::getFunction (float value)
{
  if (value <= fstart) return theta;
  if (value >= fend) return rectheta;
  value = (value - fstart) / (fend - fstart);
  return fa*value*value*value+fb*value*value+fc*value+fd;
}*/

// core AI method
void AIObj::aiAction (Uint32 dt, AIObj **f, AIObj **m, DynamicObj **c, DynamicObj **flare, DynamicObj **chaff)
{
  int i;

  timer += dt;

  if (!active && !draw) // not active, not drawn, then exit
  {
    return;
  }

  if (firecannonttl > 0) firecannonttl -= dt; // time to fire the next missile
  if (firemissilettl > 0) firemissilettl -= dt; // time to fire the next missile
  if (fireflarettl > 0) fireflarettl -= dt; // time to fire the next flare
  if (firechaffttl > 0) firechaffttl -= dt; // time to fire the next chaff
  if (smokettl > 0) smokettl -= dt; // time to fire the next chaff

  // move object according to our physics
  move (dt);

  float timefac = (float) dt / (float) timestep;

  if (id >= STATIC_PASSIVE) // no AI for static ground objects (buildings)
    return;

  // set smoke
  if ((id >= MISSILE1 && id < MISSILE_MINE1) || (id >= FIGHTER1 && id <= FIGHTER2)) // missile or fighter
  {
    float sz = COS(gamma) * COS(phi) * zoom * 1.1; // polar (spherical) coordinates
    float sy = -SIN(gamma) * zoom * 1.1;
    float sx = COS(gamma) * SIN(phi) * zoom * 1.1;
    // four smoke elements per discrete movement
//    if (smokettl <= 0)
    {
      smoke->setSmoke (tl->x - sx - forcex * 0.75, tl->y - sy - forcey * 0.75, tl->z - sz - forcez * 0.75, (int) phi, 26);
      smoke->setSmoke (tl->x - sx - forcex * 0.5, tl->y - sy - forcey * 0.5, tl->z - sz - forcez * 0.5, (int) phi, 27);
      smoke->setSmoke (tl->x - sx - forcex * 0.25, tl->y - sy - forcey * 0.25, tl->z - sz - forcez * 0.25, (int) phi, 28);
      smoke->setSmoke (tl->x - sx, tl->y - sy, tl->z - sz, (int) phi, 29);
      smoke->move (dt, 4);
    }
/*    else if (smokettl > 0 && smokettl <= timestep / 2)
    {
      smoke->setSmoke (tl->x - sx - forcex * 0.75, tl->y - sy - forcey * 0.75, tl->z - sz - forcez * 0.75, (int) phi, 28);
      smoke->setSmoke (tl->x - sx - forcex * 0.25, tl->y - sy - forcey * 0.25, tl->z - sz - forcez * 0.25, (int) phi, 29);
      smoke->move (dt, 2);
    }
    else
    {
      smoke->setSmoke (tl->x - sx - forcex * 0.5, tl->y - sy - forcey * 0.5, tl->z - sz - forcez * 0.5, (int) phi, 29);
      smoke->move (dt, 1);
    }*/
    smokettl += timestep;
  }

  if (!active) // not active, then exit
  {
    return;
  }
  if (explode > 0 || sink > 0) // exploding or sinking, then exit
  {
    return;
  }
  
  // do expensive calculations only once
  float myheight = l->getExactHeight (tl->x, tl->z);
  float targetheight = tl->y;
  if (target != NULL)
    targetheight = l->getExactHeight (target->tl->x, target->tl->z);
  if (target != NULL)
    disttarget = distance (target); // distance to target
  else
    disttarget = 1;

  // get a new target if necessary
  if (target == NULL)
  {
    if (bomber)
      targetNearestGroundEnemy (f);
    else
      targetNearestEnemy (f);
  }
  if (target != NULL)
    if (!target->active)
    {
      if (bomber)
        targetNearestGroundEnemy (f);
      else
        targetNearestEnemy (f);
    }

  if (id >= FIGHTER1 && id <= FIGHTER2) // for fighters do the following
  {
    if (haveMissile () && target != NULL)
    {
      float dgamma = atan ((target->tl->y - tl->y) / disttarget) * 180 / PI - (gamma - 180);
      float dphi = getAngle (target);
      if (missiletype == MISSILE_DF1 - MISSILE1)
      {
        ttf = 0;
      }
      else if (fabs (dphi) < 50 && fabs (dgamma) < 50 && party != target->party)
      {
        if (disttarget < 50)
        {
          if (ttf > 0)
          {
            if (missiletype >= 0 && missiletype <= 2)
            {
              if (target->id >= FIGHTER1 && target->id <= FIGHTER2)
              {
                float dphi = fabs (phi - target->phi);
                if (dphi > 270) dphi = 360 - dphi;
                if (dphi < 45)
                  ttf -= 3 * dt;
                else
                  ttf = 50 * timestep;
              }
            }
            else if (missiletype == 6 || missiletype == 7)
            {
              if (target->id >= FIGHTER1 && target->id <= FIGHTER2)
              {
                ttf -= 3 * dt;
              }
            }
            else
            {
              if (target->id > FIGHTER2)
              {
                ttf -= 3 * dt;
              }
            }
          }
        }
      }
      else
      {
        ttf = 50 * timestep;
      }
    }
  }

  if (!ai) return;

  // which height???
  float recheight2; // this is the height, the object wants to achieve
  int lsdist = 10;
//  if (id == 200) printf ("%1.2f, %1.2F  ", lsdist * forcex, lsdist * forcez);
//  if (fabs (theta) < 20) lsdist = 10;
  float flyx = tl->x + forcex * lsdist, flyz = tl->z + forcez * lsdist;
  int flyxs = l->getCoord ((int) flyx), flyzs = l->getCoord ((int) flyz);
  {
    if (manoeverheight > 0)
    {
      // precalculated height
      recheight2 = l->getExactHeight (flyx, flyz) + recheight;
    }
    else
    {
       // missiles and non intelligent objects will not change their height due to the surface
      if ((id >= MISSILE1 && id <= MISSILE2 && target != NULL) ||
          (tl->y - myheight > 6 && target != NULL && tl->y - myheight < 50/* && !manoeverheight*/))
      {
        recheight2 = target->tl->y - 8 * target->thrust * SIN(target->gamma);
      }
      else
      {
        // precalculated height
        float flyx2 = tl->x + forcex * lsdist * 3, flyz2 = tl->z + forcez * lsdist * 3;
        float flyx3 = tl->x + forcex * lsdist * 8, flyz3 = tl->z + forcez * lsdist * 8;
        float h1 = l->getHeight (flyx, flyz);
        float h2 = l->getHeight (flyx2, flyz2);
        float h3 = l->getHeight (flyx3, flyz3);
        h1 = h1 > h2 ? h1 : h2;
        h1 = h1 > h3 ? h1 : h3;
        recheight2 = recheight + h1;
      }
    }
  }
  if (ttl <= 0 && id >= MISSILE1 && id <= MISSILE2)
    recheight2 = -100;

  // calculate the recommended height, recheight2 depends on it
  if (manoeverheight > 0) manoeverheight -= dt;
  if (manoeverheight <= 0)
  {
    if (!(id >= FIGHTER1 && id <= FIGHTER2) && target != NULL) // no fighter, has target (missile, mine)
    {
      recheight = target->tl->y - targetheight;
    }
    else if (id >= FIGHTER1 && id <= FIGHTER2 && target != NULL) // fighter, has target
    {
      if (target->id >= FIGHTER1 && target->id <= FIGHTER2)
        recheight = target->tl->y - targetheight;  // target is a fighter
      else
        recheight = target->tl->y - targetheight + 5; // target is no fighter
      if (!l->isWater (l->f [flyxs] [flyzs])) // not flying above water
      {
        if (recheight < 3.5 + 0.005 * aggressivity) recheight = 3.5 + 0.005 * aggressivity;
        if (maxgamma < 40) // transporters have to stay higher
          if (recheight < 20) recheight = 20;
      }
      float minh = 4.5 + 0.005 * aggressivity; // minimum height
      if (l->type == LAND_CANYON) minh = 5.5 + 0.005 * aggressivity; // stay higher in canyons
      if (fabs (tl->y - myheight) < minh)
      {
        recheight = 8 + 0.01 * aggressivity;
        manoeverheight = 12 * timestep; // fly manoever to gain height
//        if (id == 200) printf ("M(%f)", l->getHeight (flyx, flyz));
      }
      if (disttarget < 50 && fabs (tl->y - myheight) > 20)
      {
        recheight = 5 + 0.02 * aggressivity;
        manoeverheight = 15 * timestep;
      }
    }
  }

  if (id == FIGHTER_TRANSPORT && recheight < 10) recheight = 25;

  if (ai)
  {
    if (target != NULL && ((id >= MISSILE1 && id <= MISSILE2) || (id >= FIGHTER1 && id <= FIGHTER2 && manoeverheight <= 0))) // is AGM
    {
      float dgamma = 0;
      if (disttarget <= -0.00001 || disttarget >= 0.00001) // no division by zero
        dgamma = atan ((target->tl->y - tl->y) / disttarget) * 180 / PI - (gamma - 180);
      recgamma = gamma + dgamma; // get recommended elevation to target
//      if (id == 200) printf ("f");
    }
    else
    {
      recgamma = (int) ((recheight2 - tl->y) * 10 - gamma + 360);
//      if (id == 200) printf ("m");
    }
  }

//  recgamma = 180 + atan ((recheight2 - tl->y) / thrust) * 180 / pitab;
//  if (recgamma < -80) recgamma = -80;
//  if (recgamma > 80) recgamma = 80;
/*  if (recgamma > gamma + 4) gamma += (recgamma - gamma) / 8 + 1;
  else if (recgamma < gamma - 4) gamma += (recgamma - gamma) / 8 - 1;
  if (gamma > 250) gamma = 250;
  if (gamma < 110) gamma = 110;*/

  // do a smooth roll
  float deltatheta;
/*  if (rectheta != lastrectheta)
  {
    deltatheta = (getFunction (timer) - theta); // / (value - lasttimervalue);
    fa=deltatheta+2*theta-2*rectheta;
    fb=-3*theta-2*deltatheta+3*rectheta;
    fc=deltatheta;
    fd=theta;
    fstart = timer;
    fend = timer + deltatheta * 5;
  }
  theta = getFunction (timer);
  lastrectheta = rectheta;*/

  if (easymodel == 1)
  {
    deltatheta = rectheta - theta;
    if (fabs (dtheta) > 30)
    { dtheta = 0; }
    float mynimbility = fabs (deltatheta) / 5.0F * nimbility;
    if (mynimbility > nimbility) mynimbility = nimbility;
    float nimbility2 = mynimbility;
    if (nimbility2 >= -0.00001 && nimbility2 <= 0.00001)
      nimbility2 = 0.00001;

    if (deltatheta > 0 && dtheta < 0) dtheta += mynimbility * timefac;
    else if (deltatheta < 0 && dtheta > 0) dtheta -= mynimbility * timefac;
    else if (deltatheta > 0)
    {
      float estimatedtheta = dtheta * (dtheta + nimbility2 * 5 / timefac) / 2 / nimbility2;
      if (deltatheta > estimatedtheta/* + timefac*/) dtheta += mynimbility * timefac;
      else if (deltatheta < estimatedtheta/* - timefac*/) dtheta -= mynimbility * timefac;
    }
    else
    {
      float estimatedtheta = -dtheta * (dtheta - nimbility2 * 5 / timefac) / 2 / nimbility2;
      if (deltatheta < estimatedtheta/* - timefac*/) dtheta -= mynimbility * timefac;
      else if (deltatheta > estimatedtheta/* + timefac*/) dtheta += mynimbility * timefac;
    }
    theta += dtheta;

    // height changes
    if (easymodel == 1)
    {
      float nimbility1 = nimbility / 5;
      if (nimbility1 >= -0.00001 && nimbility1 <= 0.00001)
        nimbility1 = 0.00001;
      if (theta > maxtheta) theta = maxtheta; // restrict roll angle
      else if (theta < -maxtheta) theta = -maxtheta;

      float deltagamma = recgamma - gamma;
      if (deltagamma > 0 && dgamma < 0) dgamma += nimbility1 * timefac;
      else if (deltagamma < 0 && dgamma > 0) dgamma -= nimbility1 * timefac;
      else if (deltagamma > 0)
      {
        float estimatedgamma = dgamma * (dgamma + nimbility1 * 2) / nimbility1;
        if (id == 200)
          id = id;
        if (deltagamma > estimatedgamma + 2) dgamma += nimbility1 * timefac;
        else if (deltagamma < estimatedgamma - 2) dgamma -= nimbility1 * timefac;
      }
      else if (deltagamma < 0)
      {
        float estimatedgamma = -dgamma * (dgamma + nimbility1 * 2) / nimbility1;
        if (id == 200)
          id = id;
        if (deltagamma < estimatedgamma - 2) dgamma -= nimbility1 * timefac;
        else if (deltagamma > estimatedgamma + 2) dgamma += nimbility1 * timefac;
      }
      gamma += dgamma;
    }
  }

  /*    if (gamma > 180 + maxgamma) gamma = 180 + maxgamma;
    else if (gamma < 180 - maxgamma) gamma = 180 - maxgamma;*/

  if (id >= MISSILE1 && id <= MISSILE2)
  {
    if (target == NULL)
    {
      ttl = 0;
      return;
    }
    else if (target->active == false)
    {
      ttl = 0;
      return;
    }
  }

  if (target == NULL) return;

  // fighter's targeting mechanism for missiles
  if (id >= FIGHTER1 && id <= FIGHTER2) // for fighters do the following
  {
    if (ai)
    {
      if (target->id >= FIGHTER1 && target->id <= FIGHTER2)
      {
        if (!selectMissileAirFF (m))
          selectMissileAir (m);
      }
      else
      {
        selectMissileGround (m);
      }
    }
  }

  if (!ai || target == NULL) // no AI (player) or no target found, then exit
  {
    return;
  }

  // fire flares
  if (id >= FIGHTER1 && id <= FIGHTER2) // for fighters do the following
  {
//    if (!(l->lsticker & 7))
    if (!manoevertheta)
      for (i = 0; i < maxmissile; i ++)
        if (m [i]->ttl > 0)
          if (m [i]->target == this)
          {
            if (m [i]->id >= 0 && m [i]->id <= MISSILE_AIR3)
            {
              if (fabs (theta) >= 30)
              {
                fireFlare (flare, m);
                fireflarettl += intelligence / 20 * timestep;
              }
              manoevertheta = 35 * timestep;
            }
            else
            {
              if (fabs (theta) >= 30)
              {
                fireChaff (chaff, m);
                firechaffttl += intelligence / 20 * timestep;
              }
              manoevertheta = 35 * timestep;
            }
          }
  }

  int firerate;
  if (difficulty == 0) firerate = 12;
  else if (difficulty == 1) firerate = 6;
  else firerate = 3;

  float dx2, dz2, ex, ez;
  float dx = target->tl->x - tl->x, dz = target->tl->z - tl->z; // current distances
  if ((id >= FIGHTER1 && id <= FIGHTER2) || (id >= MISSILE1 && id <= MISSILE2) || (id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
  {
    float t = 10.0 * disttarget; // generous time to new position
    if (t > 60) t = 60; // higher values will not make sense
    t *= (float) (400 - precision) / 400;
    int tt = (int) target->theta;
    if (tt < 0) tt += 360;
    float newphi = t * SIN(tt) * 5.0 * target->manoeverability; // new angle of target after time t
    if (newphi > 90) newphi = 90;
    else if (newphi < -90) newphi = -90;
    newphi += (float) target->phi;
    if (newphi >= 360) newphi -= 360;
    if (newphi < 0) newphi += 360;
    if ((id >= FIGHTER1 && id <= FIGHTER2) || (id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
    {
      ex = target->tl->x - SIN(newphi) * t * target->realspeed * 0.25; // estimated target position x
      ez = target->tl->z - COS(newphi) * t * target->realspeed * 0.25; // estimated target position z
    }
    else
    {
      ex = target->tl->x - SIN(newphi) * t * target->realspeed * 0.05; // estimated target position x
      ez = target->tl->z - COS(newphi) * t * target->realspeed * 0.05; // estimated target position z
    }
    dx2 = ex - tl->x; dz2 = ez - tl->z; // estimated distances
  }
  else
  {
    dx2 = dx; dz2 = dz;
  }
  float a, w = phi;
  if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;

/*  if (intelligence == 0)
{
m [0]->tl->x = ex; //tl->x + dx2; // visualize estimated point by a missile
m [0]->tl->z = ez; //tl->z + dz2;
m [0]->active = false;
m [0]->draw = true;
m [0]->tl->y = target->tl->y;
}*/

  // get heading to target
  a = atan (dx2 / dz2) * 180 / PI;
  if (dz2 > 0)
  {
    if (dx2 > 0) a -= 180.0F;
    else a += 180.0F;
  }
//    this->aw = a;
  aw = a - w; // aw=0: target in front, aw=+/-180: target at back
  if (aw < -180) aw += 360;
  if (aw > 180) aw -= 360;

  if (manoevertheta > 0) manoevertheta -= dt;
  if (manoeverthrust > 0) manoeverthrust -= dt;

  // heading calculations  
  if (id >= FIGHTER1 && id <= FIGHTER2) // for fighters do the following
  {
    if (!acttype && disttarget <= 1000 && manoevertheta <= 0) // no special action, near distance, no roll manoever
    {
      if (aw > 0) // positive angle
      {
        if (aw > 180 - intelligence / 10 && disttarget < 15) // target is at the back
        {
          rectheta = -90;
          manoevertheta = timestep * (10 + myrandom ((400 - intelligence) / 4)); // turn hard left or right
          if (manoeverthrust <= 0)
            recthrust = maxthrust - myrandom (intelligence) * 0.0003; // fly faster
          if (manoeverheight <= 0)
          { recheight = 5; manoeverheight = timestep * (20 - intelligence / 50); } // stay low
        }
        else if (aw < 40 && disttarget > 60)
        {
          rectheta = 0;
        }
        else // otherwise fly to target direction
        {
          int maw = aw > 90 ? 90 : (int) aw;
          maw = 90 - maw;
          rectheta = 90 - maw * intelligence / 400;
        }
      }
      else // same for negative angle
      {
        if (aw < -180 + intelligence / 10 && disttarget < 15)
        {
          rectheta = 90;
          manoevertheta = timestep * (10 + myrandom ((400 - intelligence) / 4));
          if (manoeverthrust <= 0)
            recthrust = maxthrust - myrandom (intelligence) * 0.0003;
          if (manoeverheight <= 0)
          { recheight = 5; manoeverheight = timestep * (20 - intelligence / 50); }
        }
        else if (aw > -40 && disttarget > 60)
        {
          rectheta = 0;
        }
        else
        {
          int maw = aw < -90 ? -90 : (int) aw;
          maw = -90 - maw;
          rectheta = -90 - maw * intelligence / 400;
        }
  //        rectheta = -90;
      }
  //      if (rectheta > theta + 5.0) toLeft ();
  //      else if (rectheta < theta - 5.0) toRight ();
    }
  }
  else if (id >= MISSILE1 && id <= MISSILE2) // for missiles do the following
  {
    if (fabs (aw) < 50 && disttarget > 10) // target in front and minimum distance, then no roll
      rectheta = 0;
    else // otherwise chase target
    {
      if (aw > 0)
      {
        rectheta = aw > 90 ? 90 : aw;
      }
      else
      {
        rectheta = aw < -90 ? -90 : aw;
      }
    }
  }
  else if (id >= FLAK1 && id <= FLAK2) // ground-air-cannon
  {
    recthrust = 0; thrust = 0;
    if (aw > 5)
    {
      rectheta = maxtheta;
    }
    else if (aw < -5)
    {
      rectheta = -maxtheta;
    }
    else
    {
      rectheta = 0;
    }
  }
  else if (id >= TANK1 && id <= TANK2) // tanks
  {
    recthrust = maxthrust; thrust = maxthrust; // always at maximum thrust
    if (aw > 5)
    {
      rectheta = maxtheta;
    }
    else if (aw < -5)
    {
      rectheta = -maxtheta;
    }
    else
    {
      rectheta = 0;
    }
    if (firecannonttl <= 0)
    {
      if (id == TANK_AIR1)
        if (fabs (rectheta - theta) < 2 && fabs (aw) < 20 && disttarget < 40 && target->tl->y > tl->y + 2)
        {
          fireCannon (c);
          firecannonttl += firerate * timestep;
        }
      if (id == TANK_GROUND1)
        if (fabs (rectheta - theta) < 2 && fabs (aw) < 20 && disttarget < 35 && target->tl->y <= tl->y + 1 && target->tl->y >= tl->y - 1)
        {
          fireCannon (c);
          firecannonttl += firerate * timestep;
        }
    }
  }

  // thrust and manoever calculations
  if (id >= FIGHTER1 && id <= FIGHTER2) // fighters
  {
    if (disttarget > 5 + aggressivity / 12) // 2.5 seems to be best, but fighters become far too strong
    {
      if (disttarget < 50 && fabs (aw) > 30 && manoeverthrust <= 0) // low thrust for faster heading changes in melee combat
        recthrust = maxthrust / (2 - intelligence * 0.001);
      else thrustUp (); // otherwise fly faster
    }
    else if (manoeverthrust <= 0)
    {
      if (recthrust > target->thrust) // adopt thrust of target
      {
        thrustDown ();
      }
      else
      {
        thrustUp ();
      }
    }
    if (disttarget > 50 && fabs (aw) < 20) // high distance and target in front, then fly straight
      rectheta = 0;
/*    if (disttarget < 5 && abs (aw) > 50 && (target->id < FIGHTER1 || target->id > FIGHTER2)) // avoid collisions
    {
      manoevertheta = 40 * timestep;
      rectheta = 0;
      manoeverthrust = 40 * timestep;
      recthrust = maxthrust;
    }*/
    if (manoeverthrust <= 0)
      if (disttarget < 25 && fabs (aw) > 160 && target->id >= TANK1) // avoid collisions
      {
        manoeverthrust = 25 * timestep;
        recthrust = maxthrust;
        manoevertheta = 25 * timestep;
        rectheta = 0;
        manoeverheight = 25 * timestep;
        recheight = 10;
      }
    // fire cannon?
    float agr = 4.0 - (float) aggressivity / 100;
    if (firecannonttl <= 0)
    {
      if (fabs (rectheta - theta) < agr && fabs (aw) < 20 + agr * 4 && disttarget < 30)
        fireCannon (c);
      else if (disttarget < 2 + agr && fabs (aw) < 20 + agr * 4)
        fireCannon (c);
    }
    // fire missile?
    if (firemissilettl <= 0)
    {
      if (target->id >= FIGHTER1 && target->id <= FIGHTER2)
      {
        if (fabs (rectheta - theta) < agr / 2 && fabs (aw) < agr && disttarget < 45)
//          if (!(l->lsticker & 7))
          {
            fireMissile (m, (AIObj *) target);
            firemissilettl += aggressivity / 2 * timestep;
          }
      }
      else // ground target
      {
        if (fabs (rectheta - theta) < 5 + agr * 4 && fabs (aw) < 5 + agr * 4 && disttarget < 50)
          if (!(l->lsticker & 7))
          {
            fireMissileGround (m);
            firemissilettl += aggressivity / 2 * timestep;
          }
      }
    }
/*    if (manoevertheta <= 0) // change roll angle
    {
      rectheta += myrandom (precision) - precision / 2;
      if (rectheta > 90 - precision / 5) rectheta = 90 - precision / 5;
      else if (rectheta < -90 + precision / 5) rectheta = -90 + precision / 5;
    }*/
//      if (recthrust > maxthrust - 0.001 * intelligence)
//        recthrust = maxthrust - 0.001 * intelligence;
/*    idle += dt;
    if (idle > 400 * timestep) // too long flying the same direction, then change direction
    {
      idle = 0;
      if (rectheta < 0)
      { rectheta = 90; manoevertheta = timestep * (50 + myrandom (100)); }
      if (rectheta > 0)
      { rectheta = -90; manoevertheta = timestep * (50 + myrandom (100)); }
    }*/
  }

  if ((id >= FLAK1 && id <= FLAK2) || id == SHIP_CRUISER || id == SHIP_DESTROYER1)
  {
//    if (!(l->lsticker & 4))
    if (firecannonttl <= 0)
    for (int i = 0; i < maxfighter; i ++)
    if (f [i]->active)
    if (party != f [i]->party)
    {
      disttarget = distance (f [i]); // distance to target
      ex = f [i]->tl->x; // estimated target position x
      ez = f [i]->tl->z; // estimated target position z
      dx2 = ex - tl->x; dz2 = ez - tl->z; // estimated distances
      w = (int) phi;
      if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;
      a = (atan (dx2 / dz2) * 180 / PI);
      if (dz2 > 0)
      {
        if (dx2 > 0) a -= 180;
        else a += 180;
      }
      aw = a - w;
      if (aw < -180) aw += 360;
      if (aw > 180) aw -= 360;
      if (id == FLAK_AIR1)
        if (f [i]->tl->y > tl->y + 2)
        {
          if (fabs (aw) <= 20 && disttarget < 50) // + aggressive
            fireCannon (c, phi + aw);
          firecannonttl = firerate * timestep;
        }
      if (id == SHIP_DESTROYER1)
        if (f [i]->tl->y > tl->y + 2)
//              if (myrandom (intelligence) < 120)
//            if (!(l->lsticker & firerate))
        {
          if (aw >= 0 && aw < 40 && disttarget < 50) // + aggressive
            fireCannon (c, phi + aw);
          if (aw >= -40 && aw < 0 && disttarget < 50) // + aggressive
            fireCannon (c, phi + aw);
          if (aw >= 120 && aw < 160 && disttarget < 50) // + aggressive
            fireCannon (c, phi + aw);
          if (aw >= -160 && aw < -120 && disttarget < 50) // + aggressive
            fireCannon (c, phi + aw);
          firecannonttl = firerate * timestep;
        }
      if (firemissilettl <= 0)
        if (id == FLARAK_AIR1)
//          if (!(l->lsticker & 31))
            if (fabs (aw) < 25 && disttarget < 45) // + aggressive
              if (f [i]->tl->y > tl->y + 2)
//                if (myrandom (intelligence) < 80)
                {
                  ttf = 0;
                  fireMissileAirFF (m, f [i]);
                  firemissilettl += (20 + firerate * 10) * timestep;
                }
      if (id == SHIP_CRUISER)
      {
//        if (!(l->lsticker & 15))
        if (firemissilettl <= 0)
          if (aw >= -30 && aw < 30 && disttarget < 60) // + aggressive
//            if (myrandom (intelligence) < 100)
            {
              ttf = 0;
              fireMissileAirFF (m, f [i]);
              firemissilettl += aggressivity / 5 * timestep;
              missiles [6] ++; // unlimited ammo
            }
//          if (myrandom (intelligence) < 120)
//        if (!(l->lsticker & firerate))
        if (firecannonttl <= 0)
        {
/*          if (aw >= 0 && aw < 40 && disttarget < 45) // + aggressive
            fireCannon (c, phi + aw);
          if (aw >= -40 && aw < 0 && disttarget < 45) // + aggressive
            fireCannon (c, phi + aw);
          if (aw >= 120 && aw < 160 && disttarget < 45) // + aggressive
            fireCannon (c, phi + aw);
          if (aw >= -160 && aw < -120 && disttarget < 45) // + aggressive
            fireCannon (c, phi + aw);*/
          fireCannon (c, phi + aw);
        }
      }
    }
  }
  if (id >= FIGHTER1 && id <= FIGHTER2)
  {
//    rectheta += (int) (timefac * (float) (myrandom (intelligence) - intelligence / 2));
    if (rectheta > 90 - precision / 5) rectheta = 90 - precision / 5;
    else if (rectheta < -90 + precision / 5) rectheta = -90 + precision / 5;
  }
}

#endif
