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

#ifndef IS_GLLAND_H

#include "glland.h"

float zoomz = 1.0/(100.0*MAXX);
CTexture *texgrass, *texrocks, *texwater, *textree, *textree2, *textree3, *texcactus1, *texredstone;
CTexture *textreeu, *textreeu2, *textreeu3, *textreeu4, *texcactusu1;
CTexture *textree4, *texearth, *texsand, *texredsand, *texgravel1;
CTexture *texglitter1;

void GLLandscape::norm (float *c)
{
  float n;
  n = 1.0/sqrt(c[0]*c[0]+c[1]*c[1]+c[2]*c[2]);
  c[0] *= n;
  c[1] *= n;
  c[2] *= n;
}

void GLLandscape::normalcrossproduct( float* a, float* b, float*c )
{
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = a[2]*b[0] - a[0]*b[2];
  c[2] = a[0]*b[1] - a[1]*b[0];
  norm (c);
}

// obsolete
/*float *GLLandscape::selectMaterial (int x, int y)
{
  if (f [x] [y] == GRASS) return mata [0];
  else if (f [x] [y] >= CONIFEROUSWOODS1 && f [x] [y] <= MIXEDWOODS3) return mata [1];
  else if (f [x] [y] == ROCKS) return mata [2];
  else if (f [x] [y] == GLACIER) return mata [3];
  else if (f [x] [y] >= DWARFPINES1 && f [x] [y] <= BUSHES3) return mata [1];
  else if (f [x] [y] == WATER) return mata [5];
  else if (f [x] [y] == SHALLOWWATER) return mata [4];
  else if (f [x] [y] == DEEPWATER) return mata [6];
  else if (f [x] [y] == ROCKS2) return mata [7];
  else if (f [x] [y] == XSHALLOWWATER) return mata [8];
  else if (f [x] [y] == XDEEPWATER) return mata [9];
  else if (f [x] [y] == MOONSAND) return mata [10];
  else if (f [x] [y] == REDSTONE) return mata [11];
  else if (f [x] [y] == REDSAND || f [x] [y] == REDTREE0) return mata [12];
  else if (f [x] [y] == DESERTSAND || f [x] [y] == CACTUS0) return mata [13];
  else if (f [x] [y] == GREYSAND) return mata [14];
  else if (f [x] [y] == GRAVEL) return mata [15];
  else return mat [0];
}*/

int GLLandscape::selectColor (int x, int y)
{
  if (f [x] [y] == GRASS) return 0;
  else if (f [x] [y] >= CONIFEROUSWOODS1 && f [x] [y] <= MIXEDWOODS3) return 1;
  else if (f [x] [y] == ROCKS) return 2;
  else if (f [x] [y] == GLACIER) return 3;
  else if (f [x] [y] >= DWARFPINES1 && f [x] [y] <= BUSHES3) return 1;
  else if (f [x] [y] == WATER) return 5;
  else if (f [x] [y] == SHALLOWWATER) return 4;
  else if (f [x] [y] == DEEPWATER) return 6;
  else if (f [x] [y] == ROCKS2) return 7;
  else if (f [x] [y] == XSHALLOWWATER) return 8;
  else if (f [x] [y] == XDEEPWATER) return 9;
  else if (f [x] [y] == MOONSAND) return 10;
  else if (f [x] [y] == REDSTONE) return 11;
  else if (f [x] [y] == REDSAND || f [x] [y] == REDTREE0) return 12;
  else if (f [x] [y] == DESERTSAND || f [x] [y] == CACTUS0) return 13;
  else if (f [x] [y] == GREYSAND) return 14;
  else if (f [x] [y] == GRAVEL) return 15;
  else return 0;
}

void GLLandscape::precalculate ()
{
  int i, i2, i3, i4, x, z;
  float no[3];
  float nw[3];
  float so[3];
  float sw[3];
  float a[3];
  float c[3];

  // initialize dynamic light mask
  memset (dl, 0, (MAXX + 1) * (MAXX + 1));

  norm (lv); // normalize light vector

  // generate greyish material values for night if necessary
  float matgrey [MAXMATERIAL] [4];
  if (!day)
  {
    float greyfactor = 0.75; // 0 = grey, 1 = no modification
    for (i = 0; i < MAXMATERIAL; i ++)
    {
      float mid = (mat [i] [0] + mat [i] [1] + mat [i] [2]) / 3.0F;
      matgrey [i] [0] = mid + (mat [i] [0] - mid) * greyfactor;
      matgrey [i] [1] = mid + (mat [i] [1] - mid) * greyfactor;
      matgrey [i] [2] = mid + (mat [i] [2] - mid) * greyfactor;
    }
  }

  // set the colors of the landscape
  float mzoom = zoomz;
  for (x=0; x<=MAXX; x++)
    for (z=0; z<=MAXX; z++)
    {
      int a;
      a = selectColor (x, z);
      if (type == 2 && a == 4)
      {
        a = 11;
      }
      if (day)
      {
        r [x] [z] = (unsigned char) (mat [a] [0] * 255.9);
        g [x] [z] = (unsigned char) (mat [a] [1] * 255.9);
        b [x] [z] = (unsigned char) (mat [a] [2] * 255.9);
      }
      else
      {
        r [x] [z] = (unsigned char) (matgrey [a] [0] * 255.9);
        g [x] [z] = (unsigned char) (matgrey [a] [1] * 255.9);
        b [x] [z] = (unsigned char) (matgrey [a] [2] * 255.9);
      }
//        printf ("a=%d, r=%d, g=%d, b=%d\n", a, r[x][z],g[x][z],b[x][z]); fflush (stdout);
    }

  // smooth the colors (obsolete)
  long sum;
/*  long g3[3][3]={{0,1,0},
        {1,4,1},
        {0,1,0}};*/
  /*for (i = 1; i < MAXX; i ++)
    for (i2 = 1; i2 < MAXX; i2 ++)
    {
      sum = 0;
      for (i3 = 0; i3 < 3; i3 ++)
        for (i4 = 0; i4 < 3; i4 ++)
        {
          sum += g3[i3][i4] * r[i+i3-1][i2+i4-1];
        }
      sum /= 8;
      lg[i][i2]=sum;
    }
  for (i = 1; i <= MAXX - 1; i ++)
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      r [i] [i2] = (unsigned char) lg [i] [i2];
    }
  for (i = 1; i < MAXX; i ++)
    for (i2 = 1; i2 < MAXX; i2 ++)
    {
      sum = 0;
      for (i3 = 0; i3 < 3; i3 ++)
        for (i4 = 0; i4 < 3; i4 ++)
        {
          sum += g3[i3][i4] * g[i+i3-1][i2+i4-1];
        }
      sum /= 8;
      lg[i][i2]=sum;
    }
  for (i = 1; i <= MAXX - 1; i ++)
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      g [i] [i2] = (unsigned char) lg [i] [i2];
    }
  for (i = 1; i < MAXX; i ++)
    for (i2 = 1; i2 < MAXX; i2 ++)
    {
      float sum = 0;
      for (i3 = 0; i3 < 3; i3 ++)
        for (i4 = 0; i4 < 3; i4 ++)
        {
          sum += g3[i3][i4] * b[i+i3-1][i2+i4-1];
        }
      sum /= 8;
      lg[i][i2]=sum;
    }
  for (i = 1; i <= MAXX - 1; i ++)
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      b [i] [i2] = (unsigned char) lg [i] [i2];
    }*/

  // Set the height mask for the lowest sunrays touching the landscape's surface
  // This is just an approximation presuming the sun is a vertical line
  float m1 = mzoom / hh;
  float ih = tan ((sungamma + 5) * PI / 180) / m1; // 0 degree vertical sun radius
  for (x = 0; x <= MAXX; x ++)
  {
    float rayheight = hw [x] [MAXX];
    for (z = MAXX; z >= 0; z --)
    {
      unsigned short maxheight = (int) rayheight;
      if (rayheight < hw [x] [z])
      {
        rayheight = hw [x] [z];
        maxheight = hw [x] [z];
      }
      rayheight -= ih;
      hray [x] [z] = maxheight;
    }
  }

  // precalculate water light, always the same angle
  int nlwater = 1200 - (int) (1000.0 * 2.0 * fabs ((90.0 - sungamma) * PI / 180.0) / PI);

  // precalculate a height average
  int midheight = (highestpoint + lowestpoint) / 2;

  // set minimum ambient light
  int minambient = 100;
  if (!day) minambient = 50;

  // Set the luminance of the landscape
  for (x = 0; x <= MAXX; x ++)
    for (z = 0; z <= MAXX; z ++)
    {
      int xm1 = getCoord (x - 1);
      int xp1 = getCoord (x + 1);
      int zm1 = getCoord (z - 1);
      int zp1 = getCoord (z + 1);

      // Calculate the normal vectors
      a[0] = 0;
      a[1] = (float) (hw[x][zm1] - hw[x][z]) * mzoom;
      a[2] = -hh;
      c[0] = -hh;
      c[1] = (float) (hw[xm1][z] - hw[x][z]) * mzoom;
      c[2] =  0;
      normalcrossproduct( a, c, nw );
      a[0] = hh;
      a[1] = (float) (hw[xp1][z] - hw[x][z]) * mzoom;
      a[2] = 0;
      c[0] = 0;
      c[1] = (float) (hw[x][zm1] - hw[x][z]) * mzoom;
      c[2] = -hh;
      normalcrossproduct( a, c, no );
      a[0] = 0;
      a[1] = (float) (hw[x][zp1] - hw[x][z]) * mzoom;
      a[2] = hh;
      c[0] = hh;
      c[1] = (float) (hw[xp1][z] - hw[x][z]) * mzoom;
      c[2] = 0;
      normalcrossproduct( a, c, so );
      a[0] = -hh;
      a[1] = (float) (hw[xm1][z] - hw[x][z]) * mzoom;
      a[2] =  0;
      c[0] = 0;
      c[1] = (float) (hw[x][zp1] - hw[x][z]) * mzoom;
      c[2] = hh;
      normalcrossproduct( a, c, sw );
      float normx = (no[0] + nw[0] + so[0] + sw[0]) / 4.0;
      float normy = (no[1] + nw[1] + so[1] + sw[1]) / 4.0;
      float normz = (no[2] + nw[2] + so[2] + sw[2]) / 4.0;

      // Calculate the light hitting the surface
      float gamma = (float) acos (normx * lv [0] + normy * lv [1] + normz * lv [2]); // angle
      if (!isWater (f [x] [z]))
      {
        nl [x] [z] = 1200 - (int) (900.0 * 2.0 * fabs (gamma) / PI); // calculate light
        if (type == LAND_CANYON) // in canyons more ambient light in higher regions
          nl [x] [z] += (h [x] [z] - midheight) / 40; // typical max height diff is 10000
      }
      else
      {
        nl [x] [z] = nlwater; // precalculated light (above)
      }
      if (nl [x] [z] < minambient) // minimum ambient light
        nl [x] [z] = minambient;
      // Check whether this point is in the shadow of some mountain
      if (hw [x] [z] < hray [x] [z])
      {
        nl [x] [z] /= 2;
        if (nl [x] [z] < minambient)
          nl [x] [z] = minambient; // minimum ambient light
      }
    }

  // Smooth the luminance (very important)
  long g3_1[3][3]={{1,2,1},
                   {2,4,2},
                   {1,2,1}};
  for (i = 1; i <= MAXX - 1; i ++)
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      sum = 0;
      for (i3 = 0; i3 < 3; i3 ++)
        for (i4 = 0; i4 < 3; i4 ++)
        {
          sum += g3_1[i3][i4] * nl[i+i3-1][i2+i4-1];
        }
      sum /= 16;
      lg[i][i2]=sum;
    }
  for (i = 1; i <= MAXX - 1; i ++)
    for (i2 = 1; i2 <= MAXX - 1; i2 ++)
    {
      nl [i] [i2] = lg [i] [i2];
    }
