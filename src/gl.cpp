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

/* This file includes a collection of GL features. */

#ifndef IS_GL_H

#include "gl.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

// APPLE path is different (thanks Minami)
#ifdef __APPLE__
  #include <GLUT/glut.h>
  #include "SDL/SDL.h"
  #include "SDL/SDL_thread.h"
  #ifdef HAVE_SDL_MIXER
    #include "SDL/SDL_mixer.h"
  #endif
  #ifdef HAVE_SDL_NET
    #include "SDL/SDL_net.h"
  #endif
#endif
#ifndef __APPLE__
  #ifdef USE_GLUT
    #include <GL/glut.h>
  #else
    #include <GL/glut.h>
    #include "SDL/SDL.h"
    #include "SDL/SDL_thread.h"
    #ifdef HAVE_SDL_MIXER
      #include "SDL/SDL_mixer.h"
    #endif
    #ifdef HAVE_SDL_NET
      #include "SDL/SDL_net.h"
    #endif
  #endif
#endif

/****************************************************************************
OpenGL
****************************************************************************/

GlPrimitives::GlPrimitives ()
{
  numTextures = -1; // textures start at 0
  numLists = 0; // display lists start at 1
  setFogColor (127, 127, 127);
  fogLuminance = 1.0;
}

GlPrimitives::~GlPrimitives ()
{
}

void GlPrimitives::clearBuffers () const
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlPrimitives::swapBuffers () const
{
#ifdef USE_GLUT
  glutSwapBuffers();
#else
  SDL_GL_SwapBuffers ();
#endif
}

void GlPrimitives::rotate (float x, float y, float z)
{
  glRotatef (z, 0.0f, 0.0f, 1.0f);
  glRotatef (y, 0.0f, 1.0f, 0.0f);
  glRotatef (x, 1.0f, 0.0f, 0.0f);
}

void GlPrimitives::callList (int list) const
{
  glCallList (list);
}

int GlPrimitives::createList ()
{
  numLists ++;
  glNewList (numLists, GL_COMPILE_AND_EXECUTE);
  return numLists;
}

