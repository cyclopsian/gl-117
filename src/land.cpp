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

/* This file includes a random landscape generator. */

#ifndef IS_LAND_H

#include "land.h"

/*#define CONV_EASY 1
#undef CONV_EASY

float lx = 5.0, ly = 5.0;
float u [100], v [100];
float W [100] [100];
float X [MAXX+1] [MAXX+1];
float Y [MAXX+1] [MAXX+1];

int getCoord (int x)
{
  if (x >= 0 && x <= MAXX) return x;
  if (x < -10000 || x > 10000) return 0;
  while (x < 0) x += MAXX;
  while (x > MAXX) x -= MAXX;
  return x;
}

void convolve ()
{
#ifdef CONV_EASY
  int i, j, x, y, mx, my;
  float summe;
  for (x = 0; x <= MAXX; x ++)
  {
    for (y = 0; y <= MAXX; y ++)
    {
      summe = 0;
      for (i = 20; i < 80; i ++)
        for (j = 20; j < 80; j ++)
        {
          mx = getCoord (x - 50 + i);
          my = getCoord (y - 50 + j);
          summe += X [mx] [my] * W [i] [j];
        }
      Y [x] [y] = summe / 20;
    }
  }
#else
  int i, x, y, mx, my;
  float sum;
  float summe [MAXX];
  for (y = 0; y <= MAXX; y ++)
    summe [y] = 0;
  for (x = 0; x <= MAXX; x ++)
  {
    for (y = 0; y <= MAXX; y ++) // Faltung1
    {
      summe [y] = 0;
      for (i = 0; i < 100; i ++)
      {
        mx = getCoord (x - 50 + i);
        my = y;
        summe [y] += X [mx] [my] * u [i];
      }
    }
    for (y = 0; y <= MAXX; y ++) // Faltung2
    {
      sum = 0;
      for (i = 0; i < 100; i ++)
      {
        my = getCoord (y - 50 + i);
        sum += summe [my] * v [i];
      }
      Y [x] [y] = sum / 20;
    }
  }
#endif
}

void uv_rough ()
{
  int i;
#ifdef CONV_EASY
  int j;
  for (i = 0; i < 100; i ++)
    for (j = 0; j < 100; j ++)
    {
      float x = (float) i - 50.0;
      float y = (float) j - 50.0;
      W [i] [j] = sqrt (8.0/lx/ly) * exp (-2.0*((x*x/lx/lx)+(y*y/ly/ly)));
    }
#else
  for (i = 0; i < 100; i ++)
  {
    float x = (float) i - 50.0;
    float y = (float) i - 50.0;
    u [i] = sqrt (8.0/lx) * exp (-2.0*x*x/lx/lx);
    v [i] = sqrt (1.0/ly) * exp (-2.0*y*y/ly/ly);
  }
#endif
}*/



int Landscape::random (int n)
{
  if (n == 0) return 0;
  return ((int) (rand () % n));
}

int Landscape::extremerandom (int n)
{
  if (n == 0) return 0;
  int ret = rand () % n;
  if ((ret % 5) <= 4)
  {
    if (ret > n/2 && ret < 3*n/4) return ret + n/4;
    else if (ret < n/2 && ret > n/4) return ret - n/4;
  }
  return ret;
}

void Landscape::smoothGlacier ()
{
  int i, i2;
  for (i = 0; i <= MAXX - 1; i ++)
    for (i2 = 0; i2 <= MAXX - 1; i2 ++)
    {
      if (f[i][i2] == GLACIER)
      {
        int min = h[i][i2];
        min = min<h[i+1][i2]?min:h[i+1][i2];
        min = min<h[i][i2+1]?min:h[i][i2+1];
        min = min<h[i+1][i2+1]?min:h[i+1][i2+1];
        h[i][i2] = min;
      }
    }
}

void Landscape::gauss ()
{
  int i, i2, i3, i4;
  int g5[5][5]={{1,1,2,1,1},
                {1,3,4,3,1},
                {2,4,5,4,2},
                {1,3,4,3,1},
                {1,1,2,1,1}};
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
    {
      int sum = 0;
      for (i3 = 0; i3 < 5; i3 ++)
        for (i4 = 0; i4 < 5; i4 ++)
        {
          sum += g5[i3][i4] * h[getX(i+i3-2)][getX(i2+i4-2)];
        }
      sum /= 53;
      hg[i][i2]=sum;
    }
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
    {
      h [i] [i2] = hg [i] [i2];
    }
}

void Landscape::gaussAlpine ()
{
  int i, i2, i3, i4;

  int g5[5][5]={{1,1,2,1,1},
                {1,3,4,3,1},
                {2,4,5,4,2},
                {1,3,4,3,1},
                {1,1,2,1,1}};
  int g3[3][3]={{1,2,1},
                {2,4,2},
                {1,2,1}};
//    int hg [MAXX + 1] [MAXX + 1];
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
    {
      int sum = 0;
      if (h[i][i2] < 35000)
      {
        for (i3 = 0; i3 < 5; i3 ++)
          for (i4 = 0; i4 < 5; i4 ++)
          {
            sum += g5[i3][i4] * h[getX(i+i3-2)][getX(i2+i4-2)];
          }
        sum /= 53;
      }
      else
      {
        for (i3 = 0; i3 < 3; i3 ++)
          for (i4 = 0; i4 < 3; i4 ++)
          {
            sum += g3[i3][i4] * h[getX(i+i3-1)][getX(i2+i4-1)];
          }
        sum /= 16;
      }
      hg[i][i2]=sum;
    }
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
    {
      h [i] [i2] = hg [i] [i2];
    }
}

void Landscape::gaussDesert ()
{
  int i, i2, i3, i4;

  int g5[5][5]={{1,1,2,1,1},
                {1,3,4,3,1},
                {2,4,5,4,2},
                {1,3,4,3,1},
                {1,1,2,1,1}};
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
    {
      int sum = 0;
      if (h[i][i2] < 32000)
      {
        for (i3 = 0; i3 < 5; i3 ++)
          for (i4 = 0; i4 < 5; i4 ++)
          {
            sum += g5[i3][i4] * h[getX(i+i3-2)][getX(i2+i4-2)];
          }
        sum /= 53;
      }
      else
      {
        sum = h[i][i2];
      }
      hg[i][i2]=sum;
    }
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
    {
      h [i] [i2] = hg [i] [i2];
    }
}

void Landscape::gauss (int x, int y)
{
  if (x < 5) x = 5;
  else if (x > MAXX - 5) x = MAXX - 5;
  if (y < 5) y = 5;
  else if (y > MAXX - 5) y = MAXX - 5;

  int i, i2, i3, i4;
  int hmask [5] [5];

  int g5[5][5]={{1,1,2,1,1},
                {1,3,4,3,1},
                {2,4,5,4,2},
                {1,3,4,3,1},
                {1,1,2,1,1}};
  for (i = x-2; i <= x+2; i ++)
    for (i2 = y-2; i2 <= y+2; i2 ++)
    {
      int sum = 0;
      for (i3 = 0; i3 < 5; i3 ++)
        for (i4 = 0; i4 < 5; i4 ++)
        {
          sum += g5[i3][i4] * h[i+i3-2][i2+i4-2];
        }
      sum /= 53;
      hmask [i-x+2] [i2-y+2] = sum;
    }
  for (i = x-2; i <= x+2; i ++)
    for (i2 = y-2; i2 <= y+2; i2 ++)
    {
//        h [i] [i2] = hmask [i-x+2] [i2-y+2];
      h[i][i2] = hmask[2][2];
      hw[i][i2] = hmask[2][2];
    }
}