//  rt = new RTerrain ();
//  rt->roam ((Landscape *) this);

  // Assign textures: tex1 = quad texture
  // if tex2 defined: tex1 = upper triangle texture, tex2 = lower triangle texture
  for (i = 0; i < MAXX; i ++)
    for (i2 = 0; i2 < MAXX; i2 ++)
    {
      drawrule [i] [i2] = 0;
      
      int f1 = f [i] [i2], f2 = f [i + 1] [i2], f3 = f [i] [i2 + 1], f4 = f [i + 1] [i2 + 1];
      if (isType (f1, GRASS))
      {
        tex1 [i] [i2] = texgrass->textureID;
      }
      else if (isWoods (f1) || isType (f1, MOONSAND) || isType (f1, REDSAND) || isType (f1, REDTREE0) || isType (f1, CACTUS0) || isType (f1, GREYSAND))
      {
        tex1 [i] [i2] = texredsand->textureID;
      }
      else if (isType (f1, GRAVEL))
      {
        tex1 [i] [i2] = texgravel1->textureID;
      }
      else if (isWater (f1))
      {
/*          if (isWater (f2) && isWater (f3) && isWater (f4))
          tex1 [i] [i2] = texwater;
        else*/
        if (type == 0 || type == 2)
          tex1 [i] [i2] = texgrass->textureID;
        else if (type == 1)
          tex1 [i] [i2] = 0xFF;
      }
      else if (isType (f1, ROCKS) || isType (f1, ROCKS2))
        tex1 [i] [i2] = texrocks->textureID;
      else if (isType (f1, REDSTONE))
        tex1 [i] [i2] = texredstone->textureID;
      else if (isType (f1, DESERTSAND))
        tex1 [i] [i2] = texsand->textureID;
      else
        tex1 [i] [i2] = 0xFF;
     
/*        if (!isWater (f1) && isWater (f2) && isWater (f3) && isWater (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texgrass; tex2 [i] [i2] = texwater; }
      if (isWater (f1) && !isWater (f2) && !isWater (f3) && !isWater (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texgrass; tex2 [i] [i2] = texgrass; }
      if (!isWater (f1) && !isWater (f2) && !isWater (f3) && isWater (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texgrass; tex2 [i] [i2] = texgrass; }
      if (isWater (f1) && isWater (f2) && isWater (f3) && !isWater (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texwater; tex2 [i] [i2] = texgrass; }
      
      if (isWater (f1) && !isWater (f2) && isWater (f3) && isWater (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texgrass; tex2 [i] [i2] = texwater; }
      if (!isWater (f1) && isWater (f2) && !isWater (f3) && !isWater (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texgrass; tex2 [i] [i2] = texgrass; }
      if (!isWater (f1) && !isWater (f2) && isWater (f3) && !isWater (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texgrass; tex2 [i] [i2] = texgrass; }
      if (isWater (f1) && isWater (f2) && !isWater (f3) && isWater (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texwater; tex2 [i] [i2] = texgrass; }*/

//        if (isWater (f1) || isWater (f2) || isWater (f3) || isWater (f4))
//        { tex1 [i] [i2] = texwater; }

      if (!isGlacier (f1) && isGlacier (f2) && isGlacier (f3) && isGlacier (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = 0xFF; }
      if (isGlacier (f1) && !isGlacier (f2) && !isGlacier (f3) && !isGlacier (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = texrocks->textureID; }
      if (!isGlacier (f1) && !isGlacier (f2) && !isGlacier (f3) && isGlacier (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = texrocks->textureID; }
      if (isGlacier (f1) && isGlacier (f2) && isGlacier (f3) && !isGlacier (f4))
      { drawrule [i] [i2] = 1; tex1 [i] [i2] = 0xFF; tex2 [i] [i2] = texrocks->textureID; }
      
      if (isGlacier (f1) && !isGlacier (f2) && isGlacier (f3) && isGlacier (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = 0xFF; }
      if (!isGlacier (f1) && isGlacier (f2) && !isGlacier (f3) && !isGlacier (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = texrocks->textureID; }
      if (!isGlacier (f1) && !isGlacier (f2) && isGlacier (f3) && !isGlacier (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = texrocks->textureID; tex2 [i] [i2] = texrocks->textureID; }
      if (isGlacier (f1) && isGlacier (f2) && !isGlacier (f3) && isGlacier (f4))
      { drawrule [i] [i2] = 2; tex1 [i] [i2] = 0xFF; tex2 [i] [i2] = texrocks->textureID; }
    }

}

// Get height over landscape/water, no interpolation (fast)
float GLLandscape::getHeight (float x, float z)
{
  int mx = getCoord ((int)x+MAXX/2);
  int mz = getCoord (MAXX/2-(int)z);
  return (ZOOM * ((float)hw[mx][mz]*zoomz - zoomz2));
}

// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactHeight2 (float x, float z)
{
  float mx = x+MAXX/2;
  float mz = (float)MAXX/2-z;
  int mx1 = (int) mx;
  int mz1 = (int) mz;
  mx1 -= mx1 & 1;
  mz1 -= mz1 & 1;
  int mx2 = mx1 + 2;
  int mz2 = mz1 + 2;
  int ax1 = getCoord (mx1);
  int ax2 = getCoord (mx2);
  int az1 = getCoord (mz1);
  int az2 = getCoord (mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2/4*zoomz - zoomz2));
}

// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactHeight3 (float x, float z)
{
  float mx = x+MAXX/2;
  float mz = (float)MAXX/2-z;
  int mx1 = (int) mx;
  int mz1 = (int) mz;
  mx1 -= mx1 % 3;
  mz1 -= mz1 % 3;
  int mx2 = mx1 + 3;
  int mz2 = mz1 + 3;
  int ax1 = getCoord (mx1);
  int ax2 = getCoord (mx2);
  int az1 = getCoord (mz1);
  int az2 = getCoord (mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2/9*zoomz - zoomz2));
}

// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactHeight4 (float x, float z)
{
  float mx = x+MAXX/2;
  float mz = (float)MAXX/2-z;
  int mx1 = (int) mx;
  int mz1 = (int) mz;
  mx1 -= mx1 & 3;
  mz1 -= mz1 & 3;
  int mx2 = mx1 + 4;
  int mz2 = mz1 + 4;
  int ax1 = getCoord (mx1);
  int ax2 = getCoord (mx2);
  int az1 = getCoord (mz1);
  int az2 = getCoord (mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2/16*zoomz - zoomz2));
}

// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactHeight (float x, float z)
{
  if (gridstep == 2) return getExactHeight2 (x, z);
  else if (gridstep == 3) return getExactHeight3 (x, z);
  else if (gridstep == 4) return getExactHeight4 (x, z);
  float mx = x+MAXX/2;
  float mz = (float)MAXX/2-z;
  int mx1 = (int) mx;
  int mz1 = (int) mz;
  int mx2 = mx1 + 1;
  int mz2 = mz1 + 1;
  int ax1 = getCoord (mx1);
  int ax2 = getCoord (mx2);
  int az1 = getCoord (mz1);
  int az2 = getCoord (mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2*zoomz - zoomz2));
}

// Get height over landscape/water without ZOOM scaling, linear interpolation (slow)
// Only used to draw trees
// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactLSHeight2 (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) mx;
  int mz1 = (int) mz;
  mx1 -= mx1 & 1;
  mz1 -= mz1 & 1;
  int mx2 = mx1 + 2;
  int mz2 = mz1 + 2;
  int ax1 = getCoord (mx1);
  int ax2 = getCoord (mx2);
  int az1 = getCoord (mz1);
  int az2 = getCoord (mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (h2/4*zoomz - zoomz2);
}

// Get height over landscape/water without ZOOM scaling, linear interpolation (slow)
// Only used to draw trees
// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactLSHeight3 (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) mx;
  int mz1 = (int) mz;
  mx1 -= mx1 % 3;
  mz1 -= mz1 % 3;
  int mx2 = mx1 + 3;
  int mz2 = mz1 + 3;
  int ax1 = getCoord (mx1);
  int ax2 = getCoord (mx2);
  int az1 = getCoord (mz1);
  int az2 = getCoord (mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (h2/9*zoomz - zoomz2);
}

// Get height over landscape/water without ZOOM scaling, linear interpolation (slow)
// Only used to draw trees
// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactLSHeight4 (float x, float z)
{
  float mx = x;
  float mz = z;
  int mx1 = (int) mx;
  int mz1 = (int) mz;
  mx1 -= mx1 & 3;
  mz1 -= mz1 & 3;
  int mx2 = mx1 + 4;
  int mz2 = mz1 + 4;
  int ax1 = getCoord (mx1);
  int ax2 = getCoord (mx2);
  int az1 = getCoord (mz1);
  int az2 = getCoord (mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (h2/16*zoomz - zoomz2);
}

// Get height over landscape/water without ZOOM scaling, linear interpolation (slow)
// Only used to draw trees
// Get height over landscape/water, linear interpolation (slow)
float GLLandscape::getExactLSHeight (float x, float z)
{
  if (gridstep == 2) return getExactLSHeight2 (x, z);
  else if (gridstep == 3) return getExactLSHeight3 (x, z);
  else if (gridstep == 4) return getExactLSHeight4 (x, z);
  float mx = x;
  float mz = z;
  int mx1 = (int) mx;
  int mz1 = (int) mz;
  int mx2 = mx1 + 1;
  int mz2 = mz1 + 1;
  int ax1 = getCoord (mx1);
  int ax2 = getCoord (mx2);
  int az1 = getCoord (mz1);
  int az2 = getCoord (mz2);
  float h2 = (float)hw[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hw[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hw[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hw[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (h2*zoomz - zoomz2);
}

// Get height of lowest sunray, no interpolation
float GLLandscape::getRayHeight (float x, float z)
{
  int mx = getCoord ((int)x+MAXX/2);
  int mz = getCoord (MAXX/2-(int)z);
  return (ZOOM * ((float)hray[mx][mz]*zoomz - zoomz2));
}

// Get height of lowest sunray, linear interpolation
float GLLandscape::getExactRayHeight (float x, float z)
{
  float mx = x+MAXX/2;
  float mz = (float)MAXX/2-z;
  int mx1 = (int) mx;
  int mx2 = mx1 + 1;
  int mz1 = (int) mz;
  int mz2 = mz1 + 1;
  int ax1 = getCoord (mx1);
  int ax2 = getCoord (mx2);
  int az1 = getCoord (mz1);
  int az2 = getCoord (mz2);
  float h2 = (float)hray[ax1][az1]*((float)mx2-mx)*((float)mz2-mz) + (float)hray[ax2][az1]*(mx-mx1)*((float)mz2-mz) +
             (float)hray[ax1][az2]*((float)mx2-mx)*(mz-mz1) + (float)hray[ax2][az2]*(mx-mx1)*(mz-mz1);
  return (ZOOM * (h2*zoomz - zoomz2));
}

// Draw tree using two static quads (high quality, slow)
void GLLandscape::drawTree (float x, float y, float htree, float wtree, int phi)
{
  float ht = getExactLSHeight (x, y);
  // Draw tree using a single rotated quad (low quality, fast)
  float ex1 = cosi [phi] * wtree, ey1 = sine [phi] * wtree;
  float ex2 = -ex1, ey2 = -ey1;
  int myticker;
  float zy = 0;
  if (weather == 1) // stormy weather
  {
    myticker = (int) (0.05 / htree * lsticker / timestep + 1000 * wtree + (x + y) * 50);
    if (myticker != 0)
      myticker %= 360;
    zy = 0.2 * (2.0 + sine [myticker]);
  }
  if (texturetree1 >= 0)
  {
    glBindTexture (GL_TEXTURE_2D, texturetree1);
    glBegin (GL_QUADS);
    glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
    glTexCoord2d (0, 1);
    glVertex3f (hh2*(ex1+x) - 1.0, ht + htree, hh2*(MAXX-(ey1+y+zy)) - 1.0);
    glTexCoord2d (1, 1);
    glVertex3f (hh2*(ex2+x) - 1.0, ht + htree, hh2*(MAXX-(ey2+y+zy)) - 1.0);
    glTexCoord2d (1, 0);
    glVertex3f (hh2*(ex2+x) - 1.0, ht, hh2*(MAXX-(ey2+y)) - 1.0);
    glTexCoord2d (0, 0);
    glVertex3f (hh2*(ex1+x) - 1.0, ht, hh2*(MAXX-(ey1+y)) - 1.0);
    glEnd ();
  }
  if (quality >= 2 && texturetree2 >= 0)
  {
    wtree *= 1.4F;
    ht += htree * 0.4F;
    zy *= 0.4F;
    phi += 45;
    if (phi >= 360) phi -= 360;
    ex1 = cosi [phi] * wtree; ey1 = sine [phi] * wtree;
    ex2 = -ex1; ey2 = -ey1;
    phi += 90;
    if (phi >= 360) phi -= 360;
    float ex3 = cosi [phi] * wtree, ey3 = sine [phi] * wtree;
    float ex4 = -ex3, ey4 = -ey3;
    glBindTexture (GL_TEXTURE_2D, texturetree2);
    glBegin (GL_QUADS);
    glTexCoord2d (0, 1);
    glVertex3f (hh2*(ex1+x) - 1.0, ht, hh2*(MAXX-(ey1+y+zy)) - 1.0);
    glTexCoord2d (1, 1);
    glVertex3f (hh2*(ex3+x) - 1.0, ht, hh2*(MAXX-(ey3+y+zy)) - 1.0);
    glTexCoord2d (1, 0);
    glVertex3f (hh2*(ex2+x) - 1.0, ht, hh2*(MAXX-(ey2+y+zy)) - 1.0);
    glTexCoord2d (0, 0);
    glVertex3f (hh2*(ex4+x) - 1.0, ht, hh2*(MAXX-(ey4+y+zy)) - 1.0);
    glEnd ();
  }
}

// currently not implemented
/*void GLLandscape::drawTreeGrid (float x, float y, float htree, float wtree, int phi)
{
  float ht = getExactLSHeight (x, y);
  int myticker;
  float zy = 0;
  if (weather == 1)
  {
    myticker = (int) (0.05 / htree * lsticker + 1000 * wtree + (x + y) * 50);
    if (myticker != 0)
      myticker %= 360;
    zy = 0.2 * (2.0 + sine [myticker]);
  }
  glBegin (GL_QUADS);
  glColor3ub (treecolor.c [0], treecolor.c [1], treecolor.c [2]);
  glTexCoord2d (0, 1);
  glVertex3f (hh2*(-wtree+x) - 1.0, ht + htree, hh2*(MAXX-(y+zy)) - 1.0);
  glTexCoord2d (1, 1);
  glVertex3f (hh2*(wtree+x) - 1.0, ht + htree, hh2*(MAXX-(y+zy)) - 1.0);
  glTexCoord2d (1, 0);
  glVertex3f (hh2*(wtree+x) - 1.0, ht, hh2*(MAXX-(y)) - 1.0);
  glTexCoord2d (0, 0);
  glVertex3f (hh2*(-wtree+x) - 1.0, ht, hh2*(MAXX-(y)) - 1.0);
  glEnd ();
  glBegin (GL_QUADS);
  glTexCoord2d (0, 1);
  glVertex3f (hh2*(x) - 1.0, ht + htree, hh2*(MAXX-(-wtree+y+zy)) - 1.0);
  glTexCoord2d (1, 1);
  glVertex3f (hh2*(x) - 1.0, ht + htree, hh2*(MAXX-(wtree+y+zy)) - 1.0);
  glTexCoord2d (1, 0);
  glVertex3f (hh2*(x) - 1.0, ht, hh2*(MAXX-(wtree+y)) - 1.0);
  glTexCoord2d (0, 0);
  glVertex3f (hh2*(x) - 1.0, ht, hh2*(MAXX-(-wtree+y)) - 1.0);
  glEnd ();
}*/

int visibility = 0;

// Fast landscape rendering without textures
void GLLandscape::drawQuadStrip (int x1, int y1, int x2, int y2)
{
  int x, y, xs, ys;
  float cr, cg, cb;
  bool water = false;
  bool last = false;
  int step = fargridstep;
  float texred, texgreen, texblue;

  glDisable (GL_TEXTURE_2D);

  CTexture *tex;

  x1 -= x1 % step;
  y1 -= y1 % step;

  for (xs = x1; xs < x2;)
  {
//    glBegin (GL_QUAD_STRIP);
    x = getCoord (xs);
    for (ys = y1; ys < y2;)
    {
      y = getCoord (ys);
      if (gl->isSphereInFrustum (hh2*(xs) - 1.0, (float)hw[x][y]*zoomz - zoomz2, hh2*(MAXX-(ys)) - 1.0, hh2*4*step))
      {
        int xstep = getCoord (x + step);
        int a = selectColor (x, y);
        if (a == 4 || a == 5 || a == 6 || a == 8 || a == 9)
          water = true;
        if (a != 6 && a != 9)
        {
          if (!last)
          {
            last = true;
            glBegin (GL_QUAD_STRIP);
          }
          tex = NULL;
          if (a == 0 || a == 1 || a == 10 || a == 12 || a == 13)
            tex = texgrass;
          else if (a == 2 || a == 7)
            tex = texrocks;
          else if (a == 4 || a == 5 || a == 6 || a == 8 || a == 9)
            tex = texgrass; // not texwater!
          else if (a == 11)
            tex = texredstone;
          else if (a == 13)
            tex = texsand;
          if (tex == NULL)
          {
            texred = 1.0F;
            texgreen = 1.0F;
            texblue = 1.0F;
          }
          else
          {
            texred = tex->texred;
            texgreen = tex->texgreen;
            texblue = tex->texblue;
          }
          float fac = 0.001F * (nl [x] [y] + (short) dl [x] [y] * 16) * sunlight / 256.0F;
          cr = (float) r [x] [y] * fac * texred;
          cg = (float) g [x] [y] * fac * texgreen;
          cb = (float) b [x] [y] * fac * texblue;
          if (cr >= texred) cr = texred;
          if (cg >= texgreen) cg = texgreen;
          if (cb >= texblue) cb = texblue;
          glColor3f (cr, cg, cb);
          glVertex3f (hh2*xs - 1.0, (float)h[x][y]*zoomz - zoomz2, hh2*(MAXX-ys) - 1.0);
          fac = 0.001F * (nl [xstep] [y] + (short) dl [xstep] [y] * 16) * sunlight / 256.0F;
          cr = (float) r [x + step] [y] * fac * texred;
          cg = (float) g [x + step] [y] * fac * texgreen;
          cb = (float) b [x + step] [y] * fac * texblue;
          if (cr >= texred) cr = texred;
          if (cg >= texgreen) cg = texgreen;
          if (cb >= texblue) cb = texblue;
          glColor3f (cr, cg, cb);
          glVertex3f (hh2*(xs+step) - 1.0, (float)h[xstep][y]*zoomz - zoomz2, hh2*(MAXX-ys) - 1.0);
        }
        else
        {
          if (last)
            glEnd ();
          last = false;
        }
      }
      ys += step;
    }
    if (last)
    {
      glEnd ();
      last = false;
    }
    xs += step;
  }

  last = false;
  if (water)
  {
    float texlight;
    float watergreen = 0.00025;
    if (day) watergreen = 0.0004;
    for (xs = x1; xs < x2;)
    {
      x = getCoord (xs);
//      glBegin (GL_QUAD_STRIP);
      for (ys = y1; ys < y2;)
      {
        y = getCoord (ys);
        int xstep = getCoord (x + step);
        int ystep = getCoord (y + step);
        int ymstep = getCoord (y - step);
        if (isWater (f [x] [y]) || isWater (f [xstep] [y]) || isWater (f [xstep] [ystep]) || isWater (f [x] [ystep]) ||
            isWater (f [x] [ymstep]) || isWater (f [xstep] [ymstep]))
        {
          float h1 = hw [x] [y];
          if (hw [xstep] [ystep] < h1) h1 = hw [xstep] [ystep];
          if (hw [xstep] [y] < h1) h1 = hw [xstep] [y];
          if (hw [x] [ystep] < h1) h1 = hw [x] [ystep];
          if (hw [x] [ymstep] < h1) h1 = hw [x] [ymstep];
          if (hw [xstep] [ymstep] < h1) h1 = hw [xstep] [ymstep];
          if (gl->isSphereInFrustum (hh2*(xs) - 1.0, (float)h1*zoomz - zoomz2, hh2*(MAXX-(ys)) - 1.0, hh2*2*step))
          {
            if (!last)
            {
              last = true;
              glBegin (GL_QUAD_STRIP);
            }

/*
  float li [4] = {1, 1, 1, 1};
  float lz1 = fabs ((float) MAXX/2 + camx - xs);
  float lz2 = fabs ((float) MAXX/2 + camx - xs - step);
  if (lz1 <= 5 || lz2 <= 5)
  {
    float dx1 = -((float) MAXX/2 - camz - ys);
    float dx2 = -((float) MAXX/2 - camz - ys - step);
    float dy = fabs (camy - (h1*zoomz - zoomz2) * ZOOM);
    float dgamma1 = fabs (atan (dy / dx1) * 180.0 / PI - sungamma);
//    float dgamma2 = fabs (atan (dy / dx2) * 180.0 / PI - sungamma);
    dgamma1 /= 4;
//    dgamma2 /= 5;
    float test;
    test = (5.0 - dgamma1) * (5.0 - lz1);
    if (test > 1.0 && dgamma1 < 5.0 && lz1 < 5.0) li [0] *= test;
    test = (5.0 - dgamma1) * (5.0 - lz2);
    if (test > 1.0 && dgamma1 < 5.0 && lz2 < 5.0) li [1] *= test;
//    test = (5.0 - dgamma2) * (5.0 - lz1);
//    if (test > 1.0 && dgamma2 < 5.0 && lz1 < 5.0) li [3] *= test;
//    test = (5.0 - dgamma2) * (5.0 - lz2);
//    if (test > 1.0 && dgamma2 < 5.0 && lz2 < 5.0) li [2] *= test;
  }
// do not forget to multiply li array
*/

            texlight = texwater->texlight;
            float d = watergreen * (h1 - h [x] [y]);
            if (d > 0.75) d = 0.75;
            if (type == 0)
            {
              cr = 0.1 * 256;
              cg = (0.85 - d) * 256;
              cb = (0.6 + d) * 256;
            }
            else if (type == 2)
            {
              cr = (0.55 - d/2) * 256;
              cg = (0.55 - d/2) * 256;
              cb = (0.6 + d) * 256;
            }
            float fac = 0.001 * (nl [x] [y] + (short) dl [x] [y] * 16) * sunlight / 256.0 * texlight;
            cr = (float) cr * fac;
            cg = (float) cg * fac;
            cb = (float) cb * fac;
            if (cr > texlight) cr = texlight;
            if (cg < 0.1 * texlight) cg = 0.1 * texlight;
            if (cg > texlight) cg = texlight;
            if (cb > texlight) cb = texlight;
            glColor3f (cr, cg, cb);
            glVertex3f (hh2*xs - 1.0, h1*zoomz - zoomz2, hh2*(MAXX-ys) - 1.0);

            d = watergreen * (h1 - h [xstep] [y]);
            if (d > 0.75) d = 0.75;
            if (type == 0)
            {
              cr = 0.1 * 256;
              cg = (0.85 - d) * 256;
              cb = (0.6 + d) * 256;
            }
            else if (type == 2)
            {
              cr = (0.55 - d/2) * 256;
              cg = (0.55 - d/2) * 256;
              cb = (0.6 + d) * 256;
            }
            fac = 0.001 * (nl [xstep] [y] + (short) dl [xstep] [y] * 16) * sunlight / 256.0 * texlight;
            cr = (float) cr * fac;
            cg = (float) cg * fac;
            cb = (float) cb * fac;
            if (cr > texlight) cr = texlight;
            if (cg < 0.1 * texlight) cg = 0.1 * texlight;
            if (cg > texlight) cg = texlight;
            if (cb > texlight) cb = texlight;
            glColor3f (cr, cg, cb);
            glVertex3f (hh2*(xs+step) - 1.0, h1*zoomz - zoomz2, hh2*(MAXX-ys) - 1.0);
          }
          else
          {
            if (last)
              glEnd ();
            last = false;
          }
        }
        ys += step;
      }
      if (last)
      {
        glEnd ();
        last = false;
      }
      xs += step;
    }
  }
}

// Draw a single untextured quad
void GLLandscape::drawQuad (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
  int j;
  int step = fargridstep;
  float texred, texgreen, texblue;
  CTexture *tex = NULL;
  float col [4] [3];
  float pos [4] [3];
  float fac;
  int px [4], py [4];
  int xs = x1, ys = y1;
  int x = getCoord (xs);
  int y = getCoord (ys);
  px [0] = x1; py [0] = y1;
  px [1] = x2; py [1] = y2;
  px [2] = x3; py [2] = y3;
  px [3] = x4; py [3] = y4;
  float minh = h[x][y];
  float maxh = minh;
  for (j = 1; j < 4; j ++)
  {
    int h1 = h [getCoord (px [j])] [getCoord (py [j])];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2 * zoomz - zoomz2;
  float size = (maxh - minh) * zoomz * step; // exakt wäre mal 0.5
  if (size < hh2 / 2 * step)
    size = hh2 / 2 * step;
  if (!gl->isSphereInFrustum (hh2*(0.5+xs) - 1.0, midh, hh2*(MAXX-(0.5+ys)) - 1.0, size * 2))
    return;
  int a = selectColor (x, y);
  tex = NULL;
  if (a == 0 || a == 1 || a == 10 || a == 12 || a == 13)
    tex = texgrass;
  else if (a == 2 || a == 7)
    tex = texrocks;
  else if (a == 4 || a == 5 || a == 6 || a == 8 || a == 9)
    tex = texgrass; // not texwater!
  else if (a == 11)
    tex = texredstone;
  else if (a == 13)
    tex = texsand;
  if (tex == NULL)
  {
    texred = 1.0F;
    texgreen = 1.0F;
    texblue = 1.0F;
  }
  else
  {
    texred = tex->texred;
    texgreen = tex->texgreen;
    texblue = tex->texblue;
  }
  glDisable (GL_TEXTURE_2D);
  float fac2 = 0.001F * sunlight / 256.0F;
  for (j = 0; j < 4; j ++)
  {
    int mx = getCoord (px [j]), my = getCoord (py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac * texred;
    col [j] [1] = g [mx] [my] * fac * texgreen;
    col [j] [2] = b [mx] [my] * fac * texblue;
    if (col [j] [0] >= texred) col [j] [0] = texred;
    if (col [j] [1] >= texgreen) col [j] [1] = texgreen;
    if (col [j] [2] >= texblue) col [j] [2] = texblue;
    pos [j] [0] = hh2*px[j] - 1.0;
    pos [j] [1] = (float)h[mx][my]*zoomz - zoomz2;
    pos [j] [2] = hh2*(MAXX-py[j]) - 1.0;
  }
  glBegin (GL_QUADS);
  for (j = 0; j < 4; j ++)
  {
    glColor3fv (col [j]);
    glVertex3fv (pos [j]);
  }
  glEnd();
}

// Draw a single untextured triangle
void GLLandscape::drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3)
{
  int j;
  int step = fargridstep;
  float texred, texgreen, texblue;
  CTexture *tex = NULL;
  float col [3] [3];
  float pos [3] [3];
  float fac;
  int px [3], py [3];
  int xs = x1, ys = y1;
  int x = getCoord (xs);
  int y = getCoord (ys);
  px [0] = x1; py [0] = y1;
  px [1] = x2; py [1] = y2;
  px [2] = x3; py [2] = y3;
  float minh = h[x][y];
  float maxh = minh;
  for (j = 1; j < 3; j ++)
  {
    int h1 = h [getCoord (px [j])] [getCoord (py [j])];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2 * zoomz - zoomz2;
  float size = (maxh - minh) * zoomz * step; // exakt wäre mal 0.5
  if (size < hh2 / 2 * step)
    size = hh2 / 2 * step;
  if (!gl->isSphereInFrustum (hh2*(0.5+xs) - 1.0, midh, hh2*(MAXX-(0.5+ys)) - 1.0, size * 2))
    return;
  int a = selectColor (x, y);
  tex = NULL;
  if (a == 0 || a == 1 || a == 10 || a == 12 || a == 13)
    tex = texgrass;
  else if (a == 2 || a == 7)
    tex = texrocks;
  else if (a == 4 || a == 5 || a == 6 || a == 8 || a == 9)
    tex = texgrass; // not texwater!
  else if (a == 11)
    tex = texredstone;
  else if (a == 13)
    tex = texsand;
  if (tex == NULL)
  {
    texred = 1.0F;
    texgreen = 1.0F;
    texblue = 1.0F;
  }
  else
  {
    texred = tex->texred;
    texgreen = tex->texgreen;
    texblue = tex->texblue;
  }
  glDisable (GL_TEXTURE_2D);
  float fac2 = 0.001F * sunlight / 256.0F;
  for (j = 0; j < 3; j ++)
  {
    int mx = getCoord (px [j]), my = getCoord (py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac * texred;
    col [j] [1] = g [mx] [my] * fac * texgreen;
    col [j] [2] = b [mx] [my] * fac * texblue;
    if (col [j] [0] >= texred) col [j] [0] = texred;
    if (col [j] [1] >= texgreen) col [j] [1] = texgreen;
    if (col [j] [2] >= texblue) col [j] [2] = texblue;
    pos [j] [0] = hh2*px[j] - 1.0;
    pos [j] [1] = (float)h[mx][my]*zoomz - zoomz2;
    pos [j] [2] = hh2*(MAXX-py[j]) - 1.0;
  }
  glBegin (GL_TRIANGLES);
  for (j = 0; j < 3; j ++)
  {
    glColor3fv (col [j]);
    glVertex3fv (pos [j]);
  }
  glEnd();
}

// Draw a single textured quad
void GLLandscape::drawTexturedQuad (int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
  int j;
  int step = gridstep;
//    int tx, ty;
//    float tfx, tfy, tfinc;
//  float texlight = 1.0;
  bool texture = false;
  float col [4] [3];
  float pos [4] [3];
  float tf [4] [2];
  float fac;
  float texzoom;
  int px [4], py [4];
  int x = getCoord (x2);
  int y = getCoord (y2);
  px [0] = x1; py [0] = y1;
  px [1] = x2; py [1] = y2;
  px [2] = x3; py [2] = y3;
  px [3] = x4; py [3] = y4;
  float minh = h[x][y];
  float maxh = minh;
  for (j = 1; j < 4; j ++)
  {
    int h1 = h [getCoord (px [j])] [getCoord (py [j])];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2 * zoomz - zoomz2;
  float size = (maxh - minh) * zoomz * step; // exakt wäre mal 0.5
  if (size < hh2 / 2 * step)
    size = hh2 / 2 * step;
  if (!gl->isSphereInFrustum (hh2*(0.5+x2) - 1.0, midh, hh2*(MAXX-(0.5+y2)) - 1.0, size * 2))
    return;
  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    gl->enableTextures (tex1 [x] [y]);
  }
  int texcoord = 0;
  if (tex1 [x] [y] == texredstone->textureID)
  {
    texzoom = 0.5;
    texcoord = 1;
  }
  else if (tex1 [x] [y] != texgrass->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }
  float fac2 = 0.001F * sunlight / 256.0F;
  for (j = 0; j < 4; j ++)
  {
    int mx = getCoord (px [j]), my = getCoord (py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac;
    col [j] [1] = g [mx] [my] * fac;
    col [j] [2] = b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = hh2*px[j] - 1.0;
    pos [j] [1] = (float)h[mx][my]*zoomz - zoomz2;
    pos [j] [2] = hh2*(MAXX-py[j]) - 1.0;
  }
  for (j = 0; j < 4; j ++)
  {
    if (texcoord == 0)
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) py [j] * texzoom;
    }
    else
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) h [getCoord (px [j])] [getCoord (py [j])] * texzoom / 400.0;
    }
  }
  glBegin (GL_QUADS);
  for (j = 0; j < 4; j ++)
  {
    if (texture)
      glTexCoord2fv (tf [j]);
    glColor3fv (col [j]);
    glVertex3fv (pos [j]);
  }
  glEnd();
}

// Draw a single textured triangle
void GLLandscape::drawTexturedTriangle (int x1, int y1, int x2, int y2, int x3, int y3)
{
  int j;
  int step = gridstep;
//    int tx, ty;
//    float tfx, tfy, tfinc;
//  float texlight = 1.0;
  bool texture = false;
  float col [3] [3];
  float pos [3] [3];
  float tf [3] [2];
  float fac;
  float texzoom;
  int px [3], py [3];
  int x = getCoord (x2);
  int y = getCoord (y2);
  px [0] = x1; py [0] = y1;
  px [1] = x2; py [1] = y2;
  px [2] = x3; py [2] = y3;
  float minh = h[x][y];
  float maxh = minh;
  for (j = 1; j < 3; j ++)
  {
    int h1 = h [getCoord (px [j])] [getCoord (py [j])];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2 * zoomz - zoomz2;
  float size = (maxh - minh) * zoomz * step; // exakt wäre mal 0.5
  if (size < hh2 / 2 * step)
    size = hh2 / 2 * step;
  if (!gl->isSphereInFrustum (hh2*(0.5+x2) - 1.0, midh, hh2*(MAXX-(0.5+y2)) - 1.0, size * 2))
    return;
  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    gl->enableTextures (tex1 [x] [y]);
  }
  int texcoord = 0;
  if (tex1 [x] [y] == texredstone->textureID)
  {
    texzoom = 0.5;
    texcoord = 1;
  }
  else if (tex1 [x] [y] != texgrass->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }
  float fac2 = 0.001F * sunlight / 256.0F;
  for (j = 0; j < 3; j ++)
  {
    int mx = getCoord (px [j]), my = getCoord (py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac;
    col [j] [1] = g [mx] [my] * fac;
    col [j] [2] = b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = hh2*px[j] - 1.0;
    pos [j] [1] = (float)h[mx][my]*zoomz - zoomz2;
    pos [j] [2] = hh2*(MAXX-py[j]) - 1.0;
  }
  for (j = 0; j < 3; j ++)
  {
    if (texcoord == 0)
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) py [j] * texzoom;
    }
    else
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) h [getCoord (px [j])] [getCoord (py [j])] * texzoom / 400.0;
    }
  }
  glBegin (GL_TRIANGLES);
  for (j = 0; j < 3; j ++)
  {
    if (texture)
      glTexCoord2fv (tf [j]);
    glColor3fv (col [j]);
    glVertex3fv (pos [j]);
  }
  glEnd();
}

// Draw a single textured quad
void GLLandscape::drawTexturedQuad (int xs, int ys)
{
  int j;
  int step = gridstep;
//    int tx, ty;
//    float tfx, tfy, tfinc;
//  float texlight = 1.0;
  bool texture = false;
  float col [4] [3];
  float pos [4] [3];
  float tf [4] [2];
  float fac;
  float texzoom;
  int px [4], py [4];
  int x = getCoord (xs);
  int y = getCoord (ys);
  px [0] = xs; py [0] = ys;
  px [1] = xs + step; py [1] = ys;
  px [2] = xs + step; py [2] = ys + step;
  px [3] = xs; py [3] = ys + step;
  float minh = h[x][y];
  float maxh = minh;
  for (j = 1; j < 4; j ++)
  {
    int h1 = h [getCoord (px [j])] [getCoord (py [j])];
    if (h1 > maxh) maxh = h1;
    else if (h1 < minh) minh = h1;
  }
  float midh = (minh + maxh) / 2 * zoomz - zoomz2;
  float size = (maxh - minh) * zoomz * step; // exakt wäre mal 0.5
  if (size < hh2 / 2 * step)
    size = hh2 / 2 * step;
  if (!gl->isSphereInFrustum (hh2*(0.5+xs) - 1.0, midh, hh2*(MAXX-(0.5+ys)) - 1.0, size * 2))
    return;
  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    gl->enableTextures (tex1 [x] [y]);
  }
  int texcoord = 0;
  if (tex1 [x] [y] == texredstone->textureID)
  {
    texzoom = 0.5;
    texcoord = 1;
  }
  else if (tex1 [x] [y] != texgrass->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }
  float fac2 = 0.001F * sunlight / 256.0F;
  for (j = 0; j < 4; j ++)
  {
    int mx = getCoord (px [j]), my = getCoord (py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac;
    col [j] [1] = g [mx] [my] * fac;
    col [j] [2] = b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = hh2*px[j] - 1.0;
    pos [j] [1] = (float)h[mx][my]*zoomz - zoomz2;
    pos [j] [2] = hh2*(MAXX-py[j]) - 1.0;
  }
  for (j = 0; j < 4; j ++)
  {
    if (texcoord == 0)
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) py [j] * texzoom;
    }
    else
    {
      tf [j] [0] = (float) px [j] * texzoom;
      tf [j] [1] = (float) h [getCoord (px [j])] [getCoord (py [j])] * texzoom / 400.0;
    }
  }
  glBegin (GL_QUADS);
  for (j = 0; j < 4; j ++)
  {
    if (texture)
      glTexCoord2fv (tf [j]);
    glColor3fv (col [j]);
    glVertex3fv (pos [j]);
  }
  glEnd();
}

// Draw a single textured water quad
void GLLandscape::drawWaterTexturedQuad (int xs, int ys)
{
  int i, j;
  int step = gridstep;
//    int tx, ty;
//    float tfx, tfy, tfinc;
//  float texlight = 1.0;
  bool texture = false;
  float col [4] [4];
  float pos [4] [3];
  float tf [4] [2];
  float li [4];
  float fac;
  float texzoom;
  int px [4], py [4];
  int x = getCoord (xs);
  int y = getCoord (ys);
  int xstep = getCoord (xs + step);
  int ystep = getCoord (ys + step);
  px [0] = xs; py [0] = ys; li [0] = (nl [x] [y] + (short) dl [x] [y] * 16);
  px [1] = xs + step; py [1] = ys; li [1] = (nl [xstep] [y] + (short) dl [xstep] [y] * 16);
  px [2] = xs + step; py [2] = ys + step; li [2] = (nl [xstep] [ystep] + (short) dl [xstep] [ystep] * 16);
  px [3] = xs; py [3] = ys + step; li [3] = (nl [x] [ystep] + (short) dl [x] [ystep] * 16);

  float h1 = hw [x] [y];
  for (i = 1; i <= 3; i ++)
  {
    int mx = getCoord (px [i]);
    int my = getCoord (py [i]);
    if (hw [mx] [my] < h1/* && isWater (f [px [i]] [py [i]])*/)
    {
      h1 = hw [mx] [my];
//      for (i2 = 0; i2 <= 3; i2 ++)
 //       li [i2] = li [i];
    }
  }

  if (!gl->isSphereInFrustum (hh2*(0.5+xs) - 1.0, (float) h1*zoomz - zoomz2, hh2*(MAXX-(0.5+ys)) - 1.0, hh2 * step))
    return;

//  glittering = 0;
  // glittering is obsolete, does not achieve the desired effect
  float glitter [4] = { 1, 1, 1, 1 };
  if (quality >= 2)
  {
    float dz1 = fabs ((float) MAXX/2 + camx - xs);
    float dz2 = fabs ((float) MAXX/2 + camx - xs - step);
    float dy = fabs (camy - (h1*zoomz - zoomz2) * ZOOM);
    float dtheta1 = fabs (atan (dy / dz1) * 180.0 / PI - 90);
    float dtheta2 = fabs (atan (dy / dz2) * 180.0 / PI - 90);
    dtheta1 /= 4; dtheta2 /= 4;
//    if (lz1 <= 5 || lz2 <= 5)
    {
      float dx1 = -((float) MAXX/2 - camz - ys);
      float dx2 = -((float) MAXX/2 - camz - ys - step);
//      float dy = fabs (camy - (h1*zoomz - zoomz2) * ZOOM);
      float dgamma1 = fabs (atan (dy / dx1) * 180.0 / PI - sungamma);
      float dgamma2 = fabs (atan (dy / dx2) * 180.0 / PI - sungamma);
      dgamma1 /= 4; dgamma2 /= 4;
      float sc = 1.0;
      float test;
      if (h1 >= hray [x] [y])
      {
        test = sc * exp ((-dgamma1 * dgamma1 - dtheta1 * dtheta1) / 8.0) + 0.98;
        if (test > 1.0)
        {
          glitter [0] = test;
          if (test > glittering) glittering = test;
        }
      }
      if (h1 >= hray [xstep] [y])
      {
        test = sc * exp ((-dgamma1 * dgamma1 - dtheta2 * dtheta2) / 8.0) + 0.98;
        if (test > 1.0)
        {
          glitter [1] = test;
          if (test > glittering) glittering = test;
        }
      }
      if (h1 >= hray [x] [ystep])
      {
        test = sc * exp ((-dgamma2 * dgamma2 - dtheta1 * dtheta1) / 8.0) + 0.98;
        if (test > 1.0)
        {
          glitter [3] = test;
          if (test > glittering) glittering = test;
        }
      }
      if (h1 >= hray [xstep] [ystep])
      {
        test = sc * exp ((-dgamma2 * dgamma2 - dtheta2 * dtheta2) / 8.0) + 0.98;
        if (test > 1.0)
        {
          glitter [2] = test;
          if (test > glittering) glittering = test;
        }
      }
/*      li [0] *= glitter [0];
      li [1] *= glitter [1];
      li [2] *= glitter [2];
      li [3] *= glitter [3];*/
    }
  }

  texture = true;
//  glActiveTextureARB(100);
  gl->enableTextures (texwater->textureID);
//  texlight = texwater->texlight;
  texzoom = 0.5;
  float watergreen = 0.00025;
  if (day) watergreen = 0.0004;
  float fac2 = 0.001 * sunlight/* * texlight*/;
  for (j = 0; j < 4; j ++)
  {
    int mx = getCoord (px [j]), my = getCoord (py [j]);
    float d = watergreen * (h1 - h [mx] [my]);
    if (d > 0.75) d = 0.75;
    fac = fac2 * li [j];
    if (type == 0)
    {
      col [j] [0] = 0.1 * fac;
      col [j] [1] = (0.85 - d) * fac;
      col [j] [2] = (0.6 + d) * fac;
    }
    else if (type == 2)
    {
      col [j] [0] = (0.55 - d/2) * fac;
      col [j] [1] = (0.55 - d/2) * fac;
      col [j] [2] = (0.6 + d) * fac;
    }
//    col [j] [3] = (float) abs (h1 - h [px [j]] [py [j]]) / 200 + 0.5;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] <= 0.1) col [j] [1] = 0.1;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
//    if (col [j] [3] >= 1.0) col [j] [3] = 1.0;
//    col [j] [3] = 0.5;
    pos [j] [0] = hh2*px[j] - 1.0;
    pos [j] [1] = (float)h1*zoomz - zoomz2;
    pos [j] [2] = hh2*(MAXX-py[j]) - 1.0;
  }
  for (j = 0; j < 4; j ++)
  {
    float waterspeed = 0.008;
    if (weather == 1) waterspeed = 0.016;
    tf [j] [0] = (float) px [j] * texzoom + waterspeed * lsticker / timestep;
    tf [j] [1] = (float) py [j] * texzoom;
  }
  
  glBegin (GL_QUADS);
  for (j = 0; j < 4; j ++)
  {
    if (texture)
    {
//            glMultiTexCoord2fARB(100, tf [j] [0], tf [j] [1]);
//            glMultiTexCoord2fARB(101, tf [j] [0], tf [j] [1]);
      glTexCoord2fv (tf [j]);
    }
    glColor4fv (col [j]);
    glVertex3fv (pos [j]);
  }
  glEnd();

//  glDisable (GL_DEPTH_TEST);

  if (quality >= 2 && glittering > 0)
  {
    glEnable (GL_BLEND);
    glDepthFunc (GL_LEQUAL);
    glBlendFunc (GL_ONE, GL_SRC_ALPHA);
    glEnable (GL_ALPHA_TEST);
    glAlphaFunc (GL_GEQUAL, 0.2);
  //  glBlendFunc (GL_ONE, GL_ONE);
    gl->enableTextures (texglitter1->textureID);
    gl->enableLinearTexture (texglitter1->textureID);
    glBegin (GL_QUADS);
    for (j = 0; j < 4; j ++)
    {
      if (texture)
      {
        tf [j] [0] = (px [j] * texzoom) + (float) ((lsticker / timestep / 2) & 7) * 0.6;
        tf [j] [1] = (py [j] * texzoom) + (float) ((lsticker / timestep / 2) & 7) * 0.6;
        glTexCoord2fv (tf [j]);
      }
      col [j] [3] = glitter [j] - 1.0;
      col [j] [0] = 1.0;
      col [j] [1] = 1.0;
      col [j] [2] = 1.0;
      glColor4fv (col [j]);
      glVertex3fv (pos [j]);
    }
    glEnd();
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable (GL_ALPHA_TEST);
    glDisable (GL_BLEND);
//  glEnable (GL_DEPTH_TEST);
  }

}

// Draw two textured triangles (quad)
void GLLandscape::drawTexturedTriangle1 (int xs, int ys)
{
  int j;
  int step = gridstep;
//    int tx, ty;
//    float tfx, tfy, tfinc;
//  float texlight = 1.0;
  bool texture = false;
  float col [4] [3];
  float pos [4] [3];
  float tf [4] [2];
  float fac;
  float texzoom;
  int px [4], py [4];
  int x = getCoord (xs);
  int y = getCoord (ys);
  px [0] = xs; py [0] = ys;
  px [1] = xs + step; py [1] = ys;
  px [2] = xs + step; py [2] = ys + step;
  px [3] = xs; py [3] = ys + step;

  if (!gl->isSphereInFrustum (hh2*(0.5+xs) - 1.0, (float)h[x][y]*zoomz - zoomz2, hh2*(MAXX-(0.5+ys)) - 1.0, hh2*2*step))
    return;

  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    gl->enableTextures (tex1 [x] [y]);
  }
  if (tex1 [x] [y] != texgrass->textureID && tex1 [x] [y] != texgrass->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }

  float fac2 = 0.001F * sunlight / 256.0F;

  for (j = 0; j < 4; j ++)
  {
    int mx = getCoord (px [j]), my = getCoord (py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my] * 16);
    col [j] [0] = r [mx] [my] * fac;
    col [j] [1] = g [mx] [my] * fac;
    col [j] [2] = b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = hh2*px[j] - 1.0;
    pos [j] [1] = (float)h[mx][my]*zoomz - zoomz2;
    pos [j] [2] = hh2*(MAXX-py[j]) - 1.0;
  }

  for (j = 0; j < 4; j ++)
  {
    tf [j] [0] = (float) px [j] * texzoom;
    tf [j] [1] = (float) py [j] * texzoom;
  }

  glBegin (GL_TRIANGLES);
  for (j = 0; j < 3; j ++)
  {
    if (texture)
      glTexCoord2fv (tf [j]);
    glColor3fv (col [j]);
    glVertex3fv (pos [j]);
  }
  glEnd();

  if (tex2 [x] [y] == 0xFF)
  {
    texture = false;
    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    gl->enableTextures (tex2 [x] [y]);
  }

  if (tex2 [x] [y] != texgrass->textureID && tex2 [x] [y] != texredstone->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }

  for (j = 0; j < 4; j ++)
  {
    tf [j] [0] = (float) px [j] * texzoom;
    tf [j] [1] = (float) py [j] * texzoom;
  }

  glBegin (GL_TRIANGLES);
  if (texture)
    glTexCoord2fv (tf [0]);
  glColor3fv (col [0]);
  glVertex3fv (pos [0]);
  for (j = 2; j < 4; j ++)
  {
    if (texture)
      glTexCoord2fv (tf [j]);
    glColor3fv (col [j]);
    glVertex3fv (pos [j]);
  }
  glEnd();
}