void GlPrimitives::enableLinearTexture (int texnum, bool mipmap) const
{
  glBindTexture (GL_TEXTURE_2D, texnum);
  if (!mipmap)
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  else
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void GlPrimitives::disableLinearTexture (int texnum, bool mipmap) const
{
  glBindTexture (GL_TEXTURE_2D, texnum);
  if (!mipmap)
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  else
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

int GlPrimitives::registerTexture (const std::string &name, const unsigned char *data,
                         int width, int height, bool mipmap)
{
  int i;

  numTextures ++;
  for (i = 0; i < numTextures; i ++)
    if (!texList [i].compare (name))
      return i;
  texList [i] = std::string (name);

  glBindTexture (GL_TEXTURE_2D, numTextures);
//  if (!antialiasing) disableLinearTexture (numTextures, mipmap);
//  else enableLinearTexture (numTextures, mipmap);

  if (!mipmap)
    glTexImage2D (GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  else
    gluBuild2DMipmaps (GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

  return numTextures;
}

void GlPrimitives::enableAntiAliasing () const
{
  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_POINT_SMOOTH);
  glEnable (GL_POLYGON_SMOOTH);
  glHint (GL_LINE_SMOOTH_HINT, GL_FASTEST);
  glHint (GL_POINT_SMOOTH_HINT, GL_FASTEST);
  glHint (GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
}

void GlPrimitives::disableAntiAliasing () const
{
  glDisable (GL_LINE_SMOOTH);
  glDisable (GL_POINT_SMOOTH);
  glDisable (GL_POLYGON_SMOOTH);
}

void GlPrimitives::enableAlphaBlending () const
{
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GlPrimitives::disableAlphaBlending () const
{
  glDisable (GL_BLEND);
}

void GlPrimitives::enableAlphaTesting (float intensity) const
{
  glEnable (GL_ALPHA_TEST);
  glAlphaFunc (GL_GEQUAL, intensity);
}

void GlPrimitives::disableAlphaTesting () const
{
  glDisable (GL_ALPHA_TEST);
}

void GlPrimitives::enableTexture (int num) const
{
  glBindTexture (GL_TEXTURE_2D, num);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); /* GL_CLAMP or GL_REPEAT? */
  glEnable (GL_TEXTURE_2D);
}

void GlPrimitives::enableLighting () const
{
  glEnable (GL_LIGHTING);
}

void GlPrimitives::disableLighting () const
{
  glDisable (GL_LIGHTING);
}

void GlPrimitives::enableZBuffer () const
{
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);
}

void GlPrimitives::disableZBuffer () const
{
  glDisable (GL_DEPTH_TEST);
}

void GlPrimitives::shadeFlat () const
{
  glShadeModel (GL_FLAT);
}

void GlPrimitives::shadeSmooth () const
{
  glShadeModel (GL_SMOOTH);
}

void GlPrimitives::enableFog (float view, bool fast) const
{
  float fcol [3];
  fcol [0] = fogColor [0] * fogLuminance / 256.0;
  fcol [1] = fogColor [1] * fogLuminance / 256.0;
  fcol [2] = fogColor [2] * fogLuminance / 256.0;
  glEnable (GL_FOG);
  glFogfv (GL_FOG_COLOR, fcol);
  glFogf (GL_FOG_DENSITY, 0.1);
  glFogi (GL_FOG_MODE, GL_LINEAR);
// possible alternatives:
//  glFogf (GL_FOG_DENSITY, 0.017 * 100 / view);
//  glFogi (GL_FOG_MODE, GL_EXP2);
  if (fast)
    glHint (GL_FOG_HINT, GL_FASTEST);
  else
    glHint (GL_FOG_HINT, GL_NICEST);
  glFogf (GL_FOG_START, 1.0);
  glFogf (GL_FOG_END, view);
}

void GlPrimitives::setFogColor (int r, int g, int b)
{
  fogColor [0] = (unsigned char) r;
  fogColor [1] = (unsigned char) g;
  fogColor [2] = (unsigned char) b;
}



// Frustum Culling according to OpenGL Page
void GlPrimitives::extractFrustum()
{
  float proj[16];
  float modl[16];
  float clip[16];
  float t;

  // extract transformation matrices
  glGetFloatv( GL_PROJECTION_MATRIX, proj );
  glGetFloatv( GL_MODELVIEW_MATRIX, modl );

  clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
  clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
  clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
  clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];
  clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
  clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
  clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
  clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];
  clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
  clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
  clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
  clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

  clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
  clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
  clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
  clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];

  // extract numbers for the RIGHT plane
  frustum[0][0] = clip[3] - clip[0];
  frustum[0][1] = clip[7] - clip[4];
  frustum[0][2] = clip[11] - clip[8];
  frustum[0][3] = clip[15] - clip[12];

  // normalize the result
  t = sqrt(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2]);
  frustum[0][0] /= t;
  frustum[0][1] /= t;
  frustum[0][2] /= t;
  frustum[0][3] /= t;

  // extract the numbers for the LEFT plane
  frustum[1][0] = clip[3] + clip[0];
  frustum[1][1] = clip[7] + clip[4];
  frustum[1][2] = clip[11] + clip[8];
  frustum[1][3] = clip[15] + clip[12];

  // normalize the result
  t = sqrt(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2]);
  frustum[1][0] /= t;
  frustum[1][1] /= t;
  frustum[1][2] /= t;
  frustum[1][3] /= t;

  // extract the BOTTOM plane
  frustum[2][0] = clip[3] + clip[1];
  frustum[2][1] = clip[7] + clip[5];
  frustum[2][2] = clip[11] + clip[9];
  frustum[2][3] = clip[15] + clip[13];

  // normalize the result
  t = sqrt(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2]);
  frustum[2][0] /= t;
  frustum[2][1] /= t;
  frustum[2][2] /= t;
  frustum[2][3] /= t;

  // extract the TOP plane
  frustum[3][0] = clip[3] - clip[1];
  frustum[3][1] = clip[7] - clip[5];
  frustum[3][2] = clip[11] - clip[9];
  frustum[3][3] = clip[15] - clip[13];

  // normalize the result
  t = sqrt(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2]);
  frustum[3][0] /= t;
  frustum[3][1] /= t;
  frustum[3][2] /= t;
  frustum[3][3] /= t;

  // extract the FAR plane
  frustum[4][0] = clip[3] - clip[2];
  frustum[4][1] = clip[7] - clip[6];
  frustum[4][2] = clip[11] - clip[10];
  frustum[4][3] = clip[15] - clip[14];

  // normalize the result
  t = sqrt(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2]);
  frustum[4][0] /= t;
  frustum[4][1] /= t;
  frustum[4][2] /= t;
  frustum[4][3] /= t;

  // extract the NEAR plane
  frustum[5][0] = clip[3] + clip[2];
  frustum[5][1] = clip[7] + clip[6];
  frustum[5][2] = clip[11] + clip[10];
  frustum[5][3] = clip[15] + clip[14];

  // normalize the result
  t = sqrt(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2]);
  frustum[5][0] /= t;
  frustum[5][1] /= t;
  frustum[5][2] /= t;
  frustum[5][3] /= t;
}

bool GlPrimitives::isPointInFrustum( float x, float y, float z )
{
  int i;
  for (i = 0; i < 6; i ++)
    if (frustum[i][0] * x + frustum[i][1] * y + frustum[i][2] * z + frustum[i][3] <= 0)
      return false;
  return true;
}

bool GlPrimitives::isSphereInFrustum( float x, float y, float z, float radius )
{
  int i;
  for (i = 0; i < 6; i ++)
    if (frustum[i][0] * x + frustum[i][1] * y + frustum[i][2] * z + frustum[i][3] <= -radius)
      return false;
  return true;
}

bool GlPrimitives::isCubeInFrustum( float x, float y, float z, float size )
{
  int i;
  for (i = 0; i < 6; i ++)
  {
    if (frustum[i][0] * (x - size) + frustum[i][1] * (y - size) + frustum[i][2] * (z - size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x + size) + frustum[i][1] * (y - size) + frustum[i][2] * (z - size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x - size) + frustum[i][1] * (y + size) + frustum[i][2] * (z - size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x + size) + frustum[i][1] * (y + size) + frustum[i][2] * (z - size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x - size) + frustum[i][1] * (y - size) + frustum[i][2] * (z + size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x + size) + frustum[i][1] * (y - size) + frustum[i][2] * (z + size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x - size) + frustum[i][1] * (y + size) + frustum[i][2] * (z + size) + frustum[i][3] > 0)
      continue;
    if (frustum[i][0] * (x + size) + frustum[i][1] * (y + size) + frustum[i][2] * (z + size) + frustum[i][3] > 0)
      continue;
    return false;
  }
  return true;
}

#endif

