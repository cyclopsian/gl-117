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

#include "assert.h"


AIObj::AIObj ()
{
  o = NULL;
  trafo.scaling.set (1.0, 1.0, 1.0);
  aiinit ();
  smoke = new Smoke (0);
}

AIObj::AIObj (Space *space2, Model3d *o2, float zoom2)
{
  space = space2;
  o = o2;
  trafo.scaling.set (zoom2, zoom2, zoom2);
  aiinit ();
  smoke = new Smoke (0);
  space->addObject (this);
}

AIObj::~AIObj ()
{
  delete smoke;
}

void AIObj::aiinit ()
{
  int i;
  acttype = 0;
  dualshot = false;
  intelligence = 100;
  aggressivity = 100;
  precision = 100;
  shield = 0.01F;
  ai = true;
  active = true;
  draw = true;
  target = NULL;
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
  stat.points = 0;
  easymodel = 1;
  elevatoreffect = 0;
  ruddereffect = 0;
  currot.gamma = 180;
  recrot.gamma = 180;
  dgamma = 0;
  currot.theta = 0;
  maxrot.gamma = 70;
  maxrot.theta = 90;
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
  manoeverstate = 0;
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
  float zoom;
  ai = true;
  this->id = id;
  this->party = party;
  manoeverstate = 0;
  activate ();
  for (i = 0; i < missileracks; i ++)
    missilerackn [i] = 0;
  ammo = -1;
  bomber = 0;
  dualshot = false;
  float cubefac = 0.6F; // fighter
  float cubefac1 = 0.7F; // tanks and sams
  o = getModel (id);
  o->cube.set (trafo.scaling);
  
  if (id == FIGHTER_FALCON)
  {
    maxthrust = 0.31;
    nimbility = 0.86;
    manoeverability = 0.48;
    maxshield = 85;
    zoom = 0.35;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 2;
    missilerack [0] = 0; missilerack [1] = 6; missilerack [2] = 6; missilerack [3] = 0;
    flares = 20;
    chaffs = 20;
    statfirepower = 1;
    ammo = 1200;
  }
  else if (id == FIGHTER_SWALLOW)
  {
    maxthrust = 0.24;
    nimbility = 0.64;
    manoeverability = 0.35;
    maxshield = 110;
    zoom = 0.43;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 2;
    missilerack [0] = 6; missilerack [1] = 3; missilerack [2] = 3; missilerack [3] = 6;
    flares = 20;
    chaffs = 20;
    bomber = 1;
    statfirepower = 3;
    ammo = 1200;
  }
  else if (id == FIGHTER_HAWK)
  {
    maxthrust = 0.26;
    nimbility = 0.72;
    manoeverability = 0.42;
    maxshield = 120;
    zoom = 0.43;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 1; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 3; missilerack [2] = 3; missilerack [3] = 6;
    flares = 20;
    chaffs = 20;
    bomber = 1;
    statfirepower = 2;
    ammo = 1200;
  }
  else if (id == FIGHTER_HAWK2)
  {
    maxthrust = 0.28;
    nimbility = 0.75;
    manoeverability = 0.44;
    maxshield = 140;
    zoom = 0.45;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 1; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 4; missilerack [2] = 4; missilerack [3] = 6;
    flares = 20;
    chaffs = 20;
    bomber = 1;
    statfirepower = 3;
    ammo = 1400;
    dualshot = true;
  }
  else if (id == FIGHTER_TRANSPORT)
  {
    maxthrust = 0.14;
    maxshield = 45;
    missiles [0] = 0;
    nimbility = 0.15;
    manoeverability = 0.05;
    impact = 5;
    zoom = 1.5;
    maxrot.gamma = 25;
    maxrot.theta = 30;
    flares = 0;
    chaffs = 0;
    ammo = 0;
  }
  else if (id == FIGHTER_TRANSPORT2)
  {
    maxthrust = 0.16;
    maxshield = 35;
    missiles [0] = 0;
    nimbility = 0.12;
    manoeverability = 0.04;
    impact = 5;
    zoom = 1.5;
    maxrot.gamma = 25;
    maxrot.theta = 30;
    flares = 0;
    chaffs = 0;
    ammo = 0;
  }
  else if (id == FIGHTER_BUZZARD)
  {
    maxthrust = 0.31;
    nimbility = 0.82;
    manoeverability = 0.46;
    maxshield = 75;
    zoom = 0.44;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 2;
    missilerack [0] = 0; missilerack [1] = 6; missilerack [2] = 6; missilerack [3] = 0;
    flares = 20;
    chaffs = 20;
    statfirepower = 2;
    ammo = 1200;
  }
  else if (id == FIGHTER_CROW)
  {
    maxthrust = 0.25;
    nimbility = 0.72;
    manoeverability = 0.4;
    maxshield = 60;
    zoom = 0.41;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 1; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 0; missilerack [2] = 0; missilerack [3] = 6;
    flares = 20;
    chaffs = 20;
    statfirepower = 1;
    ammo = 1000;
  }
  else if (id == FIGHTER_STORM)
  {
    maxthrust = 0.25;
    nimbility = 0.52;
    manoeverability = 0.34;
    maxshield = 160;
    zoom = 0.45;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 1; missilerackn [1] = 2; missilerackn [2] = 2; missilerackn [3] = 1;
    missilerack [0] = 6; missilerack [1] = 0; missilerack [2] = 0; missilerack [3] = 6;
    flares = 25;
    chaffs = 25;
    statfirepower = 4;
    ammo = 1800;
  }
  else if (id == FIGHTER_PHOENIX)
  {
    maxthrust = 0.3;
    nimbility = 0.54;
    manoeverability = 0.34;
    maxshield = 180;
    zoom = 0.47;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 3; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 3;
    missilerack [0] = 4; missilerack [1] = 4; missilerack [2] = 4; missilerack [3] = 4;
    flares = 25;
    chaffs = 25;
    bomber = 1;
    statfirepower = 5;
    ammo = 2000;
    dualshot = true;
  }
  else if (id == FIGHTER_REDARROW)
  {
    maxthrust = 0.33;
    nimbility = 0.95;
    manoeverability = 0.52;
    maxshield = 120;
    zoom = 0.4;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 2;
    missilerack [0] = 7; missilerack [1] = 1; missilerack [2] = 1; missilerack [3] = 7;
    flares = 25;
    chaffs = 25;
    statfirepower = 2;
    ammo = 1400;
    dualshot = true;
  }
  else if (id == FIGHTER_BLACKBIRD)
  {
    maxthrust = 0.3;
    nimbility = 1.0;
    manoeverability = 0.54;
    maxshield = 85;
    zoom = 0.33;
    maxrot.theta = 90.0;
    maxrot.gamma = 70.0;
    missilerackn [0] = 2; missilerackn [1] = 3; missilerackn [2] = 3; missilerackn [3] = 2;
    missilerack [0] = 7; missilerack [1] = 1; missilerack [2] = 1; missilerack [3] = 7;
    flares = 25;
    chaffs = 25;
    statfirepower = 2;
    ammo = 1400;
    dualshot = true;
  }
  if (id >= FIGHTER1 && id <= FIGHTER2)
  {
    recthrust = maxthrust / 2.0;
    shield = maxshield;
    thrust = recthrust = maxthrust / 2;
    smoke->type = 1;
    impact = 2;
    force.z = recthrust;
    o->cube.set (zoom * cubefac, zoom * cubefac, zoom * cubefac);
  }

  if (id == FLAK_AIR1)
  {
    maxthrust = 0;
    thrust = 0;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 12.0;
    shield = maxshield = 80;
    zoom = 0.35;
  }
  if (id == FLARAK_AIR1)
  {
    maxthrust = 0;
    thrust = 0;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 6.0;
    shield = maxshield = 70;
    zoom = 0.3;
    missiles [6] = 100;
  }
  if (id >= FLAK1 && id <= FLAK2)
  {
    o->cube.set (zoom * cubefac1, zoom * cubefac1, zoom * cubefac1);
  }

  if (id == TANK_AIR1)
  {
    maxthrust = 0.04;
    thrust = 0;
    currot.gamma = 180; currot.theta = 0; currot.phi = 0;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 8.0;
    shield = maxshield = 160;
    zoom = 0.35;
    o->cube.set (zoom * 0.7, zoom * 0.45, zoom * 0.7);
  }
  else if (id == TANK_GROUND1)
  {
    maxthrust = 0.04;
    thrust = 0;
    currot.gamma = 180; currot.theta = 0; currot.phi = 0;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 8.0;
    shield = maxshield = 200;
    zoom = 0.4;
    o->cube.set (zoom * 0.7, zoom * 0.5, zoom * 0.7);
  }
  else if (id == TANK_PICKUP1)
  {
    maxthrust = 0;
    thrust = 0.02;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 0;
    shield = maxshield = 30;
    zoom = 0.25;
    o->cube.set (zoom * 0.7, zoom * 0.55, zoom * 0.7);
  }
  else if (id == TANK_TRUCK1)
  {
    maxthrust = 0;
    thrust = 0.02;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 0;
    shield = maxshield = 20;
    zoom = 0.45;
    o->cube.set (zoom * 0.6, zoom * 0.35, zoom * 0.6);
  }
  else if (id == TANK_TRUCK2)
  {
    maxthrust = 0;
    thrust = 0.02;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 0;
    shield = maxshield = 40;
    zoom = 0.4;
    o->cube.set (zoom * 0.6, zoom * 0.35, zoom * 0.6);
  }
  else if (id == TANK_TRSAM1)
  {
    maxthrust = 0;
    thrust = 0.02;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 0;
    shield = maxshield = 50;
    zoom = 0.35;
    missiles [6] = 200;
    o->cube.set (zoom * 0.7, zoom * 0.6, zoom * 0.7);
  }
  if (id >= TANK1 && id <= TANK2)
  {
  }

  if (id == SHIP_CRUISER)
  {
    zoom = 5.0;
    maxthrust = 0.05;
    thrust = 0.05;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 4.0;
    impact = 20;
    shield = maxshield = 5500;
    missiles [6] = 200;
    o->cube.set (zoom * 0.35, zoom * 0.1, zoom * 0.35);
  }
  else if (id == SHIP_DESTROYER1)
  {
    zoom = 2.5;
    maxthrust = 0.05;
    thrust = 0.05;
    maxrot.gamma = 0;
    maxrot.theta = 0.03;
    manoeverability = 6.0;
    impact = 20;
    shield = maxshield = 2800;
    o->cube.set (zoom * 0.4, zoom * 0.12, zoom * 0.4);
  }

  float missilethrustbase = 1.2F;
  if (id == MISSILE_AIR1)
  {
    intelligence = 100;
    maxthrust = 0.7 * missilethrustbase;
    nimbility = 2.5; // old 2.2
    manoeverability = 1.5;
    ttl = 300 * timestep;
    impact = 35;
  }
  else if (id == MISSILE_AIR2)
  {
    intelligence = 50;
    maxthrust = 0.75 * missilethrustbase;
    nimbility = 3.5; // old 3.5
    manoeverability = 2.0;
    ttl = 320 * timestep;
    impact = 45;
  }
  else if (id == MISSILE_AIR3)
  {
    intelligence = 0;
    maxthrust = 0.8 * missilethrustbase;
    nimbility = 4.5;
    manoeverability = 2.5;
    ttl = 340 * timestep;
    impact = 55;
  }
  else if (id == MISSILE_GROUND1)
  {
    intelligence = 50;
    maxthrust = 0.75 * missilethrustbase;
    nimbility = 1.2;
    manoeverability = 1.0;
    ai = true;
    ttl = 300 * timestep;
    impact = 400;
  }
  else if (id == MISSILE_GROUND2)
  {
    intelligence = 0;
    maxthrust = 0.8 * missilethrustbase;
    nimbility = 1.5;
    manoeverability = 1.0;
    ai = true;
    ttl = 400 * timestep;
    impact = 500;
  }
  else if (id == MISSILE_DF1)
  {
    intelligence = 0;
    maxthrust = 0.75 * missilethrustbase;
    nimbility = 0.0;
    manoeverability = 0.0;
    ai = true;
    ttl = 350 * timestep;
    impact = 920;
  }
  else if (id == MISSILE_FF1)
  {
    intelligence = 0;
    maxthrust = 0.8 * missilethrustbase;
    nimbility = 2.0;
    manoeverability = 1.3;
    ttl = 300 * timestep;
    impact = 40;
  }
  else if (id == MISSILE_FF2)
  {
    intelligence = 0;
    maxthrust = 0.85 * missilethrustbase;
    nimbility = 3.0;
    manoeverability = 2.0;
    ttl = 320 * timestep;
    impact = 50;
  }
  else if (id == MISSILE_MINE1)
  {
    intelligence = 0;
    maxthrust = 0.1;
    if (difficulty == 1) maxthrust = 0.14;
    else if (difficulty == 2) maxthrust = 0.18;
    nimbility = 1.5;
    manoeverability = 1.0;
    ai = true;
    ttl = -1;
    impact = 500;
    zoom = 0.3;
  }
  if (id >= MISSILE1 && id <= MISSILE2)
  {
    o->cube.set (zoom, zoom, zoom);
  }

  if (id >= STATIC_PASSIVE)
  {
    intelligence = 0;
    maxthrust = 0;
    nimbility = 0;
    manoeverability = 0;
    impact = 5;
    maxrot.theta = 0;
    maxrot.gamma = 0;
  }
  if (id == STATIC_TENT1)
  {
    shield = maxshield = 80;
    zoom = 0.5;
    o->cube.set (zoom * 0.9, zoom, zoom * 0.9);
  }
  if (id == STATIC_TENT4)
  {
    shield = maxshield = 160;
    zoom = 1.2;
    o->cube.set (zoom * 0.7, zoom * 0.42, zoom * 0.7);
  }
  if (id == STATIC_CONTAINER1)
  {
    shield = maxshield = 30;
    zoom = 1.0;
    impact = 20;
    o->cube.set (zoom * 0.4, zoom * 0.35, zoom * 0.9);
  }
  if (id == STATIC_HALL1)
  {
    shield = maxshield = 450;
    zoom = 1.8;
    impact = 20;
    o->cube.set (zoom * 0.45, zoom * 0.42, zoom);
  }
  if (id == STATIC_HALL2)
  {
    shield = maxshield = 900;
    zoom = 2.5;
    impact = 20;
    o->cube.set (zoom, zoom * 0.45, zoom);
  }
  if (id == STATIC_OILRIG1)
  {
    shield = maxshield = 1400;
    zoom = 3.5;
    impact = 20;
    o->cube.set (zoom * 0.95, zoom * 0.5, zoom * 0.95);
  }
  if (id == STATIC_COMPLEX1)
  {
    shield = maxshield = 5000;
    zoom = 2.0;
    impact = 20;
    o->cube.set (zoom * 0.75, zoom * 0.6, zoom * 0.75);
  }
  if (id == STATIC_RADAR1)
  {
    shield = maxshield = 500;
    zoom = 1.3;
    impact = 20;
    o->cube.set (zoom * 0.5, zoom * 0.7, zoom * 0.5);
  }
  if (id == ASTEROID)
  {
    shield = maxshield = 100000;
    zoom = 0.01 * math.random (60) + 1.0;
    impact = 5;
    thrust = 0.25;
    maxthrust = 0.25;
    force.z = 0.12;
    ai = false;
    o->cube.set (zoom * 0.7, zoom * 0.7, zoom * 0.7);
  }
  if (id == STATIC_BASE1)
  {
    shield = maxshield = 5500;
    zoom = 4.0;
    impact = 20;
    o->cube.set (zoom * 0.7, zoom * 0.5, zoom * 0.7);
  }
  if (id == STATIC_DEPOT1)
  {
    shield = maxshield = 3000;
    zoom = 1.5;
    impact = 20;
    o->cube.set (zoom, zoom * 0.5, zoom);
  }
  if (id == STATIC_BARRIER1)
  {
    shield = maxshield = 1000;
    zoom = 12.0;
    impact = 2000;
    o->cube.set (0.8, 11, 11);
  }
  if (id >= STATIC_PASSIVE)
  {
  }

  if (difficulty == 0) // easy
  {
    intelligence = 400 - (400 - intelligence) * 1 / 3;
    precision = 400 - (400 - precision) * 1 / 3;
    aggressivity = 400 - (400 - aggressivity) * 1 / 3;
    if (party != 1 && shield > 10) // not player party
    {
      shield = shield * 8 / 10;
      maxshield = shield;
    }
  }
  else if (difficulty == 1) // normal
  {
    intelligence = 400 - (400 - intelligence) * 2 / 3;
    precision = 400 - (400 - precision) * 2 / 3;
    aggressivity = 400 - (400 - aggressivity) * 2 / 3;
  }
  else if (difficulty == 2) // hard
  {
  }

  trafo.scaling.set (zoom, zoom, zoom);
  this->intelligence = intelligence;
  this->precision = precision;
  this->aggressivity = aggressivity;
  missileCount ();
}