// Draw two textured triangles (quad)
void GLLandscape::drawTexturedTriangle2 (int xs, int ys)
{
  int j;
  int step = gridstep;
//    int tx, ty;
//    float tfx, tfy, tfinc;
//  float texlight = 1.0;
  bool texture = false;
  float col [4] [3];
  float pos [4] [3];
  float tf [4] [2];
  float fac;
  float texzoom;
  int px [4], py [4];
  int x = getCoord (xs);
  int y = getCoord (ys);
  px [0] = xs; py [0] = ys;
  px [1] = xs + step; py [1] = ys;
  px [2] = xs + step; py [2] = ys + step;
  px [3] = xs; py [3] = ys + step;

  if (!gl->isSphereInFrustum (hh2*(0.5+xs) - 1.0, (float)h[x][y]*zoomz - zoomz2, hh2*(MAXX-(0.5+ys)) - 1.0, hh2*2*step))
    return;

  if (tex1 [x] [y] == 0xFF)
  {
    texture = false;
    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    gl->enableTextures (tex1 [x] [y]);
  }

  if (tex1 [x] [y] != texgrass->textureID && tex1 [x] [y] != texredstone->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }

  float fac2 = 0.001F * sunlight / 256.0F;
  for (j = 0; j < 4; j ++)
  {
    int mx = getCoord (px [j]), my = getCoord (py [j]);
    fac = fac2 * (nl [mx] [my] + (short) dl [mx] [my]);
    col [j] [0] = r [mx] [my] * fac;
    col [j] [1] = g [mx] [my] * fac;
    col [j] [2] = b [mx] [my] * fac;
    if (col [j] [0] >= 1.0) col [j] [0] = 1.0;
    if (col [j] [1] >= 1.0) col [j] [1] = 1.0;
    if (col [j] [2] >= 1.0) col [j] [2] = 1.0;
    pos [j] [0] = hh2*px[j] - 1.0;
    pos [j] [1] = (float)h[mx][my]*zoomz - zoomz2;
    pos [j] [2] = hh2*(MAXX-py[j]) - 1.0;
  }

  for (j = 0; j < 4; j ++)
  {
    tf [j] [0] = (float) px [j] * texzoom;
    tf [j] [1] = (float) py [j] * texzoom;
  }

  glBegin (GL_TRIANGLES);
  for (j = 0; j < 2; j ++)
  {
    if (texture)
      glTexCoord2fv (tf [j]);
    glColor3fv (col [j]);
    glVertex3fv (pos [j]);
  }
  if (texture)
    glTexCoord2fv (tf [3]);
  glColor3fv (col [3]);
  glVertex3fv (pos [3]);
  glEnd();

  if (tex2 [x] [y] == 0xFF)
  {
    texture = false;
    glDisable (GL_TEXTURE_2D);
  }
  else
  {
    texture = true;
    gl->enableTextures (tex2 [x] [y]);
  }

  if (tex2 [x] [y] != texgrass->textureID && tex2 [x] [y] != texredstone->textureID)
  {
    texzoom = 0.5;
  }
  else
  {
    texzoom = 0.25;
  }

  for (j = 0; j < 4; j ++)
  {
    tf [j] [0] = (float) px [j] * texzoom;
    tf [j] [1] = (float) py [j] * texzoom;
  }

  glBegin (GL_TRIANGLES);
  for (j = 1; j < 4; j ++)
  {
    if (texture)
      glTexCoord2fv (tf [j]);
    glColor3fv (col [j]);
    glVertex3fv (pos [j]);
  }
  glEnd();
}