bool Landscape::isType (unsigned char type, unsigned char id)
{
  return ((type & 0xFF) == (id << 0));
}

bool Landscape::isWoods (int type)
{
  type &= 0xFF;
  if (type >= CONIFEROUSWOODS0 && type <= BUSHES3)
    return true;
  return false;
}

bool Landscape::isWater (int type)
{
  type &= 0xFF;
  if (type >= WATER && type <= XDEEPWATER)
    return true;
  return false;
}

bool Landscape::isGlacier (int type)
{
  type &= 0xFF;
  if (type == GLACIER)
    return true;
  return false;
}

void Landscape::init ()
{
  int i, i2;
  for (i = 0; i <= maxx; i ++)
    for (i2 = 0; i2 <= maxx; i2 ++)
    {
      h [i] [i2] = 0; f [i] [i2] = 0;
    }
}

/*  void interpolate (int n)
{
  double s, r, q, a, b, c, d, e, f;
  int i, i2, i3, i4;
  long htest, p1, p2, p3, p4;
  i3 = n;
  i = 0;
  while (i <= maxx)
  {
    i2 = 0;
    while (i2 < maxx - (1 << i3))
    {
      a = i2 + 1;
      b = i2 + (1 << i3) + 1;
      c = i2 + (1 << (i3 + 1)) + 1;
      d = h [(int) a - 1] [i];
      e = h [(int) b - 1] [i];
      f = h [(int) c - 1] [i];
      double aq = a * a, bq = b * b, cq = c * c, mba = 1/b - 1/a, mca = 1/c - 1/a;
      s = ((f/cq-d/aq)/mca-(e/bq-d/aq)/mba)/((1/cq-1/aq)/mca-(1/bq-1/aq)/mba);
      r = ((e/bq-d/aq)/mba)-(1/bq-1/aq)/mba*s;
      q = d/aq - r/a - s/aq;
      for (i4 = (int) a + 1; i4 <= (int) c - 1; i4 ++)
      {
        if (i4 != b)
        {
          htest = (long) (i4 * i4 * q + i4 * r + s);
          if (htest < 0) htest = 0;
          if (htest > 65535) htest = 65535;
          if (h [i4 - 1] [i] == 0)
          {
            h [i4 - 1] [i] = h [i4 - 1] [i] + (int) (htest / 2);
            if (i4 - 1 < (1 << i3) || i4 - 1 > maxx - (1 << i3))
              h [i4 - 1] [i] = h [i4 - 1] [i] * 2;
          }
          else
          {
            htest = htest / 2 + (htest - h [i4 - 1] [i] * 2) * (long) ((i4 - a) / (b - a) - 0.5);
            if (htest < 0) htest = 0;
            if (htest > 65535) htest = 65535;
            h [i4 - 1] [i] = h [i4 - 1] [i] + (int) htest;
          }
        }
      }
      i2 = i2 + (1 << i3);
    }
    i = i + (1 << i3);
  }
  i2 = 0;
  while (i2 <= maxx)
  {
    i = 0;
    while (i < maxx - (1 << i3))
    {
      a = i + 1;
      b = i + (1 << i3) + 1;
      c = i + (1 << (i3 + 1)) + 1;
      d = h [i2] [(int) a - 1];
      e = h [i2] [(int) b - 1];
      f = h [i2] [(int) c - 1];
      double aq = a * a, bq = b * b, cq = c * c, mba = 1/b - 1/a, mca = 1/c - 1/a;
      s = ((f/cq-d/aq)/mca-(e/bq-d/aq)/mba)/((1/cq-1/aq)/mca-(1/bq-1/aq)/mba);
      r = ((e/bq-d/aq)/mba)-(1/bq-1/aq)/mba*s;
      q = d/aq - r/a - s/aq;
      for (i4 = (int) a + 1; i4 <= (int) c - 1; i4 ++)
      {
        if (i4 != b)
        {
          htest = (long) (i4 * i4 * q + i4 * r + s);
          if (htest < 0) htest = 0;
          if (htest > 65535) htest = 65535;
          if (h [i2] [i4 - 1] == 0)
          {
            h [i2] [i4 - 1] = h [i2] [i4 - 1] + (int) (htest / 2);
            if (i4 - 1 < (1 << i3) || i4 - 1 > maxx - (1 << i3))
              h [i2] [i4 - 1] = h [i2] [i4 - 1] * 2;
          }
          else
          {
            htest = htest / 2 + (htest - h [i2] [i4 - 1] * 2) * (long) ((i4 - a) / (b - a) - 0.5);
            if (htest < 0) htest = 0;
            if (htest > 65535) htest = 65535;
            h [i2] [i4 - 1] = h [i2] [i4 - 1] + (int) htest;
          }
        }
      }
      i = i + (1 << i3);
    }
    i2 ++;
  }
}*/

void Landscape::genSurface (int hoehepc, int *heightmap)
{
  int i, i2, x, y;
  int htest, h1, h2, step;
  type = LAND_ALPINE;
  hoehe = hoehepc * 512;
  init ();
  step = maxx / 4;
  if (heightmap == NULL)
  {
    for (i = 0; i < 4; i ++)
      for (i2 = 0; i2 < 4; i2 ++)
        if (h [i * step] [i2 * step] == 0)
          h [i * step] [i2 * step] = 127 * 256 - 64 * hoehe / 1024 + extremerandom (64 * hoehe / 512);
    for (i = 0; i < 5; i ++)
    {
      h [maxx] [i * step] = h [0] [i * step];
      h [i * step] [maxx] = h [i * step] [0];
    }
  }
  else
  {
    step /= 2; n --;
    for (i = 0; i < 8; i ++)
      for (i2 = 0; i2 < 8; i2 ++)
        h [i * step] [i2 * step] = heightmap [i * 8 + i2];
    for (i = 0; i < 9; i ++)
    {
      h [maxx] [i * step] = h [0] [i * step];
      h [i * step] [maxx] = h [i * step] [0];
    }
  }
  for (i = 2; i < n; i ++)
  {
    step = step / 2; y = 0;
    do
    {
      x = step;
      do
      {
        h1 = h [x - step] [y]; h2 = h [x + step] [y];
        htest = ((h1 + h2) >> 1) - (128 >> i) * hoehe / 128 + random ((128 >> i) * hoehe / 64);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        x = x + 2 * step;
      }
      while (x != maxx + step);
      y = y + 2 * step;
    }
    while (y < maxx);
    for (x = step; x <= maxx - step; x += 2 * step)
      h [x] [maxx] = h [x] [0];
    x = 0;
    do
    {
      y = step;
      do
      {
        h1 = h [x] [y - step]; h2 = h [x] [y + step];
        htest = ((h1 + h2) >> 1) - (128 >> i) * hoehe / 128 + random ((128 >> i) * hoehe / 64);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        y = y + 2 * step;
      }
      while (y <= maxx);
      x = x + step;
    }
    while (x < maxx);
    for (y = step; y <= maxx - step; y += 2 * step)
      h [maxx] [y] = h [0] [y];
    hoehe = (int) (hoehe / 1.1);
  }
//    interpolate (LOG2MAXX - LOG2SPLINE);
  smoothGlacier ();
  gaussAlpine ();
/*    for (i = 0; i < maxx; i ++)
    for (i2 = 0; i2 < maxx; i2 ++)
      X [i] [i2] = 20000 + random (20000);
  uv_rough ();
  convolve ();
  for (i = 0; i < maxx; i ++)
    for (i2 = 0; i2 < maxx; i2 ++)
      h [i] [i2] = (int) Y [i] [i2];*/
  highestpoint = 0;
  lowestpoint = 65535;
  for (i = 0; i < maxx; i ++)
    for (i2 = 0; i2 < maxx; i2 ++)
    {
      if (h [i] [i2] > highestpoint) highestpoint = h [i] [i2];
      if (h [i] [i2] < lowestpoint) lowestpoint = h [i] [i2];
    }
}