void AIObj::newinit (int id, int party, int intelligence)
{
  newinit (id, party, intelligence, intelligence, intelligence);
}

void AIObj::initValues (DynamicObj *dobj, float phi)
{
  float fac = trafo.scaling.x / 8;
  if (dobj->id == FLARE1 || dobj->id == CHAFF1) fac = -fac;
  // use the exact polar coordinates because of gamma and theta
  float cgamma = currot.gamma;
  dobj->trafo.translation.x = trafo.translation.x + COS(cgamma) * SIN(phi) * fac;
  dobj->trafo.translation.y = trafo.translation.y - SIN(cgamma) * fac;
  if ((id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
    dobj->trafo.translation.y += fac;
  dobj->trafo.translation.z = trafo.translation.z + COS(cgamma) * COS(phi) * fac;
  dobj->currot.phi = phi;
  dobj->recrot.theta = dobj->currot.theta;
  dobj->force.set (force);
  dobj->trafo.rotation.set ((short) (90 + dobj->currot.gamma - 180), (short) dobj->currot.theta + 180, (short) -dobj->currot.phi);
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
      // exact calculation to hit enemy (non-static turret!)
      if (id >= FIGHTER1 && id <= FIGHTER2)
        laser->currot.gamma = currot.gamma;
      else
        laser->currot.gamma = 180.0 + atan ((target->trafo.translation.y - trafo.translation.y) / distance (target)) * 180.0 / PI;
    }
  }
  else
    laser->currot.gamma = currot.gamma; // + 90.0;
  laser->party = party;
  laser->ttl = 80 * timestep;
  laser->shield = 1;
  laser->immunity = (int) (trafo.scaling.x * 12) * timestep;
  laser->source = this;
  laser->currot.phi = phi;
  laser->currot.theta = currot.theta;
  initValues (laser, phi);
  float fac = 0.7F;
  laser->force.x += COS(laser->currot.gamma) * SIN(laser->currot.phi) * fac;
  laser->force.y -= SIN(laser->currot.gamma) * fac;
  laser->force.z += COS(laser->currot.gamma) * COS(laser->currot.phi) * fac;
  laser->activate ();
  firecannonttl += 45;
  if (day)
  {
    if (dualshot)
      laser->o = &model_cannon1b;
    else
      laser->o = &model_cannon1;
  }
  else
  {
    if (dualshot)
      laser->o = &model_cannon2b;
    else
      laser->o = &model_cannon2;
  }
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
  if (i < maxlaser)
  {
    fireCannon (laser [i], phi);
  }
}