bool hb [100] [100];
float hdeg [360];

// in construction, enable some lines at #1
void GLLandscape::viewculling ()
{
  memset (hb, 0, sizeof (bool) * 10000);
  for (int s = 1; s < 50; s ++)
  {
    for (int i = -s; i < s; i ++)
    {
      int x, y, dx, dy;
      x = 50 + i; y = 50 + s;
      dx = x - 50; dy = y - 50;
      float phi;
      if (dx != 0) phi = atan ((float) dy / dx);
      else phi = PI;
      if (dx < 0) phi += 2.0F * PI;
      float dist = sqrt (dx * dx + dy * dy);
      float h = getHeight (camx + dx, camy + y);
      hb [y] [x] = true;
//      hb [50 - s] [50 + i]
//      hb [50 + i] [50 + s]
//      hb [50 - i] [50 + s]
    }
  }
}
    
void GLLandscape::draw (int phi, int gamma)
{
  char buf [STDSIZE];
  int i, i2, x, y;
  int xs, ys;

  viewculling ();

  float fac;

  int fardetail = quality;
  int middetail = quality;
  int lineardetail = -1;

  if (quality == 0)
  { neargridstep = 3; fargridstep = 3; middetail = -1; fardetail = -1; lineardetail = -1; }
  else if (quality == 1)
  { neargridstep = 2; fargridstep = 4; middetail = 2; fardetail = 2; lineardetail = -1; }
  else if (quality == 2)
  { neargridstep = 2; fargridstep = 4; middetail = 5; fardetail = 5; lineardetail = -1; }
  else if (quality == 3)
  { neargridstep = 2; fargridstep = 4; middetail = 7; fardetail = 7; lineardetail = 0; }
  else if (quality == 4)
  { neargridstep = 2; fargridstep = 2; middetail = -1; fardetail = 7; lineardetail = 0; }
  else
  { neargridstep = 1; fargridstep = 2; middetail = 3; fardetail = 7; lineardetail = 0; }

  if (phi < 0) phi += 360;
  else if (phi >= 360) phi -= 360;

  if (gamma < 0) gamma += 360;
  else if (gamma >= 360) gamma -= 360;

  if (phi < 0 || phi >= 360)
  {
    sprintf (buf, "Phi exceeds in file %s, line %s, val %d", __FILE__, __LINE__, phi);
    display (buf, LOG_ERROR);
  }

  if (gamma < 0 || gamma >= 360)
  {
    sprintf (buf, "Gamma exceeds in file %s, line %s, val %d", __FILE__, __LINE__, gamma);
    display (buf, LOG_ERROR);
  }

  glPushMatrix ();

  float z2 = ZOOM; // that's wrong if one would change MAXX
//  float z2 = ZOOM;
  glScalef(z2, ZOOM, z2);

  gl->extractFrustum ();
    
  float pseudoview = getView ();
  float radius = pseudoview / cosi [45];

/*  int w = (int) phi;
  float qx [4], qy [4];
  int addw [4] = { 45, 90, 45, 0 };

  int minx = 10000, maxx = -10000, miny = 10000, maxy = -10000;
  if (gamma >= 90 && gamma < 270)
  {
    for (i = 0; i < 4; i ++)
    {
      if (w < 0) w += 360;
      if (w >= 360) w -= 360;
      qx [i] = radius * cosi [w] + camx + MAXX/2;
      qy [i] = radius * sine [w] + MAXX/2 - camz;
      w += addw [i];
    }
  }
  else
  {
    w += 180;
    for (i = 0; i < 4; i ++)
    {
      if (w >= 360) w -= 360;
      qx [i] = radius * cosi [w] + camx + MAXX/2;
      qy [i] = radius * sine [w] + MAXX/2 - camz;
      w += addw [i];
    }
  }
  for (i = 0; i < 4; i ++)
  {
    if ((int) qx [i] < minx) minx = (int) qx [i];
    if ((int) qx [i] > maxx) maxx = (int) qx [i];
    if ((int) qy [i] < miny) miny = (int) qy [i];
    if ((int) qy [i] > maxy) maxy = (int) qy [i];
  }
  minx -= 20; maxx += 20; miny -= 20; maxy += 20;
*/

  int minx = (int) (camx + MAXX/2 - radius);
  int miny = (int) (MAXX/2 - camz - radius);
  int maxx = (int) (minx + radius * 2);
  int maxy = (int) (miny + radius * 2);

/*  if (minx < 0) minx = 0;
  if (maxx >= MAXX) maxx = MAXX;
  if (miny < 0) miny = 0;
  if (maxy >= MAXX) maxy = MAXX;*/

  space->z1->x = minx - MAXX/2;
  space->z1->y = -MAXX/2;
  space->z1->z = MAXX/2 - maxy;
  space->z2->x = maxx - MAXX/2;
  space->z2->y = MAXX/2;
  space->z2->z = MAXX/2 - miny;

  if (camera == 50)
  {
    minx = 0; maxx = MAXX;
    miny = 0; maxy = MAXX;
  }
//    printf ("\nx=%d, y=%d", minx, miny); fflush (stdout);

  int parts = (int) (view / 13);
  parts *= 2;
  parts ++;

  int mp = parts / 2;
  for (i = 0; i < parts; i ++)
    for (i2 = 0; i2 < parts; i2 ++)
    {
      float d = dist (mp - i, mp - i2);
      detail [i] [i2] = (int) (d * 200.0F / view); // do not use pseudoview
    }

  float dx = (float) (maxx - minx + 1) / parts;
  float dy = (float) (maxy - miny + 1) / parts;

/*  float fx, fy;
  for (i = 0; i < parts; i ++)
    for (i2 = 0; i2 < parts; i2 ++)
    {
      fx = (float) minx + (float) (dx * (0.5 + (float) i2));
      fy = (float) miny + (float) (dy * (0.5 + (float) i));
      float d = dist (camx + MAXX/2 - fx, (float) MAXX/2 - camz - fy);
      detail [i] [i2] = (int) ((d - dx/2) / 8.0);
      if (detail [i] [i2] < 0) detail [i] [i2] = 0;
    }*/

/*    float zoomz2 = 32768.0 * zoomz;
    int step = 1;
    for (i = 0; i < 4; i ++)
      for (i2 = 0; i2 < 4; i2 ++)
      {
    step = detail [i] [i2];
    for (x = minx + (int) (dx * (float) i2); x < minx + (int) (dx * (float) (i2 + 1)); x += step)
    {
      glBegin (GL_QUAD_STRIP);
      for (y = miny + (int) (dy * (float) i); y <= miny + (int) (dy * (float) (i + 1)) + step; y += step)
      {
        int a;
        a = selectColor (x, y);
        glColor3f (mat [a] [0] * nl [x] [y], mat [a] [1] * nl [x] [y], mat [a] [2] * nl [x] [y]);
        glVertex3f (2*hh*x - 1.0, (float)h[x][y]*zoomz - zoomz2, 2*hh*(MAXX-y) - 1.0);
        a = selectColor (x+step, y);
        glColor3f (mat [a] [0] * nl [x+step] [y], mat [a] [1] * nl [x+step] [y], mat [a] [2] * nl [x+step] [y]);
        glVertex3f (2*hh*(x+step) - 1.0, (float)h[x+step][y]*zoomz - zoomz2, 2*hh*(MAXX-y) - 1.0);
      }
      glEnd();
    }
  }*/

//  float cr, cg, cb;
//  int step = 1;

  int zz1 = 0, zz2 = 0, zz = 0;

/*  if (quality == 0)
  {
    minx -= (minx & 1); // modulo 2
    miny -= (miny & 1);
  }*/

/*  if (camera == 50)
  {
    if (quality == 0)
      step = 8;
    else if (quality == 1)
      step = 4;
    else
      step = 2;
  }*/

/*  unsigned char done [400] [400];
  for (x = 0; x < 400; x ++)
  memset (&done [x], 0, 400*sizeof (unsigned char));*/

  if (quality <= 0 || camera == 50)
  {

    drawQuadStrip (minx, miny, maxx, maxy);

//gl->shadeFlat ();
/*
// Quadtrees
    for (x = minx; x < maxx;)
    {
//      y = miny + 20;
      zz = 0;
      for (y = miny; y < maxy;)
      {
    if (!done [x-minx] [y-miny])
//        if (isCubeInFrustum (hh2*(x) - 1.0, (float)h[x][y]*zoomz - zoomz2, hh2*(MAXX-(y)) - 1.0, hh2*2))
        {
        glBegin (GL_QUADS);
      int mystep = rect[x][y];
      int rx = x - (x % mystep);
      int ry = y - (y % mystep);
        zz1 ++;
        cr = r [rx] [ry];
        cg = g [rx] [ry];
        cb = b [rx] [ry];
        float fac = 0.001 * nl [rx] [ry] * sunlight / 256.0;
        cr = (float) cr * fac;
        cg = (float) cg * fac;
        cb = (float) cb * fac;
        if (cr >= 1.0) cr = 1.0;
        if (cg >= 1.0) cg = 1.0;
        if (cb >= 1.0) cb = 1.0;
//        printf ("r=%f, g=%f, b=%f\n", cr, cg, cb); fflush (stdout);
        glColor3f (cr, cg, cb);
        glVertex3f (hh2*rx - 1.0, (float)h[rx][ry]*zoomz - zoomz2, hh2*(MAXX-ry) - 1.0);
        cr = r [rx+mystep] [ry];
        cg = g [rx+mystep] [ry];
        cb = b [rx+mystep] [ry];
        fac = 0.001 * nl [rx+mystep] [ry] * sunlight / 256.0;
        cr = (float) cr * fac;
        cg = (float) cg * fac;
        cb = (float) cb * fac;
        if (cr >= 1.0) cr = 1.0;
        if (cg >= 1.0) cg = 1.0;
        if (cb >= 1.0) cb = 1.0;
        glColor3f (cr, cg, cb);
        glVertex3f (hh2*(rx+mystep) - 1.0, (float)h[rx+mystep][ry]*zoomz - zoomz2, hh2*(MAXX-ry) - 1.0);
        cr = r [rx+mystep] [ry+mystep];
        cg = g [rx+mystep] [ry+mystep];
        cb = b [rx+mystep] [ry+mystep];
        fac = 0.001 * nl [rx+mystep] [ry+mystep] * sunlight / 256.0;
        cr = (float) cr * fac;
        cg = (float) cg * fac;
        cb = (float) cb * fac;
        if (cr >= 1.0) cr = 1.0;
        if (cg >= 1.0) cg = 1.0;
        if (cb >= 1.0) cb = 1.0;
        glColor3f (cr, cg, cb);
        glVertex3f (hh2*(rx+mystep) - 1.0, (float)h[rx+mystep][ry+mystep]*zoomz - zoomz2, hh2*(MAXX-(ry+mystep)) - 1.0);
        cr = r [rx] [ry+mystep];
        cg = g [rx] [ry+mystep];
        cb = b [rx] [ry+mystep];
        fac = 0.001 * nl [rx] [ry+mystep] * sunlight / 256.0;
        cr = (float) cr * fac;
        cg = (float) cg * fac;
        cb = (float) cb * fac;
        if (cr >= 1.0) cr = 1.0;
        if (cg >= 1.0) cg = 1.0;
        if (cb >= 1.0) cb = 1.0;
        glColor3f (cr, cg, cb);
        glVertex3f (hh2*(rx) - 1.0, (float)h[rx][ry+mystep]*zoomz - zoomz2, hh2*(MAXX-(ry+mystep)) - 1.0);
        glEnd();
    for (i=(rx-minx>0?rx-minx:0);i<rx-minx+mystep;i++)
        for (i2=(ry-miny>0?ry-miny:0);i2<ry-miny+mystep;i2++)
      done [i] [i2] = 1;
        }
        else zz2 ++;
        y += step;
        zz ++;
      }
      x += step;
    }*/


/*
// ROAM
    glDisable (GL_CULL_FACE);
    int j, k;

    int trix [3], triy [3];
    float vert [3] [3];

    visibility ++;
    if (visibility == 0xFFFF)
      visibility = 1; // visibility auf 0 setzen !!!

    for (x = minx; x < maxx;)
    {
      zz = 0;
      for (y = miny; y < maxy;)
      {
        if (trimatrix->t [y] [x] != NULL)
        {
          for (j = 0; j < 8; j ++)
            if (trimatrix->t [y] [x] [j] != NULL)
              if (!trimatrix->t [y] [x] [j]->visibility != visibility)
              {
                trimatrix->t [y] [x] [j]->visibility = visibility;
                trix[0]=trimatrix->t [y] [x] [j]->xh;
                triy[0]=trimatrix->t [y] [x] [j]->yh;
                trix[1]=trimatrix->t [y] [x] [j]->x1;
                triy[1]=trimatrix->t [y] [x] [j]->y1;
                trix[2]=trimatrix->t [y] [x] [j]->x2;
                triy[2]=trimatrix->t [y] [x] [j]->y2;
                for (k = 0; k < 3; k ++)
                {
                  vert[k][0]=hh2*trix[k] - 1.0;
                  vert[k][1]=(float)h[trix[k]][triy[k]]*zoomz - zoomz2;
                  vert[k][2]=hh2*(MAXX-triy[k]) - 1.0;
                }

                if (isPointInFrustum (vert[0][0],vert[0][1],vert[0][2]) ||
                    isPointInFrustum (vert[1][0],vert[1][1],vert[1][2]) ||
                    isPointInFrustum (vert[2][0],vert[2][1],vert[2][2]))
                {
                  glBegin (GL_TRIANGLES);
                  zz1 ++;
                  for (k = 0; k < 3; k ++)
                  {
                    cr = r [trix[k]] [triy[k]];
                    cg = g [trix[k]] [triy[k]];
                    cb = b [trix[k]] [triy[k]];
                    float fac = 0.001 * nl [trix[k]] [triy[k]] * sunlight / 256.0;
                    cr = (float) cr * fac;
                    cg = (float) cg * fac;
                    cb = (float) cb * fac;
                    if (cr >= 1.0) cr = 1.0;
                    if (cg >= 1.0) cg = 1.0;
                    if (cb >= 1.0) cb = 1.0;
                    glColor3f (cr, cg, cb);
                    glVertex3f (vert[k][0],vert[k][1],vert[k][2]);
                  }
                  zz ++;
        glEnd();
                }
                else
                  zz2 ++;
              }
        }
        y += step;
      }
      x += step;
    }
*/

  }
  else
  {
//    gl->enableTextures (texgrass);
/*    if (quality >= 5)
    {
      gl->enableLinearTexture (texgrass->textureID);
      gl->enableLinearTexture (texrocks->textureID);
      gl->enableLinearTexture (texwater->textureID);
    }
    else
    {
      gl->disableLinearTexture (texgrass->textureID);
      gl->disableLinearTexture (texrocks->textureID);
      gl->disableLinearTexture (texwater->textureID);
    }*/
//    glDisable (GL_DITHER);


    if (mode == 0)
    {

//      if (quality == 1 || quality == 2) step = 2;
//      else step = 1;
/*int j;

    for (x = minx; x < maxx; x += step)
    {
      for (y = miny; y < maxy; y += step)
      {
        if (trimatrix->t [y] [x] != NULL)
        for (j = 0; j < 8; j ++)
          if (trimatrix->t [y] [x] [j] != NULL)
          {
            trimatrix->t [y] [x] [j]->setDirty ();
          }
      }
    }*/


//    int stepmask = 0xFFFF - step + 1;

//    glDisable (GL_CULL_FACE); // Why ???
      for (i = 0; i < parts; i ++)
        for (i2 = 0; i2 < parts; i2 ++)
        {
          int ax = (minx + (int) (dx * (float) i2));
          int ay = (miny + (int) (dy * (float) i));
          int zx = (minx + (int) (dx * (float) (i2 + 1)));
          int zy = (miny + (int) (dy * (float) (i + 1))/* + gridstep*/);

          if (fargridstep == 2)
          {
            ax -= ax & 1; ay -= ay & 1;
            zx -= zx & 1; zy -= zy & 1;
          }
          else if (fargridstep == 3)
          {
            ax -= ax % 3; ay -= ay % 3;
            zx -= zx % 3; zy -= zy % 3;
          }
          else if (fargridstep == 4)
          {
            ax -= ax & 3; ay -= ay & 3;
            zx -= zx & 3; zy -= zy & 3;
          }

          if (detail [i] [i2] > fardetail)
          {
            zy += fargridstep;
            if (gl->isSphereInFrustum (hh2*(ax+zx)/2 - 1.0, (float)hw[getCoord((ax+zx)/2)][getCoord((ay+zy)/2)]*zoomz - zoomz2, hh2*((float)MAXX-(ay+zy)/2) - 1.0, hh2*(zx-ax)*1.5))
              drawQuadStrip (ax, ay, zx, zy);
          }
          else
          {
            if (detail [i] [i2] <= lineardetail)
            {
              gl->enableLinearTexture (texgrass->textureID);
              gl->enableLinearTexture (texgravel1->textureID);
              gl->enableLinearTexture (texredsand->textureID);
              gl->enableLinearTexture (texrocks->textureID);
              gl->enableLinearTexture (texwater->textureID);
              gl->enableLinearTexture (texredstone->textureID);
            }
            else
            {
              gl->disableLinearTexture (texgrass->textureID);
              gl->disableLinearTexture (texgravel1->textureID);
              gl->disableLinearTexture (texredsand->textureID);
              gl->disableLinearTexture (texrocks->textureID);
              gl->disableLinearTexture (texwater->textureID);
              gl->disableLinearTexture (texredstone->textureID);
            }
            if (detail [i] [i2] <= middetail)
            {
            if (i > 0) // south
            {
              if (detail [i - 1] [i2] > middetail)
              {
                if (fargridstep == 3 * neargridstep)
                {
                  ys = ay;
                  for (xs = ax; xs < zx;)
                  {
                    drawTexturedQuad (xs, ys, xs + 3 * neargridstep, ys, xs + 2 * neargridstep, ys, xs + 1 * neargridstep, ys);
                    xs += fargridstep;
                  }
                }
                else if (fargridstep == 2 * neargridstep)
                {
                  ys = ay;
                  for (xs = ax; xs < zx;)
                  {
                    drawTexturedTriangle (xs, ys, xs + 2 * neargridstep, ys, xs + 1 * neargridstep, ys);
                    xs += fargridstep;
                  }
                }
              }
            }
            if (i < parts - 1) // north
            {
              if (detail [i + 1] [i2] > middetail)
              {
                if (fargridstep == 3 * neargridstep)
                {
                  ys = zy;
                  for (xs = ax; xs < zx;)
                  {
                    drawTexturedQuad (xs, ys, xs + 1 * neargridstep, ys, xs + 2 * neargridstep, ys, xs + 3 * neargridstep, ys);
                    xs += fargridstep;
                  }
                }
                else if (fargridstep == 2 * neargridstep)
                {
                  ys = zy;
                  for (xs = ax; xs < zx;)
                  {
                    drawTexturedTriangle (xs, ys, xs + 1 * neargridstep, ys, xs + 2 * neargridstep, ys);
                    xs += fargridstep;
                  }
                }
              }
            }
            if (i2 > 0) // east
            {
              if (detail [i] [i2 - 1] > middetail)
              {
                if (fargridstep == 3 * neargridstep)
                {
                  xs = ax;
                  for (ys = ay; ys < zy;)
                  {
                    drawTexturedQuad (xs, ys, xs, ys + 1 * neargridstep, xs, ys + 2 * neargridstep, xs, ys + 3 * neargridstep);
                    ys += fargridstep;
                  }
                }
                else if (fargridstep == 2 * neargridstep)
                {
                  xs = ax;
                  for (ys = ay; ys < zy;)
                  {
                    drawTexturedTriangle (xs, ys, xs, ys + 1 * neargridstep, xs, ys + 2 * neargridstep);
                    ys += fargridstep;
                  }
                }
              }
            }
            if (i2 < parts - 1) // west
            {
              if (detail [i] [i2 + 1] > middetail)
              {
                if (fargridstep == 3 * neargridstep)
                {
                  xs = zx;
                  for (ys = ay; ys < zy;)
                  {
                    drawTexturedQuad (xs, ys, xs, ys + 3 * neargridstep, xs, ys + 2 * neargridstep, xs, ys + 1 * neargridstep);
                    ys += fargridstep;
                  }
                }
                else if (fargridstep == 2 * neargridstep)
                {
                  xs = zx;
                  for (ys = ay; ys < zy;)
                  {
                    drawTexturedTriangle (xs, ys, xs, ys + 2 * neargridstep, xs, ys + 1 * neargridstep);
                    ys += fargridstep;
                  }
                }
              }
            }
              gridstep = neargridstep;
            }
            else
            {
              gridstep = fargridstep;
            }
            for (xs = ax; xs < zx;)
            {
              x = getCoord (xs);
              for (ys = ay; ys < zy;)
              {
                y = getCoord (ys);
                zz1 ++;
                int a;
                a = selectColor (x, y);
                // construction #1
/*                int testx = (int) getCoord ((minx + maxx) / 2) - x + 100;
                int testz = (int) getCoord ((miny + maxy) / 2) - y + 100;
                testx /= 2; testz /= 2;
                if (hb [testz] [testx])*/
                if (a != 6 && a != 9)
                {
                  if (drawrule [x] [y] == 0)
                    drawTexturedQuad (xs, ys);
                  else if (drawrule [x] [y] == 2)
                    drawTexturedTriangle1 (xs, ys);
                  else
                    drawTexturedTriangle2 (xs, ys);
                }

                ys += gridstep;
                zz ++;
              }
//glEnd ();
              xs += gridstep;
            }
//    gl->enableAlphaBlending ();
            for (xs = ax; xs < zx;)
            {
              x = getCoord (xs);
              for (ys = ay; ys < zy;)
              {
                y = getCoord (ys);
                zz1 ++;
                int xstep = getCoord (xs + gridstep);
                int ystep = getCoord (ys + gridstep);
        //        int a = 0;
                if (isWater (f [x] [y]) || isWater (f [xstep] [y]) || isWater (f [xstep] [ystep]) || isWater (f [x] [ystep]))
        //          a = selectColor (x, y);
        //        if (a == 4 || a == 5 || a == 6 || a == 8 || a == 9)
                {
                  drawWaterTexturedQuad (xs, ys);
                }
                ys += gridstep;
                zz ++;
              }
        //glEnd ();
              xs += gridstep;
            }
//  gl->disableAlphaBlending ();
          }

        } // for i2

    } // if mode

/*
    if (mode == 1) // ROAM mode, currently not used
    {

      int j, k;

      int trix [3], triy [3];
      float vert [3] [3];

//  glEnable (GL_NORMALIZE);
//  glDisable (GL_CULL_FACE);

      visibility ++;
      if (visibility == 0xFFFF)
        visibility = 1; // visibility auf 0 setzen !!!

//    int tx, ty;
      float tfx, tfy; //, tfinc;
      float texlight = 1.0;
      bool texture = false;
      float col [4] [3];
      float pos [4] [3];
      float tf [4] [2];
      for (i = 0; i < PARTS; i ++)
        for (i2 = 0; i2 < PARTS; i2 ++)
        {
          for (x = minx + (int) (dx * (float) i2); x < minx + (int) (dx * (float) (i2 + 1));)
          {
            for (y = miny + (int) (dy * (float) i); y <= miny + (int) (dy * (float) (i + 1)) + step;)
            {
              if (trimatrix->t [y] [x] != NULL)
              {
                for (j = 0; j < 8; j ++)
                  if (trimatrix->t [y] [x] [j] != NULL)
                    if (!trimatrix->t [y] [x] [j]->visibility != visibility)
                    {
                      trimatrix->t [y] [x] [j]->visibility = visibility;
                      trix[0]=trimatrix->t [y] [x] [j]->xh;
                      triy[0]=trimatrix->t [y] [x] [j]->yh;
                      trix[1]=trimatrix->t [y] [x] [j]->x1;
                      triy[1]=trimatrix->t [y] [x] [j]->y1;
                      trix[2]=trimatrix->t [y] [x] [j]->x2;
                      triy[2]=trimatrix->t [y] [x] [j]->y2;
                      for (k = 0; k < 3; k ++)
                      {
                        vert[k][0]=hh2*trix[k] - 1.0;
                        vert[k][1]=(float)h[trix[k]][triy[k]]*zoomz - zoomz2;
                        vert[k][2]=hh2*(MAXX-triy[k]) - 1.0;
                      }

                      if (gl->isPointInFrustum (vert[0][0],vert[0][1],vert[0][2]) ||
                          gl->isPointInFrustum (vert[1][0],vert[1][1],vert[1][2]) ||
                          gl->isPointInFrustum (vert[2][0],vert[2][1],vert[2][2]))
                      {
  //                      trimatrix->t [y] [x] [j]->setClean ();
  //      glBegin (GL_TRIANGLES);
                        int mx, my;
                        mx = trimatrix->t [y] [x] [j]->xh; my = trimatrix->t [y] [x] [j]->yh;
                        zz1 ++;
                        int a;
                        a = selectColor (mx, my);
                        if (detail [i] [i2] <= quality + 1)
                        {
                          texture = true;
                          texlight = 1.0;
                          if (a == 0 || a == 1)
                            gl->enableTextures (texgrass->textureID);
                          else if (a == 2 || a == 7)
                            gl->enableTextures (texrocks->textureID);
                          else if (a == 4 || a == 5 || a == 6)
                          {
                            int a2 = selectColor (mx, my);
                            if (a2 < 4 || a2 > 6) gl->enableTextures (texgrass->textureID);
                            else
                            {
                              gl->enableTextures (texwater->textureID);
  //  else glDisable (GL_TEXTURE_2D);
                            }
                          }
                          else if (a == 11)
                            gl->enableTextures (texredstone->textureID);
                          else
                            glDisable (GL_TEXTURE_2D);
                        }
                        else
                        {
                          texlight = 0.9;
                          texture = false;
                          glDisable (GL_TEXTURE_2D);
                        }
                      
                        for (k = 0; k < 3; k ++)
                        {
                          if (a > 1)
                          {
                            tfx = 0.5 * trix[k]; tfy = 0.5 * triy[k];
                          }
                          else
                          {
                            tfx = 0.25 * trix[k]; tfy = 0.25 * triy[k];
                          }
                          float fac2 = 0.001 * sunlight / 256.0 * texlight;
                          col [k] [0] = r [trix[k]] [triy[k]];
                          col [k] [1] = g [trix[k]] [triy[k]];
                          col [k] [2] = b [trix[k]] [triy[k]];
                          fac = fac2 * (nl [trix[k]] [triy[k]] + (short) dl [trix[k]] [triy[k]]);
                          col [k] [0] *= fac;
                          col [k] [1] *= fac;
                          col [k] [2] *= fac;
                          if (col [k] [0] >= 1.0) col [k] [0] = 1.0;
                          if (col [k] [1] >= 1.0) col [k] [1] = 1.0;
                          if (col [k] [2] >= 1.0) col [k] [2] = 1.0;
                          pos [k] [0] = hh2*trix[k] - 1.0;
                          pos [k] [1] = (float)h[trix[k]][triy[k]]*zoomz - zoomz2;
                          pos [k] [2] = hh2*(MAXX-triy[k]) - 1.0;
                          tf [k] [0] = tfx;
                          tf [k] [1] = tfy;
                        }
                    
                        glBegin (GL_TRIANGLES);
                        for (int j = 0; j < 3; j ++)
                        {
                          if (texture)
                            glTexCoord2fv (tf [j]);
                          glColor3fv (col [j]);
                          glVertex3fv (pos [j]);
                        }
                        glEnd();

                      }
                      else
                        zz2 ++;
                    } // if infrustum
                zz ++;
              } // if trimatrix
              y += step;
            }
  //glEnd ();
            x += step;
          }
        }

    } // end of ROAM
*/
  }

//  if (quality == 2) step = 1;
  int treestep = 2;
  if (quality >= 2) treestep = 1;

// Draw trees, bushes
  if (quality >= 1)
  {
    glPushMatrix ();
//  glRotatef (camphi, 0, 1, 0);
    glDisable (GL_CULL_FACE);
    if (quality >= 6)
    {
      gl->enableAlphaBlending ();
      glEnable (GL_ALPHA_TEST);
      glAlphaFunc (GL_GEQUAL, 0.1);
    }
    else
    {
      glEnable (GL_ALPHA_TEST);
      glAlphaFunc (GL_GEQUAL, 0.5);
    }
/*      if (quality >= 5)
      {
        gl->enableLinearTexture (textree->textureID);
        gl->enableLinearTexture (textree2->textureID);
      }
      else
      {
        gl->disableLinearTexture (textree->textureID);
        gl->disableLinearTexture (textree2->textureID);
      }*/
    gl->enableTextures (textree->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableTextures (textree2->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableTextures (textree3->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableTextures (textree4->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    gl->enableTextures (texcactus1->textureID);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    float mydep = 1000;
    if (quality == 2) mydep = 1800;
    else if (quality == 3) mydep = 2500;
    else if (quality == 4) mydep = 3200;
    else if (quality == 5) mydep = 3800;
    if (mydep > view * view) mydep = view * view;
    int cutdep = 1500;

    int lineartree = -1;
    if (quality >= 3) lineartree = 0;

    float x21 = 0.3, y21 = -0.32;
    float x22 = -0.21, y22 = 0.22;
    float x31 = 0.32, y31 = -0.08;
    float x32 = -0.31, y32 = -0.33;
    float x33 = -0.11, y33 = 0.28;
    float x41 = 0.22, y41 = -0.3;
    float x42 = -0.28, y42 = -0.18;
    float x43 = 0.31, y43 = 0.17;
    float x44 = -0.21, y44 = 0.32;
    for (i = 0; i < parts; i ++)
      for (i2 = 0; i2 < parts; i2 ++)
      {
        if (detail [i] [i2] > middetail)
          gridstep = fargridstep;
        else
          gridstep = neargridstep;
        if (detail [i] [i2] <= lineartree)
        {
          gl->enableLinearTexture (textree->textureID);
          gl->enableLinearTexture (textree2->textureID);
          gl->enableLinearTexture (textree3->textureID);
          gl->enableLinearTexture (textree4->textureID);
          gl->enableLinearTexture (texcactus1->textureID);
          gl->enableLinearTexture (textreeu->textureID);
          gl->enableLinearTexture (textreeu2->textureID);
          gl->enableLinearTexture (textreeu3->textureID);
          gl->enableLinearTexture (textreeu4->textureID);
          gl->enableLinearTexture (texcactusu1->textureID);
        }
        else
        {
          gl->disableLinearTexture (textree->textureID);
          gl->disableLinearTexture (textree2->textureID);
          gl->disableLinearTexture (textree3->textureID);
          gl->disableLinearTexture (textree4->textureID);
          gl->disableLinearTexture (texcactus1->textureID);
          gl->disableLinearTexture (textreeu->textureID);
          gl->disableLinearTexture (textreeu2->textureID);
          gl->disableLinearTexture (textreeu3->textureID);
          gl->disableLinearTexture (textreeu4->textureID);
          gl->disableLinearTexture (texcactusu1->textureID);
        }
        int ax = minx + (int) (dx * (float) i2);
        int ay = miny + (int) (dy * (float) i);
        int ex = minx + (int) (dx * (float) (i2 + 1));
        int ey = miny + (int) (dy * (float) (i + 1)) + treestep;
        if (treestep == 2)
        {
          ax -= ax & 1; ay -= ay & 1;
        }
        for (xs = ax; xs < ex;)
//      for (x = maxx; x >= minx;)
        {
          x = getCoord (xs);
          zz = 0;
          for (ys = ay; ys <= ey;)
//        for (y = maxy; y >= miny;)
          {
            y = getCoord (ys);
//            if (detail [i] [i2] <= fardetail + 1)
            float tdx = camx + MAXX/2 - xs;
            float tdy = MAXX/2 - camz - ys;
            float dep = tdx * tdx + tdy * tdy;
            if (dep < mydep)
              if (isWoods (f [x] [y]) || isType (f [x] [y], REDTREE0) || isType (f [x] [y], CACTUS0))
                if (gl->isSphereInFrustum (hh2*(xs) - 1.0, (float)h[x][y]*zoomz - zoomz2, hh2*(MAXX-(ys)) - 1.0, hh2*2))
                {
                  float cg = g [x] [y];
                  fac = 0.0007 * (nl [x] [y] + (short) dl [x] [y] * 16) * sunlight;
                  cg = (float) cg * fac;
                  if (cg >= 256.0) cg = 255.0;
                  treecolor.c [0] = treecolor.c [1] = treecolor.c [2] = (int) cg;
                  if (f [x] [y] == CONIFEROUSWOODS0 || f [x] [y] == BUSHES0)
                  {
                    texturetree1 = textree2->textureID;
                    texturetree2 = textreeu2->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x41, ys + y41, 0.0036, 0.23, phi);
                    drawTree (xs + x42, ys + y42, 0.0040, 0.25, phi);
                    drawTree (xs + x43, ys + y43, 0.0041, 0.26, phi);
                    drawTree (xs + x44, ys + y44, 0.0034, 0.22, phi);
                  }
                  else if (f [x] [y] == CONIFEROUSWOODS1 || f [x] [y] == BUSHES1)
                  {
                    texturetree1 = textree2->textureID;
                    texturetree2 = textreeu2->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x31, ys + y31, 0.0036, 0.23, phi);
                    drawTree (xs + x32, ys + y32, 0.0041, 0.25, phi);
                    drawTree (xs + x33, ys + y33, 0.003, 0.18, phi);
                  }
                  else if (f [x] [y] == CONIFEROUSWOODS2 || f [x] [y] == BUSHES2)
                  {
                    texturetree1 = textree2->textureID;
                    texturetree2 = textreeu2->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x21, ys + y21, 0.0032, 0.22, phi);
                    drawTree (xs + x22, ys + y22, 0.0045, 0.27, phi);
                  }
                  else if (f [x] [y] == CONIFEROUSWOODS3 || f [x] [y] == BUSHES3)
                  {
                    texturetree1 = textree2->textureID;
                    texturetree2 = textreeu2->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs, ys, 0.0047, 0.3, phi);
                  }
                  else if (f [x] [y] == DECIDUOUSWOODS0)
                  {
                    texturetree1 = textree->textureID;
                    texturetree2 = textreeu->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x41, ys + y41, 0.004, 0.23, phi);
                    drawTree (xs + x42, ys + y42, 0.0044, 0.27, phi);
                    drawTree (xs + x43, ys + y43, 0.0045, 0.25, phi);
                    drawTree (xs + x44, ys + y44, 0.0038, 0.22, phi);
                  }
                  else if (f [x] [y] == DECIDUOUSWOODS1)
                  {
                    texturetree1 = textree->textureID;
                    texturetree2 = textreeu->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x31, ys + y31, 0.004, 0.23, phi);
                    drawTree (xs + x32, ys + y32, 0.0045, 0.25, phi);
                    drawTree (xs + x33, ys + y33, 0.003, 0.18, phi);
                  }
                  else if (f [x] [y] == DECIDUOUSWOODS2)
                  {
                    texturetree1 = textree->textureID;
                    texturetree2 = textreeu->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x21, ys + y21, 0.0035, 0.22, phi);
                    drawTree (xs + x22, ys + y22, 0.0048, 0.27, phi);
                  }
                  else if (f [x] [y] == DECIDUOUSWOODS3)
                  {
                    texturetree1 = textree->textureID;
                    texturetree2 = textreeu->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs, ys, 0.005, 0.3, phi);
                  }
                  else if (f [x] [y] == DWARFPINES0)
                  {
                    texturetree1 = textree3->textureID;
                    texturetree2 = textreeu3->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x41, ys + y41, 0.002, 0.28, phi);
                    drawTree (xs + x42, ys + y42, 0.0023, 0.34, phi);
                    drawTree (xs + x43, ys + y43, 0.0025, 0.37, phi);
                    drawTree (xs + x44, ys + y44, 0.0018, 0.26, phi);
                  }
                  else if (f [x] [y] == DWARFPINES1)
                  {
                    texturetree1 = textree3->textureID;
                    texturetree2 = textreeu3->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x31, ys + y31, 0.002, 0.28, phi);
                    drawTree (xs + x32, ys + y32, 0.0025, 0.35, phi);
                    drawTree (xs + x33, ys + y33, 0.0015, 0.2, phi);
                  }
                  else if (f [x] [y] == DWARFPINES2)
                  {
                    texturetree1 = textree3->textureID;
                    texturetree2 = textreeu3->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x21, ys + y21, 0.0018, 0.26, phi);
                    drawTree (xs + x22, ys + y22, 0.0024, 0.37, phi);
                  }
                  else if (f [x] [y] == DWARFPINES3)
                  {
                    texturetree1 = textree3->textureID;
                    texturetree2 = textreeu3->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs, ys, 0.0027, 0.38, phi);
                  }
                  else if (f [x] [y] == MIXEDWOODS0)
                  {
                    texturetree1 = textree2->textureID;
                    texturetree2 = textreeu2->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x41, ys + y41, 0.004, 0.26, phi);
                    drawTree (xs + x42, ys + y42, 0.0041, 0.29, phi);
                    texturetree1 = textree->textureID;
                    texturetree2 = textreeu->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x43, ys + y43, 0.0045, 0.26, phi);
                    drawTree (xs + x44, ys + y44, 0.0038, 0.24, phi);
                  }
                  else if (f [x] [y] == MIXEDWOODS1)
                  {
                    texturetree1 = textree2->textureID;
                    texturetree2 = textreeu2->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x31, ys + y31, 0.004, 0.27, phi);
                    texturetree1 = textree->textureID;
                    texturetree2 = textreeu->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x32, ys + y32, 0.0045, 0.25, phi);
                    drawTree (xs + x33, ys + y33, 0.003, 0.18, phi);
                  }
                  else if (f [x] [y] == MIXEDWOODS2)
                  {
                    texturetree1 = textree2->textureID;
                    texturetree2 = textreeu2->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x21, ys + y21, 0.0035, 0.25, phi);
                    texturetree1 = textree->textureID;
                    texturetree2 = textreeu->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs + x22, ys + y22, 0.0048, 0.27, phi);
                  }
                  else if (f [x] [y] == MIXEDWOODS3)
                  {
                    texturetree1 = textree->textureID;
                    texturetree2 = textreeu->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs, ys, 0.005, 0.3, phi);
                  }
                  else if (f [x] [y] == REDTREE0)
                  {
                    texturetree1 = textree4->textureID;
                    texturetree2 = textreeu4->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs, ys, 0.004, 0.35, phi);
                  }
                  else if (f [x] [y] == CACTUS0)
                  {
                    texturetree1 = texcactus1->textureID;
                    texturetree2 = texcactusu1->textureID;
                    if (dep > cutdep) texturetree2 = -1;
                    drawTree (xs, ys, 0.004, 0.3, phi);
                  }
                }
            ys += treestep;
          } // ys for
          xs += treestep;
        } // xs for
      }

    glDisable (GL_ALPHA_TEST);
    glPopMatrix ();
    gl->disableAlphaBlending ();

  }

  glDisable (GL_TEXTURE_2D);