void Landscape::genErosionSurface (int hoehepc, int *heightmap)
{
  int i, i2, x, y;
  int htest, h1, h2, step;
  type = LAND_ALPINE;
  hoehe = hoehepc * 512;
  init ();
  step = maxx / 4;
  if (heightmap == NULL)
  {
    for (i = 0; i < 4; i ++)
      for (i2 = 0; i2 < 4; i2 ++)
        if (h [i * step] [i2 * step] == 0)
          h [i * step] [i2 * step] = 127 * 256 - 64 * hoehe / 1024 + extremerandom (64 * hoehe / 512);
    for (i = 0; i < 5; i ++)
    {
      h [maxx] [i * step] = h [0] [i * step];
      h [i * step] [maxx] = h [i * step] [0];
    }
  }
  else
  {
    step /= 2; n --;
    for (i = 0; i < 8; i ++)
      for (i2 = 0; i2 < 8; i2 ++)
        h [i * step] [i2 * step] = heightmap [i * 8 + i2];
    for (i = 0; i < 9; i ++)
    {
      h [maxx] [i * step] = h [0] [i * step];
      h [i * step] [maxx] = h [i * step] [0];
    }
  }
  for (i = 2; i < n; i ++)
  {
    step = step / 2; y = 0;
    do
    {
      x = step;
      do
      {
        h1 = h [x - step] [y]; h2 = h [x + step] [y];
        htest = ((h1 + h2) >> 1) - (128 >> i) * hoehe / 128 + random ((128 >> i) * hoehe / 64);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        x = x + 2 * step;
      }
      while (x != maxx + step);
      y = y + 2 * step;
    }
    while (y < maxx);
    for (x = step; x <= maxx - step; x += 2 * step)
      h [x] [maxx] = h [x] [0];
    x = 0;
    do
    {
      y = step;
      do
      {
        h1 = h [x] [y - step]; h2 = h [x] [y + step];
        htest = ((h1 + h2) >> 1) - (128 >> i) * hoehe / 128 + random ((128 >> i) * hoehe / 64);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        y = y + 2 * step;
      }
      while (y <= maxx);
      x = x + step;
    }
    while (x < maxx);
    for (y = step; y <= maxx - step; y += 2 * step)
      h [maxx] [y] = h [0] [y];
    hoehe = (int) (hoehe / 1.1);
  }
//    interpolate (LOG2MAXX - LOG2SPLINE);
  smoothGlacier ();
  highestpoint = 0;
  lowestpoint = 65535;
  for (i = 0; i < maxx; i ++)
    for (i2 = 0; i2 < maxx; i2 ++)
    {
      if (h [i] [i2] > highestpoint) highestpoint = h [i] [i2];
      if (h [i] [i2] < lowestpoint) lowestpoint = h [i] [i2];
    }
  int erosion = (highestpoint - lowestpoint) / 5 + lowestpoint;
  for (i = 0; i < maxx; i ++)
    for (i2 = 0; i2 < maxx; i2 ++)
      if (h [i] [i2] < erosion)
        h [i] [i2] = erosion - 100 + random (200);
  gaussAlpine ();
}

int Landscape::getX (int x)
{
  return (x & MAXX_MASK);
/*  if (x < 0) return x + MAXX + 1;
  else if (x > MAXX) return x - MAXX - 1;
  return x;*/
}

void Landscape::genMoonSurface (int height)
{
  int i, i2, i3, x, y;
  int radius, rad, depth, depth2;
  double pi = 3.14;

  type = LAND_MOON;
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      h[i][i2] = 30000 + random (300);    

  int maxholes = random (1) + 1 ;
  for (i = 0; i < maxholes; i++)
  {
    x = random (MAXX + 1);
    y = random (MAXX + 1);
    radius = random (50) + 200;
    depth = radius * height / 2;
    for (i2 = x - radius; i2 < x+radius; i2++)
      for (i3 = y - radius; i3 < y+radius; i3++)
      {
         rad = (int) sqrt ((x-i2)*(x-i2)+(y-i3)*(y-i3));
         if (rad < radius)
         {
           if (rad < 0.67 * radius) 
           {
             depth2 = (int) (cos ((float)rad / (2*radius/3) * pi/2) * depth);
           }
           else 
           {
             depth2 = (int) (0.3 * sin ((float)(radius - rad) / (radius / 3) * pi) * depth * (-1));  
           }
           h[getX(i2)][getX(i3)] -= depth2;
         } 
      }
  }

  maxholes = random (4) + 8 ;
  for (i = 0; i < maxholes; i++)
  {
    x = random (MAXX + 1);
    y = random (MAXX + 1);
    radius = random (50) + 50;
    depth = radius * height / 2;
    for (i2 = x - radius; i2 < x+radius; i2++)
      for (i3 = y - radius; i3 < y+radius; i3++)
      {
         rad = (int) sqrt ((x-i2)*(x-i2)+(y-i3)*(y-i3));
         if (rad < radius)
         {
           if (rad < 0.67 * radius) 
           {
             depth2 = (int) (cos ((float)rad / (2*radius/3) * pi/2) * depth);
           }
           else 
           {
             depth2 = (int) (0.3 * sin ((float)(radius - rad) / (radius / 3) * pi) * depth * (-1));  
           }
           h[getX(i2)][getX(i3)] -= depth2;
         } 
      }
  
  }

  maxholes = random (10) + 20 ;
  for (i = 0; i < maxholes; i++)
  {
    x = random (MAXX + 1);
    y = random (MAXX + 1);
    radius = random (20) + 20;
    depth = radius * height / 2;
    for (i2 = x - radius; i2 < x+radius; i2++)
      for (i3 = y - radius; i3 < y+radius; i3++)
      {
         rad = (int) sqrt ((x-i2)*(x-i2)+(y-i3)*(y-i3));
         if (rad < radius)
         {
           if (rad < 0.67 * radius) 
           {
             depth2 = (int) (cos ((float)rad / (2*radius/3) * pi/2) * depth);
           }
           else 
           {
             depth2 = (int) (0.6 * sin ((float)(radius - rad) / (radius / 3) * pi) * depth * (-1));  
           }
           h[getX(i2)][getX(i3)] -= depth2;
         } 
      }  
  }

  maxholes = random (50) + 400;
  for (i = 0; i < maxholes; i++)
  {
    x = random (MAXX + 1);
    y = random (MAXX + 1);
    radius = random (10) + 5;
    depth = radius * height / 3;
    for (i2 = x - radius; i2 < x+radius; i2++)
      for (i3 = y - radius; i3 < y+radius; i3++)
      {
         rad = (int) sqrt ((x-i2)*(x-i2)+(y-i3)*(y-i3));
         if (rad < radius)
         {
           if (rad < 0.67 * radius) depth2 = (int) (cos ((float)rad / (2*radius/3) * pi/2) * depth);
           else depth2 = (int) (sin ((float)(radius - rad) / (radius / 3) * pi) * depth * (-1));  
           h[getX(i2)][getX(i3)] -= depth2;
         } 
      }
  
  }
//    erosion ();
  gauss ();
}