void AIObj::fireCannon (DynamicObj **laser)
{
  if (firecannonttl > 0) return;
  if (ammo == 0) return;
  fireCannon (laser, currot.phi);
}

void AIObj::fireMissile2 (int id, AIObj *missile, AIObj *target)
{
  char buf [4096];
  if (logging.loglevel == LOG_ALL)
  {
    sprintf (buf, "Missile: party=%d, id=%d", party, id);
    logging.display (buf, LOG_ALL);
  }
  ttf = 50 * timestep;
  missile->dinit ();
  missile->aiinit ();
  missile->newinit (id, party, 0);
  initValues (missile, currot.phi);
  missile->id = id;
  missile->explode = 0;
  missile->thrust = thrust + 0.001;
  missile->recthrust = missile->maxthrust;
  missile->currot.gamma = currot.gamma;
  missile->target = target;
  missile->recrot.gamma = currot.gamma;
  missile->shield = 1;
  missile->party = party;
  missile->immunity = (45 + (int) (trafo.scaling.x * 6.0)) * timestep;
  missile->dtheta = 0;
  missile->dgamma = 0;
  missile->source = this;
  missile->activate ();
  if (id >= FIGHTER1 && id <= FIGHTER2)
  {
    missile->manoeverheight = 30 * timestep;
    missile->recheight = missile->trafo.translation.y - l->getHeight (missile->trafo.translation.x, missile->trafo.translation.z) - 4;
  }
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
  char buf [4096];
  id -= MISSILE1;
  if (id < 0 || id >= missiletypes)
  {
    sprintf (buf, "Wrong missile ID in %s, line %d", __FILE__, __LINE__);
    logging.display (buf, LOG_ERROR);
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
  char buf [4096];
  int i;
  id -= MISSILE1;
  if (id < 0 || id >= missiletypes)
  {
    sprintf (buf, "Wrong missile ID in %s, line %d", __FILE__, __LINE__);
    logging.display (buf, LOG_ERROR);
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
    ref [ptrrack * 3 + 2 - missilerackn [ptrrack]].trafo.scaling.set (0, 0, 0);
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
  if (target)
    if (distance (target) > 400)
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
    if (distance (target) > 400)
      target = NULL;
}

void AIObj::targetLockingEnemy (AIObj **f)
{
  int i;
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
  } while ((!f [i]->active || distance (f [i]) > 400) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

void AIObj::targetNextEnemy (AIObj **f)
{
  int i;
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
  } while ((!f [i]->active || distance (f [i]) > 400 || party == f [i]->party) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

void AIObj::targetPrevious (AIObj **f)
{
  int i;
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
  } while ((!f [i]->active || distance (f [i]) > 400) && z <= 1);
  target = f [i];
  if (z > 1 && !ai) target = NULL;
}

void AIObj::setSmoke (Uint32 dt)
{
  float sz = COS(currot.gamma) * COS(currot.phi) * trafo.scaling.z * 1.1; // polar (spherical) coordinates
  float sy = -SIN(currot.gamma) * trafo.scaling.y * 1.1;
  float sx = COS(currot.gamma) * SIN(currot.phi) * trafo.scaling.x * 1.1;
  
  // some smoke elements per discrete movement
  float fg = sqrt (force.x * force.x + force.y * force.y + force.z * force.z) * 13;
  if (fg >= MAXSMOKEELEM) fg = (float) MAXSMOKEELEM - 0.5;
  for (int i = 0; i < (int) fg; i ++)
  {
    float fac = (float) i / fg;
    smoke->setSmoke (trafo.translation.x - sx - force.x * fac, trafo.translation.y - sy - force.y * fac, trafo.translation.z - sz - force.z * fac, 39 - i);
  }
/*    smoke->setSmoke (trafo.translation.x - sx - force.x * 0.6, trafo.translation.y - sy - force.y * 0.6, trafo.translation.z - sz - force.z * 0.6, (int) phi, 36);
  smoke->setSmoke (trafo.translation.x - sx - force.x * 0.4, trafo.translation.y - sy - force.y * 0.4, trafo.translation.z - sz - force.z * 0.4, (int) phi, 37);
  smoke->setSmoke (trafo.translation.x - sx - force.x * 0.2, trafo.translation.y - sy - force.y * 0.2, trafo.translation.z - sz - force.z * 0.2, (int) phi, 38);
  smoke->setSmoke (trafo.translation.x - sx, trafo.translation.y - sy, trafo.translation.z - sz, (int) phi, 39);
  smoke->move (dt, 5);*/
  smoke->move (dt, (int) fg + 1);
  smokettl += timestep;
}

void AIObj::checkTtl (Uint32 dt)
{
  if (firecannonttl > 0) firecannonttl -= dt; // time to fire the next missile
  if (firemissilettl > 0) firemissilettl -= dt; // time to fire the next missile
  if (fireflarettl > 0) fireflarettl -= dt; // time to fire the next flare
  if (firechaffttl > 0) firechaffttl -= dt; // time to fire the next chaff
  if (smokettl > 0) smokettl -= dt; // time to fire the next chaff
}

void AIObj::selectNewTarget (AIObj **f)
{
  if (bomber)
    targetNearestGroundEnemy (f);
  else
    targetNearestEnemy (f);
}

void AIObj::selectTarget (AIObj **f)
{
  if (target == NULL)
  {
    selectNewTarget (f);
  }
  if (target != NULL)
  {
    if (!target->active)
      selectNewTarget (f);
  }
}

float AIObj::getMinimumHeight ()
{
  // precalculated height
  int lsdist = 15;
  float flyx = trafo.translation.x + force.x * lsdist, flyz = trafo.translation.z + force.z * lsdist;
  float flyx2 = trafo.translation.x + force.x * lsdist * 3, flyz2 = trafo.translation.z + force.z * lsdist * 3;
  float flyx3 = trafo.translation.x + force.x * lsdist * 8, flyz3 = trafo.translation.z + force.z * lsdist * 8;
  float h1 = l->getMaxHeight (flyx, flyz);
  float h2 = l->getMaxHeight (flyx2, flyz2);
  float h3 = l->getMaxHeight (flyx3, flyz3);
  h1 = h1 > h2 ? h1 : h2;
  h1 = h1 > h3 ? h1 : h3;
  return h1;
}

void AIObj::easyPiloting (Uint32 dt)
{
  float timefac = (float) dt / (float) timestep;
  float deltatheta;
  deltatheta = recrot.theta - currot.theta;
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
    if (deltatheta > estimatedtheta) dtheta += mynimbility * timefac;
    else if (deltatheta < estimatedtheta) dtheta -= mynimbility * timefac;
  }
  else
  {
    float estimatedtheta = -dtheta * (dtheta - nimbility2 * 5 / timefac) / 2 / nimbility2;
    if (deltatheta < estimatedtheta) dtheta -= mynimbility * timefac;
    else if (deltatheta > estimatedtheta) dtheta += mynimbility * timefac;
  }
  if (dtheta > (nimbility * (1.0 + realspeed)) * timefac * 5.0F)
    dtheta = (nimbility * (1.0 + realspeed)) * timefac * 5.0F;
  currot.theta += dtheta;

  assert (easymodel == 1);
  // height changes
  float nimbility1 = nimbility / 5;
  if (nimbility1 >= -0.00001 && nimbility1 <= 0.00001)
    nimbility1 = 0.00001;
  if (currot.theta > maxrot.theta) currot.theta = maxrot.theta; // restrict roll angle
  else if (currot.theta < -maxrot.theta) currot.theta = -maxrot.theta;

  float deltagamma = recrot.gamma - currot.gamma;
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
  if (dgamma > manoeverability * (3.33 + 15.0 * realspeed) * timefac)
    dgamma = manoeverability * (3.33 + 15.0 * realspeed) * timefac;
  currot.gamma += dgamma;
}

void AIObj::limitRotation ()
{
  if (currot.gamma > 180 + maxrot.gamma) currot.gamma = 180 + maxrot.gamma;
  else if (currot.gamma < 180 - maxrot.gamma) currot.gamma = 180 - maxrot.gamma;
}

void AIObj::estimateTargetPosition (float *dx2, float *dz2)
{
  float ex, ez;
  float t = 10.0 * disttarget; // generous time to new position
  if (t > 60) t = 60; // higher values will not make sense
  t *= (float) (400 - precision) / 400;
  int tt = (int) target->currot.theta;
  if (tt < 0) tt += 360;
  float newphi = t * SIN(tt) * 5.0 * target->manoeverability; // new angle of target after time t
  if (newphi > 90) newphi = 90;
  else if (newphi < -90) newphi = -90;
  newphi += (float) target->currot.phi;
  if (newphi >= 360) newphi -= 360;
  if (newphi < 0) newphi += 360;
  if ((id >= FIGHTER1 && id <= FIGHTER2) || (id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
  {
    ex = target->trafo.translation.x - SIN(newphi) * t * target->realspeed * 0.25; // estimated target position x
    ez = target->trafo.translation.z - COS(newphi) * t * target->realspeed * 0.25; // estimated target position z
  }
  else
  {
    ex = target->trafo.translation.x - SIN(newphi) * t * target->realspeed * 0.05; // estimated target position x
    ez = target->trafo.translation.z - COS(newphi) * t * target->realspeed * 0.05; // estimated target position z
  }
  *dx2 = ex - trafo.translation.x; *dz2 = ez - trafo.translation.z; // estimated distances
}

void AIObj::estimateHeading (float dx2, float dz2)
{
  float a, w = currot.phi;
  if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;

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
}

void AIObj::estimateFighterHeading (AIObj *fi)
{
  disttarget = distance (fi); // distance to target
  float ex = fi->trafo.translation.x; // estimated target position x
  float ez = fi->trafo.translation.z; // estimated target position z
  float dx2 = ex - trafo.translation.x;
  float dz2 = ez - trafo.translation.z; // estimated distances
  int w = (int) currot.phi;
  if (dz2 > -0.0001 && dz2 < 0.0001) dz2 = 0.0001;
  int a = (atan (dx2 / dz2) * 180 / PI);
  if (dz2 > 0)
  {
    if (dx2 > 0) a -= 180;
    else a += 180;
  }
  aw = a - w;
  if (aw < -180) aw += 360;
  if (aw > 180) aw -= 360;
}

void AIObj::decreaseManoeverCounter (Uint32 dt)
{
  if (manoeverheight > 0) manoeverheight -= dt;
  if (manoevertheta > 0) manoevertheta -= dt;
  if (manoeverthrust > 0) manoeverthrust -= dt;
}

int AIObj::getFireRate ()
{
  if (difficulty == 0) return 12;
  else if (difficulty == 1) return 6;
  else return 3;
}

// core AI method
void AIObj::aiAction (Uint32 dt, AIObj **f, AIObj **m, DynamicObj **c, DynamicObj **flare, DynamicObj **chaff, float camphi, float camgamma)
{
  timer += dt;

  if (!active && !draw) // not active, not drawn, then exit
  {
    return;
  }

  checkTtl (dt);

  // move object according to our physics
  move (dt, camphi, camgamma);

  if (id >= STATIC_PASSIVE) // no AI for static ground objects (buildings)
    return;

  // set smoke
  if ((id >= MISSILE1 && id < MISSILE_MINE1) || (id >= FIGHTER1 && id <= FIGHTER2)) // missile or fighter
  {
    setSmoke (dt);
  }

  if (!active) // not active, then exit
  {
    return;
  }
  if (explode > 0 || sink > 0) // exploding or sinking, then exit
  {
    thrust = 0;
    return;
  }
  
  // do expensive calculations only once
  float myheight = l->getExactHeight (trafo.translation.x, trafo.translation.z);
  float targetheight = trafo.translation.y;
  if (target != NULL)
    targetheight = l->getExactHeight (target->trafo.translation.x, target->trafo.translation.z);
  if (target != NULL)
    disttarget = distance (target); // distance to target
  else
    disttarget = 1;

  // get a new target if necessary
  if (id >= MISSILE1 && id <= MISSILE2)
  {
    if (target == NULL)
      ttl = 0;
    else if (!target->active)
      ttl = 0;
  }

  selectTarget (f);

  if (!ai) return;
 
  // which height???
  float recheight2; // this is the height, the object wants to achieve
  int lsdist = 15;
  float flyx = trafo.translation.x + force.x * lsdist, flyz = trafo.translation.z + force.z * lsdist;
  if (manoeverheight > 0)
  {
    // precalculated height
    recheight2 = l->getExactHeight (flyx, flyz) + recheight;
  }
  else
  {
     // missiles and non intelligent objects will not change their height due to the surface
    if ((id >= MISSILE1 && id <= MISSILE2 && target != NULL) ||
        (trafo.translation.y - myheight > 8 && target != NULL && trafo.translation.y - myheight < 50/* && !manoeverheight*/))
    {
      recheight2 = target->trafo.translation.y - 8 * target->thrust * SIN(target->currot.gamma);
    }
    else
    {
      recheight2 = recheight + getMinimumHeight ();
    }
  }

  // FIGHTERs only: manoevers (may use the height information)
//  if (!performManoevers (myheight))
//    return;

  decreaseManoeverCounter (dt);

  // calculate the recommended height, recheight2 depends on it
  if (manoeverheight <= 0)
  {
    if (!(id >= FIGHTER1 && id <= FIGHTER2) && target != NULL) // no fighter, has target (missile, mine)
    {
      recheight = target->trafo.translation.y - targetheight;
    }
    else if (id == FIGHTER_TRANSPORT || id == FIGHTER_TRANSPORT2) // transporters have to stay higher
    {
      recheight = 40; manoeverheight = 1;
    }
    else if (id >= FIGHTER1 && id <= FIGHTER2 && target != NULL) // fighter, has target
    {
      if (target->id >= FIGHTER1 && target->id <= FIGHTER2)
        recheight = target->trafo.translation.y - targetheight;  // target is a fighter
      else
        recheight = target->trafo.translation.y - targetheight + 5; // target is no fighter

      int flyxs = GETCOORD((int) (trafo.translation.x + force.x * 15)), flyzs = GETCOORD((int) (trafo.translation.z + force.z * 15));
      if (!l->isWater (l->f [flyxs] [flyzs])) // not flying above water
      {
        if (recheight < 3.5 + 0.01 * aggressivity)
          recheight = 3.5 + 0.01 * aggressivity;
      }

      float minh = 5.5 + 0.01 * aggressivity; // minimum height
      if (l->type == LAND_CANYON) minh = 6.5 + 0.01 * aggressivity; // stay higher in canyons
      if (fabs (trafo.translation.y - myheight) < minh)
      {
        recheight = 9 + 0.015 * aggressivity;
        if (fabs (trafo.translation.y - myheight) < minh * 0.3)
        {
          manoeverstate = 10;
          logging.display ("Manoever: Vertical climb", LOG_ALL);
        }
        else
        {
          manoeverheight = 5 * timestep; // fly manoever to gain height
        }
      }

      if (disttarget < 50 && fabs (trafo.translation.y - myheight) > 25)
      {
        recheight = 8 + 0.025 * aggressivity;
        manoeverheight = 12 * timestep;
      }
    }
  }
  
  // get recommended elevation to target
  if (ttl <= 0 && id >= MISSILE1 && id <= MISSILE2 && id != MISSILE_MINE1)
  { recheight = -100; recheight2 = -100; recrot.gamma = 90; }
  else if (ai)
  {
    if (target != NULL && ((id >= MISSILE1 && id <= MISSILE2) || (id >= FIGHTER1 && id <= FIGHTER2 && manoeverheight <= 0))) // is AGM
    {
      float dgamma = 0;
      if (disttarget <= -0.00001 || disttarget >= 0.00001) // no division by zero
        dgamma = atan ((target->trafo.translation.y - trafo.translation.y) / disttarget) * 180 / PI - (currot.gamma - 180);
      recrot.gamma = currot.gamma + dgamma;
    }
    else
    {
      recrot.gamma = (int) ((recheight2 - trafo.translation.y) * 10 - currot.gamma + 360);
    }
  }





  // do a smooth roll
  if (easymodel == 1)
  {
    easyPiloting (dt);
  }

  limitRotation ();

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

  float dx2, dz2;
  float dx = target->trafo.translation.x - trafo.translation.x, dz = target->trafo.translation.z - trafo.translation.z; // current distances
  if ((id >= FIGHTER1 && id <= FIGHTER2) || (id >= MISSILE1 && id <= MISSILE2) || (id >= FLAK1 && id <= FLAK2) || (id >= TANK1 && id <= TANK2))
  {
    estimateTargetPosition (&dx2, &dz2);
  }
  else
  {
    dx2 = dx; dz2 = dz;
  }

  estimateHeading (dx2, dz2);

  // heading calculations
  int firerate = getFireRate ();

  if (id >= MISSILE1 && id <= MISSILE2) // for missiles do the following
  {
    if (fabs (aw) < 50 && disttarget > 50) // target in front and minimum distance, then no roll
      recrot.theta = 0;
    else // otherwise chase target
    {
      if (aw < -90 || aw > 90) recrot.theta = 0;
      else if (aw > 0)
      {
        recrot.theta = aw > 90 ? 90 : aw;
      }
      else
      {
        recrot.theta = aw < -90 ? -90 : aw;
      }
    }
  }
  else if (id >= FLAK1 && id <= FLAK2) // ground-air-cannon
  {
    recthrust = 0; thrust = 0;
    if (aw > 5)
    {
      recrot.theta = maxrot.theta;
    }
    else if (aw < -5)
    {
      recrot.theta = -maxrot.theta;
    }
    else
    {
      recrot.theta = 0;
    }
  }
  else if (id >= TANK1 && id <= TANK2) // tanks
  {
    recthrust = maxthrust; thrust = maxthrust; // always at maximum thrust
    if (aw > 5)
    {
      recrot.theta = maxrot.theta;
    }
    else if (aw < -5)
    {
      recrot.theta = -maxrot.theta;
    }
    else
    {
      recrot.theta = 0;
    }
    if (firecannonttl <= 0)
    {
      if (id == TANK_AIR1)
        if (fabs (recrot.theta - currot.theta) < 2 && fabs (aw) < 20 && disttarget < 40 && target->trafo.translation.y > trafo.translation.y + 2)
        {
          fireCannon (c);
          firecannonttl += firerate * timestep;
        }
      if (id == TANK_GROUND1)
        if (fabs (recrot.theta - currot.theta) < 2 && fabs (aw) < 20 && disttarget < 35 && target->trafo.translation.y <= trafo.translation.y + 1 && target->trafo.translation.y >= trafo.translation.y - 1)
        {
          fireCannon (c);
          firecannonttl += firerate * timestep;
        }
    }
  }

  // thrust and manoever calculations
  if ((id >= FLAK1 && id <= FLAK2) || id == SHIP_CRUISER || id == SHIP_DESTROYER1 || id == TANK_TRSAM1)
  {
    if (firecannonttl <= 0)
      for (int i = 0; i < maxfighter; i ++)
        if (f [i]->active && party != f [i]->party)
        {
          estimateFighterHeading (f [i]);

          if (id == FLAK_AIR1)
            if (f [i]->trafo.translation.y > trafo.translation.y + 2)
            {
              if (fabs (aw) <= 20 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              firecannonttl = firerate * timestep;
            }
          if (id == SHIP_DESTROYER1)
            if (f [i]->trafo.translation.y > trafo.translation.y + 2)
            {
              if (aw >= 0 && aw < 40 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              if (aw >= -40 && aw < 0 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              if (aw >= 120 && aw < 160 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              if (aw >= -160 && aw < -120 && disttarget < 50) // + aggressive
                fireCannon (c, currot.phi + aw);
              firecannonttl = firerate * timestep;
            }
          if (firemissilettl <= 0)
            if (id == FLARAK_AIR1)
              if (fabs (aw) < 25 && disttarget < 45) // + aggressive
                if (f [i]->trafo.translation.y > trafo.translation.y + 2)
                {
                  ttf = 0;
                  fireMissileAirFF (m, f [i]);
                  firemissilettl += (20 + firerate * 10) * timestep;
                }
          if (id == TANK_TRSAM1)
          {
            if (firemissilettl <= 0)
              if (aw >= -30 && aw < 30 && disttarget < 60) // + aggressive
              {
                ttf = 0;
                fireMissileAirFF (m, f [i]);
                firemissilettl += aggressivity / 5 * timestep;
                missiles [6] ++; // unlimited ammo
              }
          }
          if (id == SHIP_CRUISER)
          {
            if (firemissilettl <= 0)
              if (aw >= -30 && aw < 30 && disttarget < 60) // + aggressive
              {
                ttf = 0;
                fireMissileAirFF (m, f [i]);
                firemissilettl += aggressivity / 5 * timestep;
                missiles [6] ++; // unlimited ammo
              }
            if (firecannonttl <= 0)
            {
              fireCannon (c, currot.phi + aw);
            }
          }
        }
  }
}

#endif