//    } // mode

//    }
    
  glPopMatrix ();
//    printf ("\n%d, %d + %d", zz, zz1, zz2);

  gridstep = neargridstep; // set to finer grid for ground collision detection
}

GLLandscape::GLLandscape (Space *space2, int type, int *heightmask)
{
  int i, i2;
  lsticker = 0;
  space = space2;
  if (!multiplayer || isserver)
  {
    if (type == LANDSCAPE_ALPINE || type == LANDSCAPE_ALPINE_NOLAKE || type == LANDSCAPE_LOW_ALPINE)
    {
      if (type == 0 || type == 1)
      {
        genSurface (60, heightmask);
        genRocks (30, 40);
      }
      else
      {
        genSurface (40, heightmask);
        genRocks (30, 10);
      }
      if (type == 0 || type == 2)
      {
        int lakes = myrandom (20) + 20;
        genLake (lakes);
        genLake (lakes / 3);
        genLake (lakes / 4);
        genLake (lakes / 4);
        genLake (2);
        genLake (2);
        genLake (2);
        genLake (2);
        genLake (2);
      }
      calcWoods (150);
    }
    else if (type == LANDSCAPE_ALPINE_EROSION)
    {
      genErosionSurface (50, heightmask);
      genRocks (30, 25);
      calcWoods (150);
    }
    else if (type == LANDSCAPE_ALPINE_SEA)
    {
      genSurface (60, heightmask);
      genRocks (30, 80);
      int diff = lowestpoint + (highestpoint - lowestpoint) * 3 / 4;
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          if (h [i] [i2] < diff)
          {
            hw [i] [i2] = diff;
            if (diff - h [i] [i2] < 1000)
              f [i] [i2] = SHALLOWWATER;
            else
              f [i] [i2] = DEEPWATER;
          }
        }
    }
    else if (type == LANDSCAPE_SEA)
    {
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          f [i] [i2] = DEEPWATER;
          h [i] [i2] = 10000;
          hw [i] [i2] = 30000;
        }
    }
    else if (type == LANDSCAPE_MOON)
    {
      genMoonSurface (60);
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          f [i] [i2] = MOONSAND;
        }
    }
    else if (type == LANDSCAPE_FLAT_MOON)
    {
      genMoonSurface (30);
      for (i = 0; i <= MAXX; i ++)
        for (i2 = 0; i2 <= MAXX; i2 ++)
        {
          f [i] [i2] = MOONSAND;
        }
    }
    else if (type == LANDSCAPE_CANYON)
    {
      genCanyonSurface (120);
    }
    else if (type == LANDSCAPE_DESERT)
    {
      genDesertSurface (20);
    }
  }