void Landscape::genCanyonSurface (int hoehepc)
{
  int i, i2, x, y;
  int htest, h1, h2, step;
  type = LAND_CANYON;
  hoehe = hoehepc * 512;
  init ();
  step = maxx / 16;
//    int minh = 127 * 256 - 64 * hoehe / 1024;
  int minh = 0;
  int maxh = 127 * 256 + 64 * hoehe / 1024;
    for (i = 0; i < 16; i ++)
      for (i2 = 0; i2 < 16; i2 ++)
        if (h [i * step] [i2 * step] == 0)
          if (random (2))
            h [i * step] [i2 * step] = 127 * 256 - 64 * hoehe / 1024;
          else
            h [i * step] [i2 * step] = 127 * 256 + 64 * hoehe / 1024;
    for (i = 0; i < 17; i ++)
    {
      h [maxx] [i * step] = h [0] [i * step];
      h [i * step] [maxx] = h [i * step] [0];
    }
  for (i = 4; i < n; i ++)
  {
    step = step / 2; y = 0;
    do
    {
      x = step;
      do
      {
        h1 = h [x - step] [y]; h2 = h [x + step] [y];
        htest = ((h1 + h2) >> 1) - (128 >> i) * hoehe / 128 + random ((128 >> i) * hoehe / 64);
        if (htest < minh) htest = minh;
        if (htest > maxh) htest = maxh;
        if (h [x] [y] == 0) h [x] [y] = htest;
        x = x + 2 * step;
      }
      while (x != maxx + step);
      y = y + 2 * step;
    }
    while (y < maxx);
    for (x = step; x <= maxx - step; x += 2 * step)
      h [x] [maxx] = h [x] [0];
    x = 0;
    do
    {
      y = step;
      do
      {
        h1 = h [x] [y - step]; h2 = h [x] [y + step];
        htest = ((h1 + h2) >> 1) - (128 >> i) * hoehe / 128 + random ((128 >> i) * hoehe / 64);
        if (htest < minh) htest = minh;
        if (htest > maxh) htest = maxh;
        if (h [x] [y] == 0) h [x] [y] = htest;
        y = y + 2 * step;
      }
      while (y <= maxx);
      x = x + step;
    }
    while (x < maxx);
    for (y = step; y <= maxx - step; y += 2 * step)
      h [maxx] [y] = h [0] [y];
    hoehe = (int) (hoehe / 1.1);
  }
//    interpolate (LOG2MAXX - LOG2SPLINE);
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      if (h [i] [i2] > 32836 - hoehepc * 30)
        h [i] [i2] -= h [i] [i2] % 2600;
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      f [i] [i2] = REDSTONE;
  gaussAlpine ();
  gaussAlpine ();
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      if (abs (h [i] [i2] - h [i + 1] [i2]) < 200 && abs (h [i] [i2] - h [i] [i2 + 1]) < 200)
      {
        f [i] [i2] = REDSAND;
        if (h [i] [i2] > 30000 && !random (80))
          f [i] [i2] = REDTREE0;
      }
      if (h [i] [i2] < 32836 - hoehepc * 40)
      {
        int dh = 32836 - hoehepc * 40 - h [i] [i2];
        if (dh < 1000)
          f [i] [i2] = SHALLOWWATER;
        else
          f [i] [i2] = DEEPWATER;
        hw [i] [i2] = 32836 - hoehepc * 40;
      }
    }
  highestpoint = 0;
  lowestpoint = 65535;
  for (i = 0; i < maxx; i ++)
    for (i2 = 0; i2 < maxx; i2 ++)
    {
      if (h [i] [i2] > highestpoint) highestpoint = h [i] [i2];
      if (h [i] [i2] < lowestpoint) lowestpoint = h [i] [i2];
    }
}

void Landscape::genDesertSurface (int hoehepc)
{
  int i, i2, x, y;
  int htest, h1, h2, step;
  type = LAND_DESERT;
  hoehe = hoehepc * 512;
  init ();
  step = maxx / 16;
    for (i = 0; i < 16; i ++)
      for (i2 = 0; i2 < 16; i2 ++)
        if (h [i * step] [i2 * step] == 0)
          h [i * step] [i2 * step] = 127 * 256 - 64 * hoehe / 1024 + extremerandom (64 * hoehe / 512);
    for (i = 0; i < 17; i ++)
    {
      h [maxx] [i * step] = h [0] [i * step];
      h [i * step] [maxx] = h [i * step] [0];
    }
  for (i = 4; i < n; i ++)
  {
    step = step / 2; y = 0;
    do
    {
      x = step;
      do
      {
        h1 = h [x - step] [y]; h2 = h [x + step] [y];
        htest = ((h1 + h2) >> 1) - (128 >> i) * hoehe / 128 + random ((128 >> i) * hoehe / 64);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        x = x + 2 * step;
      }
      while (x != maxx + step);
      y = y + 2 * step;
    }
    while (y < maxx);
    for (x = step; x <= maxx - step; x += 2 * step)
      h [x] [maxx] = h [x] [0];
    x = 0;
    do
    {
      y = step;
      do
      {
        h1 = h [x] [y - step]; h2 = h [x] [y + step];
        htest = ((h1 + h2) >> 1) - (128 >> i) * hoehe / 128 + random ((128 >> i) * hoehe / 64);
        if (htest < 0) htest = 0;
        if (htest > 65535) htest = 65535;
        if (h [x] [y] == 0) h [x] [y] = htest;
        y = y + 2 * step;
      }
      while (y <= maxx);
      x = x + step;
    }
    while (x < maxx);
    for (y = step; y <= maxx - step; y += 2 * step)
      h [maxx] [y] = h [0] [y];
    hoehe = (int) (hoehe / 1.5);
  }
//    interpolate (LOG2MAXX - LOG2SPLINE);
//    gauss ();
  gauss ();
  int i3;
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
    {
      const int range = 11;
      int ha [range];
      for (i3 = 0; i3 < range; i3 ++)
        ha [i3] = h [i] [getX (i2 + i3 - range / 2)];
      int hmin = 65000, hmax = 0;
      for (i3 = 0; i3 < range; i3 ++)
      {
        if (ha [i3] < hmin) hmin = ha [i3];
        if (ha [i3] > hmax) hmax = ha [i3];
      }
//        if (h [i] [i2] > 127 * 256)
//        {
      if (ha [range / 2 + 1] < ha [range / 2 - 1])
      {
        hg [i] [i2] = hmin;
      }
      else
      {
        hg [i] [i2] = hmax;
      }
//        }
    }
  gaussDesert ();
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      h [i] [i2] = hg [i] [i2];
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      f [i] [i2] = DESERTSAND;
  for (i = 0; i < MAXX; i += 4)
    for (i2 = 0; i2 < MAXX; i2 += 4)
    {
      if (!random (50))
        f [i] [i2] = CACTUS0;
    }
  highestpoint = 0;
  lowestpoint = 65535;
  for (i = 0; i < maxx; i ++)
    for (i2 = 0; i2 < maxx; i2 ++)
    {
      if (h [i] [i2] > highestpoint) highestpoint = h [i] [i2];
      if (h [i] [i2] < lowestpoint) lowestpoint = h [i] [i2];
    }
}

