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
#include "gl.h"

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
  maxspeed = 0.3; braking = 0.5; manoeverability = 0.5;
  speed = maxspeed; recspeed = speed; recheight = 5.0;// height = 5.0;
  ttl = -1;
  shield = 1; maxshield = 1;
  immunity = 0;
//    controls = false;
  recgamma = 180;
  id = CANNON1;
  impact = 10;
  source = NULL;
  points = 0;
  party = 0;
  easymodel = true;
  aileroneffect = 0;
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

void DynamicObj::speedUp ()
{
  recspeed += maxspeed / 12;
  if (recspeed > maxspeed) recspeed = maxspeed;
}

void DynamicObj::speedDown ()
{
  recspeed -= maxspeed / 12;
  if (recspeed < maxspeed / 2) recspeed = maxspeed / 2;
}

float DynamicObj::distance (DynamicObj *target)
{
  float dx = target->tl->x - tl->x;
  float dz = target->tl->z - tl->z;
  return sqrt (dx * dx + dz * dz);
}

// check whether the object is exploding or sinking and deactivate if necessary
void DynamicObj::checkExplosion ()
{
  if (explode > 0)
  {
    explode ++;
    if (explode == 3)
    {
      if (id == STATIC_CONTAINER1 || id == STATIC_RADAR1 || id == STATIC_COMPLEX1)
      {
        setExplosion (1.5, 40);
        setBlackSmoke (3.0, 80);
      }
      else if (id == STATIC_OILRIG1)
      {
        setExplosion (3.0, 40);
        setBlackSmoke (5.5, 80);
      }
      else if (id == STATIC_TENT1)
      {
      }
      else if (id == TANK1)
      {
        explode = explode;
      }
      else
      {
        setExplosion (0.6, 40);
        setBlackSmoke (1.0, 60);
      }
    }
    if (explode >= 35)
    {
      deactivate (); ttl = -1;
    }
  }
  if (sink)
  {
    sink ++;
    if (sink > 100)
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
    if (!explode)
    {
      if (id >= MISSILE1 && id <= MISSILE2)
      { explode = 1; active = false; }
      if (id >= FIGHTER1 && id <= FIGHTER2)
      { explode = 1; active = false; }
      if (id >= TANK1 && id <= TANK2)
      { explode = 1; active = false; }
      if (id >= FLAK1 && id <= FLAK2)
      { explode = 1; active = false; }
      if (id >= STATIC)
      { explode = 1; active = false; }
    }
    if (!sink)
      if (id >= SHIP1 && id <= SHIP2)
      { sink = 1; }
  }
//    printf ("\nshield=%d", shield); fflush (stdout);
}

// check whether the object collides on the ground and alter gamma and y-translation
void DynamicObj::crashGround ()
{
  if ((id >= SHIP1 && id <= SHIP2) || (id >= FLAK1 && id <= FLAK2) || id >= STATIC || (id >= TANK1 && id <= TANK2))
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
        setExplosion (1.2, 30);
        setBlackSmoke (1.2, 30);
      }
      if (id >= FIGHTER1 && id <= FIGHTER2)
      {
        setExplosion (0.2, 25);
        setBlackSmoke (0.5, 25);
      }
    }
    if (id >= CANNON1 && id <= CANNON2)
      deactivate ();
    shield -= 1;
//      shield += (int) height - 2;
  }
  // restrict to a maximum height, we want an action game!!!
  if (height > 50) tl->y = l->getHeight (tl->x, tl->z) + 50;
}