#ifdef HAVE_SDL_NET
  if (isserver)
  {
// Send map data to all clients
    char buf [10];
    for (i = 0; i <= MAXX; i ++)
    {
    printf (" %d ", i);
      server->sendMessage (1, (char *) h [i], (MAXX + 1) * 2);
      server->sendMessage (1, (char *) hw [i], (MAXX + 1) * 2);
      server->sendMessage (1, (char *) f [i], (MAXX + 1) * 1);
      while (!server->getMessage (1, buf)) ;
    }
  }
  if (multiplayer && !isserver)
  {
//    for (;;)
//    {
    for (i = 0; i <= MAXX; i ++)
    {
//    printf (" %d ", i);
      while (!client->getMessage ((char *) h [i])) ;
      while (!client->getMessage ((char *) hw [i])) ;
      while (!client->getMessage ((char *) f [i])) ;
      client->sendMessage (".", 1);
    }
  
  }
#endif
  hh = (float) 1 / (float) MAXX;
  zoomz2 = 32768.0 * zoomz;
  hh2 = 2.0*hh;
  lv [0] = 0.0; lv [1] = 1.0; lv [2] = 1.0;
  mat [0] [0] = 0.4; mat [0] [1] = 0.8; mat [0] [2] = 0.3; mat [0] [3] = 1.0;
  mat [1] [0] = 0.3; mat [1] [1] = 0.55; mat [1] [2] = 0.2; mat [1] [3] = 1.0;