void Landscape::genRocks (int diffmin, int percent)
{
  int i, i2, i3, i4, i5, d = 150;
  long hmin, hmax;
  long htest;
  double r, rd;
  rockborder = 65535;
  hmax = highestpoint; hmin = lowestpoint;
  if (hmax - hmin >= (long) diffmin * 256) // Generation
    for (i = 0; i < maxx; i ++)
      for (i2 = 0; i2 < maxx; i2 ++)
      {
        r = (hmax - hmin) / 100 * percent;
        rd = h [i] [i2];
        rd = rd - h [i + 1] [i2] / 3 - h [i] [i2 + 1] / 3 - h [i + 1] [i2 + 1] / 3;
        rd = fabs (rd);
        if (h [i] [i2] > hmax - r)
        {
          if (rd >= 800 - 800 * (h [i] [i2] - r) / (hmax - r))
          {
            f [i] [i2] = ROCKS + random (2);
            htest = h [i] [i2] - d + random (d * 2);
            if (htest > 65535) htest = 65535;
            h [i] [i2] = (unsigned short) htest;
            if (h [i] [i2] < rockborder) rockborder = h [i] [i2];
          }
          else if (rd >= 500 - 500 * (h [i] [i2] - r) / (hmax - r))
          {
            f [i] [i2] = GRAVEL;
          }
        }
        if (abs (h [i] [i2] - h [i + 1] [i2]) >= 600 || abs (h [i] [i2] - h [i] [i2 + 1]) >= 600)
        {
          f [i] [i2] = ROCKS + random (2);
          htest = h [i] [i2] - d + random (d * 2);
          if (htest > 65535) htest = 65535;
          h [i] [i2] = (unsigned short) htest;
//            if (h [i] [i2] < rockborder) rockborder = h [i] [i2];
        }
        if (h [i] [i2] > hmax - r / 2)
          if (rd < 200 * (h [i] [i2] - r / 2) / (hmax - r / 2))
            f [i] [i2] = GLACIER;
      }
  for (i = 1; i < maxx; i ++) // Lücken schließen
    for (i2 = 1; i2 < maxx; i2 ++)
    {
      if (f [i] [i2] != ROCKS)
        if (f [i - 1] [i2] == ROCKS && f [i + 1] [i2] == ROCKS ||
            f [i] [i2 - 1] == ROCKS && f [i] [i2 + 1] == ROCKS)
          f [i] [i2] = ROCKS;
      if (f [i] [i2] != ROCKS)
        if (f [i - 1] [i2] == GLACIER && f [i + 1] [i2] == GLACIER ||
            f [i] [i2 - 1] == GLACIER && f [i] [i2 + 1] == GLACIER)
          f [i] [i2] = GLACIER;
    }
  for (i = 1; i < maxx; i ++) // Felsengebiet uneben
    for (i2 = 1; i2 < maxx; i2 ++)
      if (f [i] [i2] == ROCKS)
      {
        i5 = 0;
        for (i3 = -1; i3 <= 1; i3 ++)
          for (i4 = -1; i4 <= 1; i4 ++)
            if (f [i + i3] [i2 + i4] == ROCKS)
              i5 ++;
        htest = (long) h [i] [i2] + (long) i5 * 50;
        if (htest > 65535) htest = 65535;
        h [i] [i2] = (unsigned short) htest;
      }
}

/*  void Landscape::cLakeRecursion (short x, short y, unsigned short level, unsigned char sn, int extent)
{
  if (h [x] [y] < level)
  {
    ftry [x] [y] = sn; extent ++;
    if (x > 0)
      if (ftry [x-1] [y] == GRASS) cLakeRecursion (x-1, y, level, sn, extent);
    if (x < MAXX)
      if (ftry [x+1] [y] == GRASS) cLakeRecursion (x+1, y, level, sn, extent);
    if (y > 0)
      if (ftry [x] [y-1] == GRASS) cLakeRecursion (x, y-1, level, sn, extent);
    if (y < MAXX)
      if (ftry [x] [y+1] == GRASS) cLakeRecursion (x, y+1, level, sn, extent);
  }
}*/