// check for collision, simplified model, each model is surrounded by a cube
// this works pretty well, but we must use more than one model for complex models or scenes
void DynamicObj::collide (DynamicObj *d) // d must be the medium (laser, missile)
{
  if (immunity > 0 || d->immunity > 0) return;
  if (explode > 0 || sink > 0) return;
  float z = d->zoom > zoom ? d->zoom : zoom;
  if ((id >= MISSILE1 && id <= MISSILE2) || (d->id >= MISSILE1 && d->id <= MISSILE2))
  {
    z *= 2; // missiles need not really hit the fighter, but will explode near it
  }
  if ((id >= SHIP1 && id <= SHIP2) || (d->id >= SHIP1 && d->id <= SHIP2))
    z *= 0.3;
  if (id == ASTEROID)
    z *= 1.2;
/*    else if ((id >= CANNON1 && id <= CANNON2) || (d->id >= CANNON1 && d->id <= CANNON2))
    z *= 1.0;
  else if (id >= FLAK1 && id <= FLAK2)
    z *= 1.0;*/
  if (tl->x >= d->tl->x - z && tl->x <= d->tl->x + z &&
      tl->y >= d->tl->y - z && tl->y <= d->tl->y + z &&
      tl->z >= d->tl->z - z && tl->z <= d->tl->z + z)
  {
    shield -= d->impact;
    d->shield -= impact;
//      printf (" c(%d,%d)=>s(%d,%d)\n", id, d->id, shield, d->shield);
    if (d->source != NULL && active) // only for missiles/cannons
    {
      if (d->source->party != party && maxshield < 2000) // calculate points
        d->source->points += impact; // extra points for shooting an enemy object
      else
        d->source->points -= impact; // subtract points for shooting an own object
      
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
    setExplosion (0.2, 30);
    setBlackSmoke (0.5, 30);
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

void DynamicObj::move ()
{
  checkExplosion ();
  if (sink) // only ships
  {
    tl->y -= 0.02;
    gamma = recgamma = 180.0 + 0.5 * (float) sink;
    return;
  }
  if (!active) return;

  if (id >= STATIC) // only buildings, static objects
  {
    if (id == STATIC_TENT1) theta = 178;
    // set the correct angles to diplay
    rot->setAngles ((short) (90 + gamma - 180), (short) theta + 180, (short) -phi);
    checkShield ();
    return; // and exit this function (optimization)
  }

  int theta0 = (int) theta; // get a normalized theta, as our sine/cosi tables only reach from 0 to 359
  while (theta0 < 0) theta0 += 360;
  while (theta0 > 360) theta0 -= 360;
  // the core of directional alterations and force calculations:
  // easymodels change heading due to roll angle
  // this may seem complete nonsense for fighters, but it is just a simplification!!!
  // angle / aileron = constant, thats enough for a simple AI
  if (easymodel)
  {
    if (id >= FLAK1 && id <= FLAK2)
    {
      phi += sine [(int) theta0] * manoeverability * 10.0;
    }
    else
    {
      float div = maxspeed + speed;
      if (div == 0) div = 1;
      phi += sine [(int) theta0] * manoeverability * 10.0 * maxspeed / div;
    }
  }
  else // now this is much more general:
  {
    int vz = 1;
    if (gamma < 90 || gamma > 270)
      vz = -1;
    // change heading and elevation due to ailerons and rudder
    if (maxspeed + speed <= -0.00001 || maxspeed + speed >= 0.00001)
    {
      phi += vz * sine [(int) theta0] * aileroneffect * manoeverability * 10.0 * maxspeed / (maxspeed + speed);
      gamma += cosi [(int) theta0] * aileroneffect * manoeverability * 10.0 * maxspeed / (maxspeed + speed);
      phi += -vz * cosi [(int) theta0] * ruddereffect * manoeverability * 2.0 * maxspeed / (maxspeed + speed);
      gamma += sine [(int) theta0] * ruddereffect * manoeverability * 2.0 * maxspeed / (maxspeed + speed);
    }
    // change roll due to roll ;-)
    if (rolleffect)
    {
      rectheta += rolleffect;
    }
  }
  if (phi < 0) phi += 360.0;
  else if (phi >= 360.0) phi -= 360.0;

  if (easymodel) // easymodel restrictions
  {
    if (rectheta > maxtheta) rectheta = maxtheta;
    else if (rectheta < -maxtheta) rectheta = -maxtheta;
    if (recgamma > 180 + maxgamma) recgamma = 180 + maxgamma;
    else if (recgamma < 180 - maxgamma) recgamma = 180 - maxgamma;
  }
  else
  {
    if (theta < -180 && rectheta < -180)
    { rectheta += 360; theta += 360; }
    else if (theta >= 180 && rectheta >= 180)
    { rectheta -= 360; theta -= 360; }
  }

/*    if (controls)
  {
  }*/

  if (recspeed > maxspeed) // check maximum throttle
    recspeed = maxspeed;
  if (recspeed > speed) // alter throttle effect slowly
    speed += maxspeed / 60;
  else if (recspeed < speed)
    speed -= maxspeed / 60;

  // axis pointing through the fighter's nose
  CVector3 vaxis (cosi [(int) gamma] * sine [(int) phi], sine [(int) gamma], cosi [(int) gamma] * cosi [(int) phi]);

  // add throttle force
  forcez += speed * vaxis.z;
  forcex += speed * vaxis.x;
  forcey -= speed * vaxis.y;

  if (id >= FIGHTER1 && id <= FIGHTER1 + 50)
  {
    // lift force
    int ngamma = (int) gamma + 90; // get normalized gamma (0 <= ngamma < 360)
    if (ngamma >= 360) ngamma -= 360;
    else if (ngamma < 0) ngamma += 360;
    CVector3 vx (cosi [ngamma] * sine [(int) phi], sine [ngamma], cosi [ngamma] * cosi [(int) phi]);
    CVector3 x1;
    x1.take (&vx);
    int mytheta = (int) -theta; // get normalized theta (0 <= mytheta < 360)
    while (mytheta < 0) mytheta += 360;
    while (mytheta >= 360) mytheta -= 360;
    x1.x *= cosi [mytheta];
    x1.y *= cosi [mytheta];
    x1.z *= cosi [mytheta];
    vx.crossproduct (&vaxis);
    vx.x *= sine [mytheta];
    vx.y *= sine [mytheta];
    vx.z *= sine [mytheta];
    vx.add (&x1);
    float addy = speed * vx.x * 0.3;
    if (addy >= -100 && addy <= 100) // valid values??? addy should be < 1.0
    {
      forcex += speed * vx.x * 0.35; // add the lifting force
      forcey -= speed * vx.y * 0.35;
      forcez += speed * vx.z * 0.35;

      // weight force (directly subtract)
      forcey -= 0.06;
    }
//      cosi [(int) phi]
  }

  // drag force simulated by just halving the vector, this is not realistic, yet easy to play
  forcex *= braking; forcez *= braking; forcey *= braking;

  bool stop = false;
  if (id >= TANK1 && id <= TANK2) // tanks cannot climb steep faces
  {
    float newy = l->getExactHeight (tl->x + forcex, tl->z + forcez) + zoom / 2;
    if (fabs (newy - tl->y) > 0.025)
      stop = true;
    else if (fabs (newy - tl->y) > 2)
      stop = false;
  }
  if (!stop)
  {
    tl->x += forcex; // add our vector to the translation
    tl->z += forcez;
    tl->y += forcey;
  }
//  tl->y = l->getHeight (tl->x, tl->z)+5;
//  gamma = 180;

  // objects moving on the ground should always change their elevation due to the surface
  if (id >= TANK1 && id <= TANK2 && speed > 0 && !stop)
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
    ttl --; // we use the ttl value as timer, for other methods ttl<0 is the same as ttl=-1
    if (ttl <= -360) ttl = -1;
    int rot1 = (int) (sin ((zoom - 1.3) * 8) * 4);
    int rot2 = (int) (cos ((zoom - 1.3) * 8) * 4);
    rot->setAngles ((short) (90 + gamma + ttl * rot1 - 180), (short) theta + ttl * rot2 + 180, (short) -phi);
  }

  // calculate the objects real speed only once
  realspeed = sqrt (forcex * forcex + forcez * forcez);

  if (ttl > 0) ttl --; // decrease time to live
  if (ttl == 0)
  {
    if (id >= MISSILE1 && id <= MISSILE2) recheight = -10; // missiles drop
    else deactivate (); // cannon shots vanish
  }
  checkShield (); // check shield issues
  crashGround (); // check ground collision
  if (immunity > 0) immunity --; // decrease immunity
}

void AIObj::aiinit ()
{
  acttype = 0;
  intelligence = 100;
  aggressivity = 100;
  precision = 100;
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
  manoeverspeed = 0;
  idle = 0;
  firemissilettl = 0;
  aw = 0;
  source = NULL;
  points = 0;
  easymodel = true;
  aileroneffect = 0;
  ruddereffect = 0;
  gamma = 180;
  theta = 0;
  maxgamma = 80;
  maxtheta = 90;
  missiletype = 0;
  autofire = false;
  ttl = -1;
  score = -1;
  for (int i = 0; i < missiletypes; i ++)
    missiles [i] = 0;
}

void AIObj::newinit (int id, int party, int intelligence, int precision, int aggressivity)
{
  ai = true;
  this->id = id;
  this->party = party;
  this->intelligence = intelligence;
  this->precision = precision;
  this->aggressivity = aggressivity;
  activate ();
  if (id == FIGHTER_FALCON)
  {
    maxspeed = 0.31;
    nimbility = 0.8;
    manoeverability = 0.45;
    maxshield = 95;
    zoom = 0.4;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missiles [0] = 6;
    missiles [3] = 2;
  }
  else if (id == FIGHTER_SWALLOW)
  {
    maxspeed = 0.23;
    nimbility = 0.6;
    manoeverability = 0.32;
    maxshield = 105;
    zoom = 0.43;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missiles [0] = 2;
    missiles [3] = 6;
  }
  else if (id == FIGHTER_HAWK)
  {
    maxspeed = 0.26;
    nimbility = 0.62;
    manoeverability = 0.35;
    maxshield = 120;
    zoom = 0.43;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missiles [0] = 4;
    missiles [3] = 6;
    missiles [5] = 2;
  }
  else if (id == FIGHTER_HAWK2)
  {
    maxspeed = 0.28;
    nimbility = 0.65;
    manoeverability = 0.38;
    maxshield = 140;
    zoom = 0.44;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missiles [1] = 2;
    missiles [3] = 0;
    missiles [4] = 6;
    missiles [5] = 2;
  }
  else if (id == FIGHTER_TRANSPORT)
  {
    maxspeed = 0.14;
    maxshield = 45;
    missiles [0] = 0;
    manoeverability = 0.1;
    impact = 5;
    zoom = 1.5;
    maxgamma = 20;
    maxtheta = 30;
  }
  else if (id == FIGHTER_BUZZARD)
  {
    maxspeed = 0.31;
    nimbility = 0.76;
    manoeverability = 0.43;
    maxshield = 80;
    zoom = 0.4;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missiles [0] = 4;
    missiles [1] = 2;
    missiles [3] = 2;
  }
  else if (id == FIGHTER_CROW)
  {
    maxspeed = 0.23;
    nimbility = 0.62;
    manoeverability = 0.35;
    maxshield = 60;
    zoom = 0.39;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missiles [0] = 2;
    missiles [3] = 1;
  }
  else if (id == FIGHTER_PHOENIX)
  {
    maxspeed = 0.3;
    nimbility = 0.5;
    manoeverability = 0.25;
    maxshield = 170;
    zoom = 0.47;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missiles [4] = 6;
    missiles [5] = 6;
  }
  else if (id == FIGHTER_REDARROW)
  {
    maxspeed = 0.33;
    nimbility = 1;
    manoeverability = 0.51;
    maxshield = 125;
    zoom = 0.41;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missiles [1] = 8;
  }
  else if (id == FIGHTER_BLACKBIRD)
  {
    maxspeed = 0.28;
    nimbility = 1.02;
    manoeverability = 0.5;
    maxshield = 90;
    zoom = 0.25;
    maxtheta = 90.0;
    maxgamma = 80.0;
    missiles [0] = 6;
  }
  if (id >= FIGHTER1 && id <= FIGHTER2)
  {
    recspeed = maxspeed / 2.0;
    shield = maxshield;
    speed = recspeed = maxspeed / 2;
    smoke->type = 1;
    impact = 2;
  }

  if (id == FLAK_AIR1)
  {
    maxspeed = 0;
    speed = 0;
    maxgamma = 0;
    maxtheta = 3;
    manoeverability = 12.0;
    shield = 80;
    zoom = 0.35;
  }
  if (id == FLARAK_AIR1)
  {
    maxspeed = 0;
    speed = 0;
    maxgamma = 0;
    maxtheta = 3;
    manoeverability = 6.0;
    shield = 70;
    zoom = 0.3;
    missiles [0] = 100;
  }

  if (id == TANK_AIR1)
  {
    maxspeed = 0.04;
    speed = 0;
    gamma = 180; theta = 0; phi = 0;
    maxgamma = 0;
    maxtheta = 3;
    manoeverability = 8.0;
    shield = 160;
    zoom = 0.35;
  }
  else if (id == TANK_GROUND1)
  {
    maxspeed = 0.04;
    speed = 0;
    gamma = 180; theta = 0; phi = 0;
    maxgamma = 0;
    maxtheta = 3;
    manoeverability = 8.0;
    shield = 200;
    zoom = 0.4;
  }
  else if (id == TANK_PICKUP1)
  {
    maxspeed = 0;
    speed = 0.02;
    maxgamma = 0;
    maxtheta = 3;
    manoeverability = 0;
    shield = 30;
    zoom = 0.25;
  }
  else if (id == TANK_TRUCK1)
  {
    maxspeed = 0;
    speed = 0.02;
    maxgamma = 0;
    maxtheta = 3;
    manoeverability = 0;
    shield = 20;
    zoom = 0.45;
  }

  if (id == SHIP_CRUISER)
  {
    zoom = 4.0;
    maxspeed = 0;
    speed = 0;
    maxgamma = 0;
    maxtheta = 3;
    manoeverability = 4.0;
    impact = 20;
    shield = 2800;
    missiles [1] = 200;
  }
  else if (id == SHIP_DESTROYER1)
  {
    zoom = 2.0;
    maxspeed = 0;
    speed = 0;
    maxgamma = 0;
    maxtheta = 3;
    manoeverability = 6.0;
    impact = 20;
    shield = 1600;
  }

  if (id == MISSILE_AIR1)
  {
    intelligence = 100;
    maxspeed = 0.4;
    nimbility = 2.2;
    manoeverability = 2.2;
    ttl = 200;
    impact = 30;
  }
  else if (id == MISSILE_AIR2)
  {
    intelligence = 50;
    maxspeed = 0.42;
    nimbility = 3.5;
    manoeverability = 2.8;
    ttl = 220;
    impact = 40;
  }
  else if (id == MISSILE_AIR3)
  {
    intelligence = 0;
    maxspeed = 0.44;
    nimbility = 5.5;
    manoeverability = 3.5;
    ttl = 250;
    impact = 50;
  }
  else if (id == MISSILE_GROUND1)
  {
    intelligence = 50;
    maxspeed = 0.48;
    nimbility = 1.2;
    manoeverability = 1.0;
    ai = true;
    ttl = 250;
    impact = 250;
  }
  else if (id == MISSILE_GROUND2)
  {
    intelligence = 0;
    maxspeed = 0.50;
    nimbility = 1.5;
    manoeverability = 1.0;
    ai = true;
    ttl = 310;
    impact = 300;
  }
  else if (id == MISSILE_DF1)
  {
    intelligence = 0;
    maxspeed = 0.50;
    nimbility = 0.0;
    manoeverability = 0.0;
    ai = true;
    ttl = 300;
    impact = 650;
  }
  else if (id == MISSILE_MINE1)
  {
    maxspeed = 0.08;
    nimbility = 1.5;
    manoeverability = 1.0;
    ai = true;
    ttl = -1;
    impact = 500;
    zoom = 0.3;
  }

  if (id >= STATIC)
  {
    intelligence = 0;
    maxspeed = 0;
    nimbility = 0;
    manoeverability = 0;
    impact = 5;
    maxtheta = 0;
    maxgamma = 0;
  }
  if (id == STATIC_TENT1)
  {
    shield = 80;
    zoom = 0.5;
  }
  if (id == STATIC_TENT4)
  {
    shield = 150;
    zoom = 1.2;
  }
  if (id == STATIC_CONTAINER1)
  {
    shield = 30;
    zoom = 1.0;
    impact = 20;
  }
  if (id == STATIC_HALL1)
  {
    shield = 250;
    zoom = 1.8;
    impact = 20;
  }
  if (id == STATIC_HALL2)
  {
    shield = 400;
    zoom = 2.5;
    impact = 20;
  }
  if (id == STATIC_OILRIG1)
  {
    shield = 1300;
    zoom = 3.5;
    impact = 20;
  }
  if (id == STATIC_COMPLEX1)
  {
    shield = 2000;
    zoom = 2.0;
    impact = 20;
  }
  if (id == STATIC_RADAR1)
  {
    shield = 400;
    zoom = 1.3;
    impact = 20;
  }
  if (id == ASTEROID)
  {
    shield = 30000;
    zoom = 0.01 * myrandom (60) + 1.0;
    impact = 5;
    speed = 0.18;
    maxspeed = 0.18;
    ai = false;
  }
  if (id == STATIC_BASE1)
  {
    shield = 4000;
    zoom = 4.0;
    impact = 20;
  }

  if (difficulty == 0)
  {
    intelligence = 400 - (400 - intelligence) / 2;
    precision = 400 - (400 - precision) / 2;
    aggressivity = 400 - (400 - aggressivity) / 2;
  }
  else if (difficulty == 2)
  {
    intelligence /= 2;
    precision /= 2;
    aggressivity /= 2;
  }
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
  // Hier bitte exakte Kugelkoordinaten auf Grund von gamma und theta verwenden!
  dobj->tl->x = tl->x + cosi [(int) gamma] * sine [(int) phi] * zoom;
  dobj->tl->y = tl->y - sine [(int) gamma] * zoom;
  if ((id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
    dobj->tl->y += zoom;
  dobj->tl->z = tl->z + cosi [(int) gamma] * cosi [(int) phi] * zoom;
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
  laser->speed = speed + 0.4;
  laser->realspeed = realspeed + 0.4;
  laser->recspeed = laser->speed;
  laser->manoeverability = 0.0;
  laser->maxspeed = 1.0;
  if (target != NULL && ai)
  {
    if (target->active)
    {
      float timelaser = 0.0001;
      if (fabs (laser->realspeed - target->realspeed) > 0.0001)
        timelaser = distance (target) / (laser->realspeed - target->realspeed); // urks, should be target->speed*(norm2(v_target||v_laser))
      laser->gamma = 180;
      if (fabs (timelaser) > 0.00001 && fabs (laser->realspeed) > 0.00001)
        laser->gamma = 180.0 + atan ((target->tl->y - tl->y) / timelaser / laser->realspeed) * 180.0 / pitab;
    }
  }
  else
    laser->gamma = gamma; // + 90.0;
  laser->party = party;
  laser->ttl = 65;
  laser->shield = 1;
  laser->immunity = (int) (zoom * 12);
  laser->source = this;
  laser->phi = phi;
  initValues (laser, phi);
  laser->activate ();
}

void AIObj::fireCannon (DynamicObj **laser, float phi)
{
  int i;
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
  fireCannon (laser, phi);
}

void AIObj::fireMissile2 (int id, AIObj *missile, AIObj *target)
{
  missile->dinit ();
  missile->aiinit ();
  missile->newinit (id, party, 0);
  initValues (missile, phi);
  missile->id = MISSILE1;
  missile->explode = 0;
  missile->speed = speed + 0.0005;
  missile->recspeed = 0.5;
//    missile->gamma = gamma / 2.0 + 90.0;
  missile->gamma = gamma;
  missile->target = target;
  missile->recgamma = gamma;
  missile->shield = 1;
//    missile->recheight = 5;
  missile->party = party;
  missile->immunity = 20 + (int) (zoom * 6.0);
  missile->dtheta = 0;
  missile->dgamma = 0;
  missile->source = this;
  missile->activate ();
  if (id >= FIGHTER1 && id <= FIGHTER2)
  {
    missile->manoeverheight = 30;
    missile->recheight = missile->tl->y - l->getHeight (missile->tl->x, missile->tl->z) - 4;
  }
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
  return i;
}

bool AIObj::haveMissile (int id)
{
  if (missiles [id - MISSILE1] > 0)
    return true;
  return false;
}

void AIObj::decreaseMissile (int id)
{
  missiles [id - MISSILE1] --;
}

void AIObj::fireMissile (int id, AIObj **missile, AIObj *target)
{
  int i;
  if (!haveMissile (id)) return;
  for (i = 0; i < maxmissile; i ++)
  {
    if (missile [i]->ttl <= 0) break;
  }
  if (i < maxmissile)
  {
    fireMissile2 (id, missile [i], target);
    decreaseMissile (id);
    firemissilettl = 100;
  }
}

void AIObj::fireMissile (int id, AIObj **missile)
{
  fireMissile (id, missile, (AIObj *) target);
}

void AIObj::fireMissileAir (AIObj **missile, AIObj *target)
{
  if (haveMissile (MISSILE_AIR3))
    fireMissile (MISSILE_AIR3, missile, (AIObj *) target);
  else if (haveMissile (MISSILE_AIR2))
    fireMissile (MISSILE_AIR2, missile, (AIObj *) target);
  else if (haveMissile (MISSILE_AIR1))
    fireMissile (MISSILE_AIR1, missile, (AIObj *) target);
}

void AIObj::fireMissileGround (AIObj **missile)
{
  if (haveMissile (MISSILE_GROUND2))
    fireMissile (MISSILE_GROUND2, missile, (AIObj *) target);
  else if (haveMissile (MISSILE_GROUND1))
    fireMissile (MISSILE_GROUND1, missile, (AIObj *) target);
}

void AIObj::targetNearestEnemy (AIObj **f)
{
  int i;
  float d = 10000;
  for (i = 0; i < maxfighter; i ++)
  {
    if (this != f [i] && party != f [i]->party && f [i]->active)
    {
      float d2 = distance (f [i]);
      if (d2 < d)
      {
        d = d2;
        target = f [i];
      }
    }
  }
}

void AIObj::targetNext (AIObj **f)
{
  int i;
//    float d = 10000;
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
  } while (!f [i]->active && z <= 1);
  target = f [i];
}

void AIObj::targetPrevious (AIObj **f)
{
  int i, lastfighter = 0;
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
  target = f [i];
}

void AIObj::aiAction (AIObj **f, AIObj **m, DynamicObj **c)
{
  if (debug)
  { printf ("a("); fflush (stdout); }
  if (!active && !draw)
  {
    if (debug)
    { printf (")"); fflush (stdout); }
    return;
  }

  if (firemissilettl > 0) firemissilettl --;

  // Move object according to our physics
  move ();

  if (id >= STATIC)
    return;

  // Set smoke
//    if (ai) // at the moment no smoke for player's fighter
  if ((id >= MISSILE1 && id < MISSILE_MINE1) || (id >= FIGHTER1 && id <= FIGHTER2))
  {
    float sz = cosi [(int) gamma] * cosi [(int) phi] * zoom * 1.1;
    float sy = -sine [(int) gamma] * zoom * 1.1;
    float sx = cosi [(int) gamma] * sine [(int) phi] * zoom * 1.1;
    smoke->move ();
    smoke->setSmoke (tl->x - sx - forcex * 0.67, tl->y - sy - forcey * 0.67, tl->z - sz - forcez * 0.67, (int) phi, 8);
    smoke->setSmoke (tl->x - sx - forcex * 0.33, tl->y - sy - forcey * 0.33, tl->z - sz - forcez * 0.33, (int) phi, 9);
    smoke->setSmoke (tl->x - sx, tl->y - sy, tl->z - sz, (int) phi, 10);
  }

  if (!active)
  {
    if (debug)
    { printf (")"); fflush (stdout); }
    return;
  }
  if (explode || sink)
  {
    if (debug)
    { printf (")"); fflush (stdout); }
    return;
  }

  // Do expensive calculations only once
  float myheight = l->getExactHeight (tl->x, tl->z);
  float targetheight = tl->y;
  if (target != NULL)
    targetheight = l->getExactHeight (target->tl->x, target->tl->z);

  // Which height???
  float recheight2; // this is the height, the object wants to achieve
  float flyx = tl->x + forcex * 6, flyz = tl->z + forcez * 6;
  if (ttl != 0)
  {
    if (manoeverheight)
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
        recheight2 = target->tl->y - 8 * target->speed * sine [(int) target->gamma];
      }
      else
      {
        // precalculated height
        recheight2 = l->getExactHeight (flyx, flyz) + recheight;
      }
    }
  }
  else recheight2 = -100;

  if (manoeverheight > 0) manoeverheight --;
  if (!manoeverheight)
  {
    if (!(id >= FIGHTER1 && id <= FIGHTER2) && target != NULL)
    {
      recheight = target->tl->y - targetheight;
    }
    else if (id >= FIGHTER1 && id <= FIGHTER2 && target != NULL)
    {
      if (target->id >= FIGHTER1 && target->id <= FIGHTER2)
        recheight = target->tl->y - targetheight;
      else
        recheight = target->tl->y - targetheight + 5;
      if (!l->isWater (l->f [(int) flyx] [(int) flyz]))
      {
        if (recheight < 3.5 + 0.005 * aggressivity) recheight = 3.5 + 0.005 * aggressivity;
        if (maxgamma < 40)
          if (recheight < 20) recheight = 20;
      }
      float minh = 2.0 + 0.005 * aggressivity;
      if (l->type == LAND_CANYON) minh = 5.0 + 0.005 * aggressivity;
      if (fabs (l->getHeight (flyx, flyz) - myheight) < minh)
      {
        recheight = 8 + 0.01 * aggressivity;
        manoeverheight = 30;
      }
/*        if (myrandom (intelligence + 50) == 0)
      {
        recheight = 10.0 * fabs (l->getHeight (flyx, flyz) - myheight);
        manoeverheight = 15;
      }*/
    }
  }

  if (ai)
  {
    if (id >= MISSILE_GROUND1 && id <= MISSILE_GROUND2)
    {
      float dgamma = 0;
      if (disttarget <= -0.00001 || disttarget >= 0.00001)
        dgamma = atan ((target->tl->y - tl->y) / disttarget) * 180 / PI - (gamma - 180);
      recgamma = gamma + dgamma;
    }
    else
      recgamma = (int) ((recheight2 - tl->y) * 10 - gamma + 360);
  }

//  recgamma = 180 + atan ((recheight2 - tl->y) / speed) * 180 / pitab;
//  if (recgamma < -80) recgamma = -80;
//  if (recgamma > 80) recgamma = 80;
/*  if (recgamma > gamma + 4) gamma += (recgamma - gamma) / 8 + 1;
  else if (recgamma < gamma - 4) gamma += (recgamma - gamma) / 8 - 1;
  if (gamma > 250) gamma = 250;
  if (gamma < 110) gamma = 110;*/

//  float nimbility;
float nimbility2 = nimbility;
if (nimbility2 >= -0.00001 && nimbility2 <= 0.00001)
  nimbility2 = 0.00001;
float deltatheta = rectheta - theta;
//  nimbility = 1.0;
if (deltatheta > 0 && dtheta < 0) dtheta += nimbility;
else if (deltatheta < 0 && dtheta > 0) dtheta -= nimbility;
else if (deltatheta > 0)
{
  float estimatedtheta = dtheta * (dtheta + nimbility) / 2 / nimbility2;
  if (deltatheta > estimatedtheta + 2) dtheta += nimbility;
  else if (deltatheta < estimatedtheta - 2) dtheta -= nimbility;
}
else
{
  float estimatedtheta = -dtheta * (dtheta - nimbility) / 2 / nimbility2;
  if (deltatheta < estimatedtheta - 2) dtheta -= nimbility;
  else if (deltatheta > estimatedtheta + 2) dtheta += nimbility;
}
theta += dtheta;

if (easymodel)
{
if (theta > maxtheta) theta = maxtheta;
else if (theta < -maxtheta) theta = -maxtheta;

float deltagamma = recgamma - gamma;
//  nimbility = maxspeed * 2;
if (deltagamma > 0 && dgamma < 0) dgamma += nimbility;
else if (deltagamma < 0 && dgamma > 0) dgamma -= nimbility;
else if (deltagamma > 0)
{
  float estimatedgamma = dgamma * (dgamma + nimbility) / nimbility2;
  if (deltagamma > estimatedgamma + 2) dgamma += nimbility;
  else if (deltagamma < estimatedgamma - 2) dgamma -= nimbility;
}
else if (deltagamma < 0)
{
  float estimatedgamma = -dgamma * (dgamma - nimbility) / nimbility2;
  if (deltagamma < estimatedgamma - 2) dgamma -= nimbility;
  else if (deltagamma > estimatedgamma + 2) dgamma += nimbility;
}
gamma += dgamma;

}

if (easymodel)
{
  if (gamma > 180 + maxgamma) gamma = 180 + maxgamma;
  else if (gamma < 180 - maxgamma) gamma = 180 - maxgamma;
}
else
{
  (void) checkLooping ();
}

/*    if (gamma > 180 + maxgamma) gamma = 180 + maxgamma;
  else if (gamma < 180 - maxgamma) gamma = 180 - maxgamma;*/

  if (target == NULL)
    targetNearestEnemy (f);
  if (target != NULL)
    if (!target->active)
      targetNearestEnemy (f);

  if (!ai || target == NULL)
  {
    if (debug)
    { printf (")"); fflush (stdout); }
    return;
  }

disttarget = distance (target); // distance to target

float dx2, dz2, ex, ez;
float dx = target->tl->x - tl->x, dz = target->tl->z - tl->z; // actual distances
if ((id >= FIGHTER1 && id <= FIGHTER2) || (id >= MISSILE1 && id <= MISSILE2) || (id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
{
  float t = 10.0 * disttarget; // generous time to new position
  if (t > 60) t = 60; // higher values will not make sense
  int tt = (int) target->theta;
  if (tt < 0) tt += 360;
  float newphi = t * sine [tt] * 5.0 * target->manoeverability; // new angle of target after time t
  if (newphi > 90) newphi = 90;
  else if (newphi < -90) newphi = -90;
  newphi += (float) target->phi;
  if (newphi >= 360) newphi -= 360;
  if (newphi < 0) newphi += 360;
  if ((id >= FIGHTER1 && id <= FIGHTER2) || (id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
  {
    ex = target->tl->x - sine [(int) newphi] * t * target->realspeed * 0.25; // estimated target position x
    ez = target->tl->z - cosi [(int) newphi] * t * target->realspeed * 0.25; // estimated target position z
  }
  else
  {
    ex = target->tl->x - sine [(int) newphi] * t * target->realspeed * 0.05; // estimated target position x
    ez = target->tl->z - cosi [(int) newphi] * t * target->realspeed * 0.05; // estimated target position z
  }
  dx2 = ex - tl->x; dz2 = ez - tl->z; // estimated distances
}
else
{
  dx2 = dx; dz2 = dz;
}
  int a, w = (int) phi;
  if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;

/*  if (intelligence == 0)
{
m [0]->tl->x = ex; //tl->x + dx2; // visualize estimated point by a missile
m [0]->tl->z = ez; //tl->z + dz2;
m [0]->active = false;
m [0]->draw = true;
m [0]->tl->y = target->tl->y;
}*/

  a = (int) (atan (dx2 / dz2) * 180 / PI);
  if (dz2 > 0)
  {
    if (dx2 > 0) a -= 180;
    else a += 180;
  }
//    this->aw = a;
  aw = a - w;
  if (aw < -180) aw += 360;
  if (aw > 180) aw -= 360;

  if (manoevertheta > 0) manoevertheta --;
  if (manoeverspeed > 0) manoeverspeed --;
  if (id >= FIGHTER1 && id <= FIGHTER2)
  {
  if (!acttype && disttarget <= 1000 && !manoevertheta)
  {
    if (aw > 0)
    {
      if (aw > 180 - intelligence / 20 && disttarget < 15)
      {
        rectheta = -90;
        manoevertheta = 10 + myrandom ((400 - intelligence) / 4);
        if (!manoeverspeed)
          recspeed = maxspeed - myrandom (intelligence) * 0.0003;
        if (!manoeverheight)
        { recheight = 5; manoeverheight = 8 - intelligence / 50; }
      }
      else
      {
        int maw = aw > 90 ? 90 : aw;
        maw = 90 - maw;
        rectheta = 90 - maw * intelligence / 100;
      }
    }
    else
    {
      if (aw < -180 + intelligence / 20 && disttarget < 15)
      {
        rectheta = 90;
        manoevertheta = 10 + myrandom ((400 - intelligence) / 4);
        if (!manoeverspeed)
          recspeed = maxspeed - myrandom (intelligence) * 0.0003;
        if (!manoeverheight)
        { recheight = 5; manoeverheight = 8 - intelligence / 50; }
      }
      else
      {
        int maw = aw < -90 ? -90 : aw;
        maw = -90 - maw;
        rectheta = -90 - maw * intelligence / 100;
      }
//        rectheta = -90;
    }
//      if (rectheta > theta + 5.0) toLeft ();
//      else if (rectheta < theta - 5.0) toRight ();
  }
  }
  else if (id >= MISSILE1 && id <= MISSILE2)
  {
    if (abs (aw) < 50 && disttarget > 10)
      rectheta = 0;
    else
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
  else if (id >= FLAK1 && id <= FLAK2)
  {
    recspeed = 0; speed = 0;
    if (aw > 0)
    {
      rectheta = aw > 90 ? 90 : aw;
    }
    else
    {
      rectheta = aw < -90 ? -90 : aw;
    }
  }
  else if (id >= TANK1 && id <= TANK2)
  {
    recspeed = maxspeed; speed = maxspeed;
    if (aw > 0)
    {
      rectheta = maxtheta;
    }
    else
    {
      rectheta = -maxtheta;
    }
    if (id == TANK_AIR1)
      if (fabs (rectheta - theta) < 2 && fabs (aw) < 20 && disttarget < 30 && target->tl->y > tl->y + 2)
        if (myrandom (intelligence) < 80)
          fireCannon (c);
    if (id == TANK_GROUND1)
      if (fabs (rectheta - theta) < 2 && fabs (aw) < 20 && disttarget < 30 && target->tl->y <= tl->y + 1 && target->tl->y >= tl->y - 1)
        if (myrandom (intelligence) < 100)
          fireCannon (c);
  }
  if (id >= FIGHTER1 && id <= FIGHTER2)
  {
    if (disttarget > 2.5 + aggressivity / 100)
    {
      if (disttarget < 50 && abs (aw) > 30 && !manoeverspeed)
        recspeed = maxspeed / 2;
      else speedUp ();
    }
    else if (!manoeverspeed)
    {
      if (recspeed > target->speed)
      {
        speedDown ();
      }
      else
      {
        speedUp ();
      }
    }
    if (disttarget > 60 && abs (aw) < 20)
      rectheta = 0; // fly straight
    if (disttarget < 10 && abs (aw) > 50 && (target->id < FIGHTER1 || target->id > FIGHTER2))
    {
      manoevertheta = 50;
      rectheta = 0;
      manoeverspeed = 50;
      recspeed = maxspeed;
    }
    float agr = 4.0 - aggressivity / 100;
    if (fabs (rectheta - theta) < 2 + agr && fabs (aw) < 20 + agr * 4 && disttarget < 30)
      fireCannon (c);
    else if (disttarget < 2 + agr && fabs (aw) < 20 + agr * 4)
      fireCannon (c);
    if (!firemissilettl)
    {
      if (target->id >= FIGHTER1 && target->id <= FIGHTER2)
      {
        if (fabs (rectheta - theta) < 2 + agr && fabs (aw) < 20 + agr * 4 && disttarget < 30 && target->theta < 20)
          fireMissileAir (m, (AIObj *) target);
      }
      else // ground target
      {
        if (fabs (rectheta - theta) < 2 + agr && fabs (aw) < 20 + agr * 4 && disttarget < 50)
          fireMissileGround (m);
      }
    }
    if (!manoevertheta)
    {
      rectheta += myrandom (precision) - precision / 2;
      if (rectheta > 90 - precision / 5) rectheta = 90 - precision / 5;
      else if (rectheta < -90 + precision / 5) rectheta = -90 + precision / 5;
    }
//      if (recspeed > maxspeed - 0.001 * intelligence)
//        recspeed = maxspeed - 0.001 * intelligence;
    idle ++;
    if (idle > 400)
    {
      idle = 0;
      if (rectheta < 0)
      { rectheta = 90; manoevertheta = 50 + myrandom (100); }
      if (rectheta > 0)
      { rectheta = -90; manoevertheta = 50 + myrandom (100); }
    }
  }

  if ((id >= FLAK1 && id <= FLAK2) || id == SHIP_CRUISER || id == SHIP_DESTROYER1)
  {
    if (!(l->lsticker & 4))
    for (int i = 0; i < maxfighter; i ++)
    if (party != f [i]->party)
    {
      disttarget = distance (f [i]); // distance to target
      ex = f [i]->tl->x; // estimated target position x
      ez = f [i]->tl->z; // estimated target position z
      dx2 = ex - tl->x; dz2 = ez - tl->z; // estimated distances
      w = (int) phi;
      if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;
      a = (int) (atan (dx2 / dz2) * 180 / PI);
      if (dz2 > 0)
      {
        if (dx2 > 0) a -= 180;
        else a += 180;
      }
      aw = a - w;
      if (aw < -180) aw += 360;
      if (aw > 180) aw -= 360;
      int firerate;
      if (difficulty == 0) firerate = 7;
      else if (difficulty == 1) firerate = 3;
      else firerate = 1;
      if (id == FLAK_AIR1 || id == SHIP_DESTROYER1)
        if (fabs (aw) < 35 && disttarget < 40) // + aggressive
          if (f [i]->tl->y > tl->y + 2)
//              if (myrandom (intelligence) < 120)
            if (!(l->lsticker & firerate))
              fireCannon (c);
      if (id == FLARAK_AIR1)
        if (!(l->lsticker & 31))
          if (fabs (aw) < 25 && disttarget < 40) // + aggressive
            if (f [i]->tl->y > tl->y + 2)
              if (myrandom (intelligence) < 80)
                fireMissileAir (m, f [i]);
      if (id == SHIP_CRUISER)
      {
        if (!(l->lsticker & 15))
          if (fabs (aw) >= -30 && fabs (aw) < 30 && disttarget < 40) // + aggressive
            if (myrandom (intelligence) < 100)
            {
              fireMissileAir (m, f [i]);
  //            missiles ++;
            }
//          if (myrandom (intelligence) < 120)
        if (!(l->lsticker & firerate))
        {
          if (fabs (aw) >= 0 && fabs (aw) < 40 && disttarget < 40) // + aggressive
            fireCannon (c, aw);
          if (fabs (aw) >= -40 && fabs (aw) < 0 && disttarget < 40) // + aggressive
            fireCannon (c, aw + 360);
          if (fabs (aw) >= 120 && fabs (aw) < 160 && disttarget < 40) // + aggressive
            fireCannon (c, aw);
          if (fabs (aw) >= -160 && fabs (aw) < -120 && disttarget < 40) // + aggressive
            fireCannon (c, aw + 360);
        }
      }
    }
    rectheta += myrandom (intelligence) - intelligence / 2;
    if (rectheta > 90 - intelligence / 5) rectheta = 90 - intelligence / 5;
    else if (rectheta < -90 + intelligence / 5) rectheta = -90 + intelligence / 5;
  }
  if (debug)
  { printf (")"); fflush (stdout); }
}

#endif