/*  mat [0] [0] = 0.4; mat [0] [1] = 0.8; mat [0] [2] = 0.3; mat [0] [3] = 1.0;
  mat [1] [0] = 0.3; mat [1] [1] = 0.5; mat [1] [2] = 0.2; mat [1] [3] = 1.0;*/
/*  mat [0] [0] = 0.3; mat [0] [1] = 1.0; mat [0] [2] = 0.3; mat [0] [3] = 1.0;
  mat [1] [0] = 0.5; mat [1] [1] = 0.85; mat [1] [2] = 0.2; mat [1] [3] = 1.0;*/
  mat [2] [0] = 0.7; mat [2] [1] = 0.7; mat [2] [2] = 0.7; mat [2] [3] = 1.0;
  mat [3] [0] = 1.0; mat [3] [1] = 1.0; mat [3] [2] = 1.0; mat [3] [3] = 1.0;
  mat [4] [0] = 0.25; mat [4] [1] = 1.0; mat [4] [2] = 0.25; mat [4] [3] = 1.0;
  mat [5] [0] = 0.2; mat [5] [1] = 1.0; mat [5] [2] = 0.2; mat [5] [3] = 1.0;
  mat [6] [0] = 0.1; mat [6] [1] = 0.25; mat [6] [2] = 1.0; mat [6] [3] = 1.0;
  mat [7] [0] = 0.5; mat [7] [1] = 0.5; mat [7] [2] = 0.5; mat [7] [3] = 1.0;
  mat [8] [0] = 0.3; mat [8] [1] = 1.0; mat [8] [2] = 0.3; mat [8] [3] = 1.0;
  mat [9] [0] = 0.1; mat [9] [1] = 0.15; mat [9] [2] = 1.0; mat [9] [3] = 1.0;
  mat [10] [0] = 0.8; mat [10] [1] = 0.8; mat [10] [2] = 0.8; mat [10] [3] = 1.0;
  mat [11] [0] = 0.95; mat [11] [1] = 0.6; mat [11] [2] = 0.4; mat [11] [3] = 1.0;
  mat [12] [0] = 0.9; mat [12] [1] = 0.75; mat [12] [2] = 0.55; mat [12] [3] = 1.0;
  mat [13] [0] = 1.0; mat [13] [1] = 0.76; mat [13] [2] = 0.35; mat [13] [3] = 1.0;
  mat [14] [0] = 0.7; mat [14] [1] = 0.7; mat [14] [2] = 0.65; mat [14] [3] = 1.0;
  mat [15] [0] = 0.75; mat [15] [1] = 0.78; mat [15] [2] = 0.68; mat [15] [3] = 1.0;