int Landscape::calcLake (int ys, int xs, unsigned short level, int num, int maxextent)
{
  int i, i2, n, w, timeout = 0;
  bool hits;
  unsigned char sn = (unsigned char) num;
  int extent = 0;
//  h [xs] [ys] = level;
  w = 5;
  if (xs <= 0) xs = 1;
  if (ys <= 0) ys = 1;
  if (xs >= maxx) xs = maxx - 1;
  if (ys >= maxx) ys = maxx - 1;
  ftry [xs] [ys] = sn;
//    cLakeRecursion (xs, ys, level, num, extent);
  do
  {
    timeout ++;
    i2 = 0; hits = false;
    while (i2 < maxx * 2)
    {
      i2 ++;
      n = i2;
      if (n > maxx) n = n - maxx * 2 + 1;
      for (i = xs; i < maxx; i ++)
      {
        if (ys - n >= 0 && ys - n < maxx)
          if (h [i] [ys - n] < level && ftry [i] [ys - n] != sn)
            if ((ftry [i - 1] [ys - n] == sn) || (ftry [i] [ys - n + 1] == sn) ||
                (ftry [i + 1] [ys - n + 1] == sn) || (ftry [i - 1] [ys - n + 1] == sn))
            {
              ftry [i] [ys - n] = sn; extent ++; hits = true;
            }
        if (ys + n > 0 && ys + n <= maxx)
          if (h [i] [ys + n] < level && ftry [i] [ys + n] != sn)
            if ((ftry [i - 1] [ys + n] == sn) || (ftry [i] [ys + n - 1] == sn) ||
                (ftry [i + 1] [ys + n - 1] == sn) || (ftry [i - 1] [ys + n - 1] == sn))
            {
              ftry [i] [ys + n] = sn; extent ++; hits = true;
            }
      }
      for (i = xs; i > 0; i --)
      {
        if (ys - n >= 0 && ys - n < maxx)
          if (h [i] [ys - n] < level && ftry [i] [ys - n] != sn)
            if ((ftry [i + 1] [ys - n] == sn) || (ftry [i] [ys - n + 1] == sn) ||
                (ftry [i + 1] [ys - n + 1] == sn) || (ftry [i - 1] [ys - n + 1] == sn))
            {
              ftry [i] [ys - n] = sn; extent ++; hits = true;
            }
        if (ys + n > 0 && ys + n <= maxx)
          if (h [i] [ys + n] < level && ftry [i] [ys + n] != sn)
            if ((ftry [i + 1] [ys + n] == sn) || (ftry [i] [ys + n - 1] == sn) ||
                (ftry [i + 1] [ys + n - 1] == sn) || (ftry [i - 1] [ys + n - 1] == sn))
            {
              ftry [i] [ys + n] = sn; extent ++; hits = true;
            }
      }
      for (i = ys; i < maxx; i ++)
      {
        if (xs - n >= 0 && xs - n < maxx)
          if (h [xs - n] [i] < level && ftry [xs - n] [i] != sn)
            if ((ftry [xs - n] [i - 1] == sn) || (ftry [xs - n + 1] [i] == sn) ||
                (ftry [xs - n + 1] [i + 1] == sn) || (ftry [xs - n + 1] [i - 1] == sn))
            {
              ftry [xs - n] [i] = sn; extent ++; hits = true;
            }
        if (xs + n > 0 && xs + n <= maxx)
          if (h [xs + n] [i] < level && ftry [xs + n] [i] != sn)
            if ((ftry [xs + n] [i - 1] == sn) || (ftry [xs + n - 1] [i] == sn) ||
                (ftry [xs + n - 1] [i + 1] == sn) || (ftry [xs + n - 1] [i - 1] == sn))
            {
              ftry [xs + n] [i] = sn; extent ++; hits = true;
            }
      }
      for (i = ys; i > 0; i --)
      {
        if (xs - n >= 0 && xs - n < maxx)
          if (h [xs - n] [i] < level && ftry [xs - n] [i] != sn)
            if ((ftry [xs - n] [i + 1] == sn) || (ftry [xs - n + 1] [i] == sn) ||
                (ftry [xs - n + 1] [i + 1] == sn) || (ftry [xs - n + 1] [i - 1] == sn))
            {
              ftry [xs - n] [i] = sn; extent ++; hits = true;
            }
        if (xs + n > 0 && xs + n <= maxx)
          if (h [xs + n] [i] < level && ftry [xs + n] [i] != sn)
            if ((ftry [xs + n] [i - 1] == sn) || (ftry [xs + n - 1] [i] == sn) ||
                (ftry [xs + n - 1] [i + 1] == sn) || (ftry [xs + n - 1] [i - 1] == sn))
            {
              ftry [xs + n] [i] = sn; extent ++; hits = true;
            }
      }
    }
    if (extent > maxextent)
      return -1;
  }
  while (hits && timeout < 10);
  if (timeout >= 10)
    return -1;
  return extent;
}

void Landscape::genLake (int depthpc)
{
//    System.out.println ("generating lake: " + str);
  for (int j = 0; j < 5; j ++)
  {
    int a1, i, i2;
    unsigned short level, zmin;
    int xs, ys;
    unsigned short depth = (highestpoint - lowestpoint) * depthpc / 100;
    zmin = 65535;
    int radius = MAXX * 2 * depthpc / 100;
    a1 = maxx / 8;
    for (i = 0; i < 20; i ++)
    {
      xs = random (MAXX - 2 * a1) + a1;
      ys = random (MAXX - 2 * a1) + a1;
      if (hw [ys] [xs] == 0) break;
    }
    if (i == 20) return;
    int minx = xs - radius, miny = ys - radius;
    int maxx = xs + radius, maxy = ys + radius;
    if (minx < 1) minx = 1;
    if (maxx > MAXX - 1) maxx = MAXX - 1;
    if (miny < 1) miny = 1;
    if (maxy > MAXX - 1) maxy = MAXX - 1;
    xs = -1; ys = -1;
    for (i = miny; i < maxy; i += 2)
      for (i2 = minx; i2 < maxx; i2 += 2)
        if (h [i] [i2] < zmin && hw [i] [i2] == 0)
        {
          zmin = h [i] [i2];
          xs = i2; ys = i;
        }
    int extent = 0;
    if (xs >= 0)
    {
      level = zmin + depth;
  //      System.out.println ("depth = " + depth+" x = "+xs+" y = "+ys+" id = "+aktid);
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          ftry [i] [i2] = GRASS;
        }
      extent = calcLake (xs, ys, level, TRYLAKE, radius * radius / 2);
      if (extent <= 0) break;
      for (i = 0; i <= MAXX; i ++)
      {
        if (ftry [i] [0] != ftry [i] [MAXX])
        {
          if (ftry [i] [0] == TRYLAKE)
            extent += calcLake (MAXX, i, level, TRYLAKE, radius * radius / 2);
          else
            extent += calcLake (0, i, level, TRYLAKE, radius * radius / 2);
          if (extent <= 0) break;
        }
      }
      if (extent > 0)
      for (i = 0; i <= MAXX; i ++)
      {
        if (ftry [0] [i] != ftry [MAXX] [i])
        {
          if (ftry [0] [i] == TRYLAKE)
            extent += calcLake (i, MAXX, level, TRYLAKE, radius * radius / 2);
          else
            extent += calcLake (i, 0, level, TRYLAKE, radius * radius / 2);
          if (extent <= 0) break;
        }
      }
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
          if (ftry [i] [i2] == TRYLAKE)
          {
            ftry [i] [i2] = GRASS;
            if (level - h [i] [i2] < 200)
              f [i] [i2] = XSHALLOWWATER;
            else if (level - h [i] [i2] < 400)
              f [i] [i2] = SHALLOWWATER;
            else if (level - h [i] [i2] < 900)
              f [i] [i2] = WATER;
            else if (level - h [i] [i2] < 1500)
              f [i] [i2] = DEEPWATER;
            else
              f [i] [i2] = XDEEPWATER;
            hw [i] [i2] = level; // + random (w);
          }
      return;
    }
  }
}

void Landscape::calcWoods (int dy)
{
//	  printf ("rockborder=%d\n", rockborder); fflush (stdout);
  int i, i2;
  int var = 2000;
  //  long htest;
  for (i = 0; i < maxx; i ++)
    for (i2 = 0; i2 < maxx; i2 ++)
    {
      if (fabs (h [i] [i2] - h [i + 1] [i2]) > dy)
        if (f [i] [i2] == 0)
        {
          int ra = random (3);
          if (h [i] [i2] >= rockborder + 3000 + random (var)) f [i] [i2] = (DWARFPINES1 + ra);
          else if (h [i] [i2] >= 15000 + random (var) && h [i] [i2] < rockborder - 10000 + random (var)) f [i] [i2] = (MIXEDWOODS1 + ra);
          else if (h [i] [i2] < 15000 + var) f [i] [i2] = (CONIFEROUSWOODS1 + ra);
          else f [i] [i2] = (DECIDUOUSWOODS1 + ra);
  //          htest = h [i] [i2] + 200 + random (200);
  //          if (htest < 65535) h [i] [i2] = htest;
        }
  //      if (f [i] [i2] == 0 && random (2)) f [i] [i2] = CONIFEROUSWOODS1;
    }
  for (i = 1; i < maxx; i ++)
    for (i2 = 1; i2 < maxx; i2 ++)
      if (f [i] [i2] == 0)
        if (isWoods (f [i - 1] [i2]) && isWoods (f [i + 1] [i2]) ||
            isWoods (f [i] [i2 - 1]) && isWoods (f [i] [i2 + 1]))
        {
          int ra = random (3);
          if (h [i] [i2] >= rockborder + 3000 + random (var)) f [i] [i2] = (DWARFPINES1 + ra);
          else if (h [i] [i2] >= 15000 + random (var) && h [i] [i2] < rockborder - 10000 + random (var)) f [i] [i2] = (BUSHES1 + ra);
          else if (h [i] [i2] < 15000 + var) f [i] [i2] = (BUSHES1 + ra);
          else f [i] [i2] = (BUSHES1 + ra);
        }
  for (i = 4; i < maxx - 4; i ++)
    for (i2 = 4; i2 < maxx - 4; i2 ++)
      if (isWoods (f [i - 4] [i2]) && isWoods (f [i + 4] [i2]) &&
          isWoods (f [i] [i2 - 4]) && isWoods (f [i] [i2 + 4]) &&
      	  isWoods (f [i] [i2]))
      {
        f [i] [i2] --;
      }
  for (i = 1; i < maxx; i ++)
    for (i2 = 1; i2 < maxx; i2 ++)
      if (f [i] [i2] == f [i + 1] [i2] && f [i] [i2] == f [i - 1] [i2] &&
          f [i] [i2] == f [i] [i2 + 1] && f [i] [i2] == f [i] [i2 - 1])
	    {
        if ((f [i] [i2] & 4) == 1) f [i] [i2] ++;
        else if ((f [i] [i2] & 4) == 2) f [i] [i2] --;
      }
  for (i = 1; i < maxx; i += 3)
    for (i2 = 1; i2 < maxx; i2 += 3)
      if (isWater (f [i] [i2]))
      {
        if (f [i + 1] [i2] == GRASS) f [i + 1] [i2] = BUSHES1 + random (3);
        if (f [i - 1] [i2] == GRASS) f [i - 1] [i2] = BUSHES1 + random (3);
        if (f [i] [i2 + 1] == GRASS) f [i] [i2 + 1] = BUSHES1 + random (3);
        if (f [i] [i2 - 1] == GRASS) f [i] [i2 - 1] = BUSHES1 + random (3);
      }
}

bool Landscape::isGround (int x, int y)
{
  if (x < 0 || x > maxx || y < 0 || y > maxx) return false;
  int type = f [y] [x];
  if (!isWater (type))
    return true;
  return false;
}

bool Landscape::riverCheck (int x, int y, int *fl, int z, int z2)
{
  if (x <= 0 || x >= maxx || y <= 0 || y >= maxx)
    return false;
  if (isType (f [x] [y], GRASS) || isWoods (f [x] [y]))
  {
    int bad = 0;
    for (int i = z; i <= z2; i ++)
      if (abs (fl [i * 2] - x) + abs (fl [i * 2 + 1] - y) <= 1)
        bad ++;
    if (bad <= 2) return true;
  }
  return false;
}

void Landscape::genRiver ()
{
//  System.out.println ("generating river: " + name);
  bool ok, ok2;
  int lang;
  int i, i2, i3;
  int fl1 = 0, fl2 = 0, fl3 = 0, fl4 = 0, z, z2, xf, yf, xf2, yf2, h1, h2;
  int fl [400] [2];
  int abbruch;
  abbruch = 0;
  lang = 30;
  do
  {
    abbruch ++;
    do
    {
      xf = random (maxx - 19) + 10; yf = random (maxx - 19) + 10;
    }
    while (!isGround (yf, xf));
    fl [200] [0] = xf; fl [200] [1] = yf;
    xf2 = xf; yf2 = yf;
    z = 0; z2 = 0;
    do
    {
      h1 = 65535;
      ok = true;
      for (i = -1; i <= 1; i ++)
        for (i2 = -1; i2 <= 1; i2 ++)
          if (xf + i >= 0 && xf + i <= maxx &&
              yf + i2 >= 0 && yf + i2 <= maxx)
            if (riverCheck (xf + i, yf + i2, &fl [0] [0], 200 - z, 200 + z2))
              if (h [xf + i] [yf + i2] < h [xf] [yf])
                if (h [xf] [yf] - h [xf + i] [yf + i2] < h1)
                {
                  h1 = h [xf] [yf] - h [xf + i] [yf + i2];
                  fl1 = xf + i; fl2 = yf + i2;
                  ok = false;
                }
      h2 = 65535;
      ok2 = true;
      for (i = -1; i <= 1; i ++)
        for (i2 = -1; i2 <= 1; i2 ++)
          if (xf2 + i >= 0 && xf2 + i <= maxx &&
              yf2 + i2 >= 0 && yf2 + i2 <= maxx)
            if (riverCheck (xf2 + i, yf2 + i2, &fl [0] [0], 200 - z, 200 + z2))
              if (h [xf2 + i] [yf2 + i2] > h [xf2] [yf2])
                if (h [xf2 + i] [yf2 + i2] - h [xf2] [yf2] < h2)
                {
                  h2 = h [xf2 + i] [yf2 + i2] - h [xf2] [yf2];
                  fl3 = xf2 + i; fl4 = yf2 + i2;
                  ok2 = false;
                }
      if (!ok)
      {
        z ++;
        fl [200 - z] [0] = fl1; fl [200 - z] [1] = fl2;
        xf = fl1; yf = fl2;
      }
      if (!ok2)
      {
        z2 ++;
        fl [200 + z2] [0] = fl3; fl [200 + z2] [1] = fl4;
        xf2 = fl3; yf2 = fl4;
      }
    }
    while ((!ok || !ok2) && (z < 200) && (z2 < 200)); //ok and ok2 or (z >= 200) or (z2 >= 200);
  }
  while (z + z2 <= lang && abbruch <= 10); //until (z + z2 > lang) or (abbruch > 2000);
  if (abbruch <= 10)
  {
    if (z > 200) z = 200;
    if (z2 > 200) z2 = 200;
    for (i = 200 - z; i <= 200 + z2; i ++)
    {
      xf = fl [i] [0]; yf = fl [i] [1];
      f [xf] [yf] = SHALLOWWATER;
      if (isWater (f [xf] [yf]))
      {
        int hf = h [xf] [yf];
        for (i2 = -1; i2 <= 1; i2 ++)
          for (i3 = -1; i3 <= 1; i3 ++)
            if (xf + i2 >= 0 && xf + i2 <= maxx && yf + i3 >= 0 && yf + i3 < maxx)
              if (hf - h [xf + i2] [yf + i3] > 0
                  && !isWater (f [xf + i2] [yf + i3]))
              {
                hf = h [xf + i2] [yf + i3];
              }
        hf -= 200;
        if (hf < 0) hf = 0;
        if (h [xf] [yf] - hf < 350)
        {
          f [xf] [yf] = SHALLOWWATER;
        }
        hw [xf] [yf] = h [xf] [yf];
        h [xf] [yf] = hf;
      }
/*        if (xf > 0 && xf < maxx && yf > 0 && yf < maxx)
      {
        for (i2 = -1; i2 <= 1; i2 ++)
          for (i3 = -1; i3 <= 1; i3 ++)
          {
            int border = 0;
            for (int i4 = -1; i4 <= 1; i4 ++)
              for (int i5 = -1; i5 <= 1; i5 ++)
                if (isWater (f [xf + i4] [yf + i5]))
                  border ++;
            if (Math.abs (h [xf] [yf] - h [xf + i2] [yf + i3]) < 200
                && !isWater (f [xf + i2] [yf + i3]) && border >= 5)
            {
              h [xf + i2] [yf + i3] = h [xf] [yf];
              f [xf + i2] [yf + i3] = SHALLOWWATER << TYPE | (10 << 16);
              f [xf + i2] [yf + i3] |= ((aktid + 1) << 16);
            }
          }
      }*/
    }
/*      if (z + z2 >= lang + 10)
      BerechneSee (fl [200 - z] [0], fl [200 - z] [1],
                   h [fl [200 - z] [0]] [fl [200 - z] [1]] + 5, 1);*/
/*      for (i = 200 - z; i <= 200 + z2; i ++)
    {
    }*/
  }
}