/*  for (i = 0; i < 7; i ++)
    for (i2 = 0; i2 < 4; i2 ++)
      mata [i] [i2] = mat [i] [i2] / 2.0;*/
  for (i = 0; i <= MAXX; i ++)
    for (i2 = 0; i2 <= MAXX; i2 ++)
      if (hw [i] [i2] == 0)
      {
        hw [i] [i2] = h [i] [i2];
      }
  precalculate ();
}

void GLLandscape::calcDynamicLight (CExplosion **explo, DynamicObj **cannon, DynamicObj **missile, DynamicObj **flare)
{
  int i, x, y;
  memset (dl, 0, (MAXX + 1) * (MAXX + 1));
  for (i = 0; i < maxexplosion; i ++)
  {
    if (explo [i]->ttl > 0)
    {
      int mx = MAXX / 2 + (int) explo [i]->tl->x;
      int mz = MAXX / 2 - (int) explo [i]->tl->z;
      float h = explo [i]->tl->y - getHeight (explo [i]->tl->x, explo [i]->tl->z);
      if (h < 0) h = 0;
      float radius = h / 2 + 3;
      if (h < 50)
      {
        float intens = 100.0 - 2 * h;
        for (x = mx - (int) radius; x <= mx + (int) radius; x ++)
          for (y = mz - (int) radius; y <= mz + (int) radius; y ++)
          {
            int xn = getCoord (x);
            int yn = getCoord (y);
            int dx = x - mx, dy = y - mz;
            float dist = sqrt (dx*dx + dy*dy);
            if (dist < radius)
            {
              int light = (int) ((radius - dist) * intens / radius * explo [i]->zoom) + dl [xn] [yn];
              if (light > 255) light = 255;
              dl [xn] [yn] = light;
            }
          }
      }
    }
  }
  if (!day)
    for (i = 0; i < maxlaser; i ++)
    {
      if (cannon [i]->draw && cannon [i]->active)
      {
        int mx = MAXX / 2 + (int) cannon [i]->tl->x;
        int mz = MAXX / 2 - (int) cannon [i]->tl->z;
        float h = cannon [i]->tl->y - getHeight (cannon [i]->tl->x, cannon [i]->tl->z);
        if (h < 0) h = 0;
        float radius = h / 2 + 3;
        if (h < 10)
        {
          float intens = 50.0 - 5 * h;
          for (x = mx - (int) radius; x <= mx + (int) radius; x ++)
            for (y = mz - (int) radius; y <= mz + (int) radius; y ++)
            {
              int xn = getCoord (x);
              int yn = getCoord (y);
              int dx = x - mx, dy = y - mz;
              float dist = sqrt (dx*dx + dy*dy);
              if (dist < radius)
              {
                int light = (int) ((radius - dist) * intens / radius * cannon [i]->zoom) + dl [xn] [yn];
                if (light > 255) light = 255;
                dl [xn] [yn] = light;
              }
            }
        }
      }
    }
  for (i = 0; i < maxmissile; i ++)
  {
    if (missile [i]->draw && missile [i]->active)
    {
      int mx = MAXX / 2 + (int) missile [i]->tl->x;
      int mz = MAXX / 2 - (int) missile [i]->tl->z;
      float h = missile [i]->tl->y - getHeight (missile [i]->tl->x, missile [i]->tl->z);
      if (h < 0) h = 0;
      float radius = h / 2 + 3;
      if (h < 10)
      {
        float intens = 100.0 - 10 * h;
        for (x = mx - (int) radius; x <= mx + (int) radius; x ++)
          for (y = mz - (int) radius; y <= mz + (int) radius; y ++)
          {
            int xn = getCoord (x);
            int yn = getCoord (y);
            int dx = x - mx, dy = y - mz;
            float dist = sqrt (dx*dx + dy*dy);
            if (dist < radius)
            {
              int light = (int) ((radius - dist) * intens / radius * missile [i]->zoom) + dl [xn] [yn];
              if (light > 255) light = 255;
              dl [xn] [yn] = light;
            }
          }
      }
    }
  }
  float flarezoom = 0.2F;
  for (i = 0; i < maxflare; i ++)
  {
    if (flare [i]->draw && flare [i]->active)
    {
      int mx = MAXX / 2 + (int) flare [i]->tl->x;
      int mz = MAXX / 2 - (int) flare [i]->tl->z;
      float h = flare [i]->tl->y - getHeight (flare [i]->tl->x, flare [i]->tl->z);
      if (h < 0) h = 0;
      float radius = h / 2 + 3;
      if (h < 50)
      {
        float intens = 200.0 - 2 * h;
        for (x = mx - (int) radius; x <= mx + (int) radius; x ++)
          for (y = mz - (int) radius; y <= mz + (int) radius; y ++)
          {
            int xn = getCoord (x);
            int yn = getCoord (y);
            int dx = x - mx, dy = y - mz;
            float dist = sqrt (dx*dx + dy*dy);
            if (dist < radius)
            {
              int light = (int) ((radius - dist) * intens / radius * flarezoom) + dl [xn] [yn];
              if (light > 255) light = 255;
              dl [xn] [yn] = light;
            }
          }
      }
    }
  }
}

#endif