void Landscape::searchPlain (int divx, int divy, int *x, int *y)
{
  int i, i2;
  if (divx == -1 || divy == -1)
  {
    int val [MAXX_8] [MAXX_8];
    for (i = 8; i <= MAXX - 8; i += 8)
      for (i2 = 8; i2 <= MAXX - 8; i2 += 8)
      {
        if ((type != LAND_CANYON && f [i] [i2] == GRASS && f [i-4] [i2] == GRASS && f [i+4] [i2] == GRASS && f [i] [i2-4] == GRASS && f [i] [i2+4] == GRASS) ||
            (type == LAND_CANYON && f [i] [i2] == REDSAND && f [i-4] [i2] == REDSAND && f [i+4] [i2] == REDSAND && f [i] [i2-4] == REDSAND && f [i] [i2+4] == REDSAND))
        {
          if (type != 2)
            val [i >> 3] [i2 >> 3] = h [i] [i2] / 4;
          else
            val [i >> 3] [i2 >> 3] = 32000 - h [i] [i2] / 4;
          val [i >> 3] [i2 >> 3] += abs (h [i] [i2] - h [i-4] [i2]);
          val [i >> 3] [i2 >> 3] += abs (h [i] [i2] - h [i+4] [i2]);
          val [i >> 3] [i2 >> 3] += abs (h [i] [i2] - h [i] [i2-4]);
          val [i >> 3] [i2 >> 3] += abs (h [i] [i2] - h [i] [i2+4]);
        }
        else
        {
          val [i >> 3] [i2 >> 3] = 100000;
        }
      }
    int min = 100000;
    for (i = 1; i < MAXX_8; i ++)
      for (i2 = 1; i2 < MAXX_8; i2 ++)
      {
        if (val [i] [i2] < min)
        {
          min = val [i] [i2];
          *x = 8 * i;
          *y = 8 * i2;
        }
      }
  }
  else if (divx < DIVISIONS && divy < DIVISIONS)
  {
    int val [DIVX_8] [DIVX_8];
    for (i = 8 + divx * DIVX; i <= (divx + 1) * DIVX - 8; i += 8)
      for (i2 = 8 + divy * DIVX; i2 <= (divy + 1) * DIVX - 8; i2 += 8)
      {
        int ix = (i - 8 - divx * DIVX) >> 3;
        int iy = (i2 - 8 - divy * DIVX) >> 3;
        if (f [i] [i2] == GRASS && f [i-4] [i2] == GRASS && f [i+4] [i2] == GRASS && f [i] [i2-4] == GRASS && f [i] [i2+4] == GRASS)
        {
          val [ix] [iy] = abs (h [i] [i2] - h [i-4] [i2]);
          val [ix] [iy] += abs (h [i] [i2] - h [i+4] [i2]);
          val [ix] [iy] += abs (h [i] [i2] - h [i] [i2-4]);
          val [ix] [iy] += abs (h [i] [i2] - h [i] [i2+4]);
          val [ix] [iy] += h [i] [i2] / 250;
        }
        else
        {
          val [ix] [iy] = 100000;
        }
      }
    int min = 100000;
    for (i = 0; i < DIVX_8 - 1; i ++)
      for (i2 = 0; i2 < DIVX_8 - 1; i2 ++)
      {
        if (val [i] [i2] < min)
        {
          min = val [i] [i2];
          *x = 8 + 8 * i + divx * DIVX;
          *y = 8 + 8 * i2 + divy * DIVX;
        }
      }
  }
}

/*void Landscape::searchGreatPlain (int *x, int *y)
{
  int i, i2;
    int val [MAXX_8 / 2] [MAXX_8 / 2];
    for (i = 16; i <= MAXX - 16; i += 16)
      for (i2 = 16; i2 <= MAXX - 16; i2 += 16)
        if (f [i] [i2] == GRASS && f [i-8] [i2] == GRASS && f [i+8] [i2] == GRASS && f [i] [i2-8] == GRASS && f [i] [i2+8] == GRASS)
        {
          val [i >> 4] [i2 >> 4] = abs (h [i] [i2] - h [i-8] [i2]);
          val [i >> 4] [i2 >> 4] += abs (h [i] [i2] - h [i+8] [i2]);
          val [i >> 4] [i2 >> 4] += abs (h [i] [i2] - h [i] [i2-8]);
          val [i >> 4] [i2 >> 4] += abs (h [i] [i2] - h [i] [i2+8]);
          val [i >> 4] [i2 >> 4] += abs (h [i] [i2] - h [i-4] [i2]);
          val [i >> 4] [i2 >> 4] += abs (h [i] [i2] - h [i+4] [i2]);
          val [i >> 4] [i2 >> 4] += abs (h [i] [i2] - h [i] [i2-4]);
          val [i >> 4] [i2 >> 4] += abs (h [i] [i2] - h [i] [i2+4]);
        }
    int min = 100000;
    for (i = 1; i < MAXX_8; i ++)
      for (i2 = 1; i2 < MAXX_8; i2 ++)
      {
        if (val [i] [i2] < min)
        {
          min = val [i] [i2];
          *x = i;
          *y = i2;
        }
      }
}*/

Landscape::Landscape ()
{
  maxx = MAXX;
  n = LOG2SPLINE;
  //n = LOG2MAXX;
  maxn = LOG2MAXX;
  hoehe = HEIGHT * 256;
  for (int i = 0; i <= MAXX; i ++)
    for (int i2 = 0; i2 <= MAXX; i2 ++)
    {
      hw [i] [i2] = 0;
      h [i] [i2] = 0;
      f [i] [i2] = GRASS;
    }
}

Landscape::~Landscape ()
{
}

#endif
