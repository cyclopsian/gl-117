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

/****************************************************************************
OpenGL
****************************************************************************/

GL::GL () // (GLfloat clipback, GLfloat clipfront)
{
  antialiasing = false;
  zbuffer = false;
  alphablending = false;
  shading = 0;
  gllistnr = 0;
  gllightnr = 0;
//  glclipback = clipback;
//  glclipfront = clipfront;
  texnum = -1;
  aktlist = 0;
  fogcolor [0] = 0.5; fogcolor [1] = 0.5; fogcolor [2] = 0.5;
  foglum = 1.0;
/*    gluPerspective (90.0, 1.0, clipback, clipfront);
  glMatrixMode (GL_MATRIXMODE);
  glClearColor (0.0, 0.0, 0.0, 0.0);*/
}

GL::~GL ()
{
/*  int i;
  for (i = 0; i <= texnum; i ++)
    delete tex [i];*/
}

void GL::clearScreen ()
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GL::drawScreen ()
{
  glutSwapBuffers ();
}

void GL::rotate (int x, int y, int z)
{
  glRotatef (z, 0.0f, 0.0f, 1.0f);
  glRotatef (y, 0.0f, 1.0f, 0.0f);
  glRotatef (x, 1.0f, 0.0f, 0.0f);
}

/*  void genLight (float x, float y, float z, float a)
{
  glEnable (0x4000 + gllightnr);
  GLfloat position [] = { x, y, z };
  glLightfv (0x4000 + gllightnr, GL_POSITION, position);
  GLfloat p [] = { a, 0, 0 };
  glLightfv (0x4000 + gllightnr, GL_CONSTANT_ATTENUATION, p);
  gllightnr ++;
  if (gllightnr > 7) gllightnr = 0;
}*/

GLuint GL::genListSphere (GLint slices, GLint stacks, float radius)
{
  GLUquadricObj *quadObj;
  quadObj = gluNewQuadric ();
  glNewList (++ gllistnr, GL_COMPILE);
  gluSphere (quadObj, radius, slices, stacks);
  glEndList ();
  return gllistnr;
}

void GL::setList (GLuint listnr)
{
  glCallList (listnr);
}

void GL::genList (int *list)
{
  *list = ++ aktlist;
  glNewList (aktlist, GL_COMPILE_AND_EXECUTE);
}

int GL::genTexture ()
{
  return 1;
}

/*  GLuint genTextureAlphaTGA (char *fname, GLenum minFilter, GLenum maxFilter, int alphatype)
{
  short x, y;
  glBindTexture (GL_TEXTURE_2D, 1);
//    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
  FILE *in = fopen (fname, "rb");
  if (!in) return 0;
  fseek (in, 12, SEEK_SET);
  fread (&x, 2, 1, in);
  fread (&y, 2, 1, in);
  fseek (in, 58, SEEK_SET);
  unsigned char *buf = (unsigned char *) malloc (x * y * 4);
  fread (buf, x * y * 4, 1, in);
  fclose (in);
  glTexImage2D (GL_TEXTURE_2D, 0, 4, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
  free (buf);
  return 1;
}*/

void GL::enableLinearTexture (int texnum)
{
  glBindTexture (GL_TEXTURE_2D, texnum);
  if (!tex [texnum]->mipmap)
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  else
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void GL::disableLinearTexture (int texnum)
{
  glBindTexture (GL_TEXTURE_2D, texnum);
  if (!tex [texnum]->mipmap)
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  else
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

CTexture *GL::getTextureTGA (char *fname)
{
  int i;
  for (i = 0; i <= texnum; i ++)
  {
    if (!strcmp (fname, tex [i]->name)) break;
  }
  if (i <= texnum) return tex [i];
  return NULL;
}

CTexture *GL::genTextureTGA (char *fname, int quality, int alphatype, int mipmap2, bool alpha)
{
  char buf [STDSIZE];
  CTexture *mytex;
  if ((mytex = getTextureTGA (fname)) != NULL)
  {
    return mytex;
  }
  texnum ++;
  glBindTexture (GL_TEXTURE_2D, texnum);
  tex [texnum] = new CTexture ();
  if (!tex [texnum]->loadFromTGA (fname, quality, alphatype, mipmap2))
  {
    sprintf (buf, "Texture %s not found", fname);
    display (buf, LOG_FATAL);
    exit (1);
  }
  tex [texnum]->alpha = alpha;
  tex [texnum]->textureID = texnum;
  if (quality == 0) disableLinearTexture (texnum);
  else enableLinearTexture (texnum);
  if (!mipmap2)
    glTexImage2D (GL_TEXTURE_2D, 0, 4, tex [texnum]->width, tex [texnum]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex [texnum]->data);
  else
    gluBuild2DMipmaps (GL_TEXTURE_2D, 4, tex [texnum]->width, tex [texnum]->height, GL_RGBA, GL_UNSIGNED_BYTE, tex [texnum]->data);
  return tex [texnum];
}

float GL::getTexLight ()
{
  return tex [akttex]->texlight;
}

void GL::enableAntiAliasing ()
{
  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_POINT_SMOOTH);
  glEnable (GL_POLYGON_SMOOTH);
  glHint (GL_LINE_SMOOTH_HINT, GL_FASTEST);
  glHint (GL_POINT_SMOOTH_HINT, GL_FASTEST);
  glHint (GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
//    glLineWidth (2.0);
//    glPointSize (2.0);
  antialiasing = true;
}

void GL::disableAntiAliasing ()
{
  glDisable (GL_LINE_SMOOTH);
  glDisable (GL_POINT_SMOOTH);
  glDisable (GL_POLYGON_SMOOTH);
  antialiasing = false;
}

void GL::enableAlphaBlending ()
{
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  alphablending = true;
}

void GL::disableAlphaBlending ()
{
  glDisable (GL_BLEND);
  alphablending = false;
}

void GL::enableTextures (int num)
{
  akttex = num;
  glBindTexture (GL_TEXTURE_2D, num);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); /* GL_CLAMP or GL_REPEAT? */
  glEnable (GL_TEXTURE_2D);
}

void GL::enableLighting ()
{
  glEnable (GL_LIGHTING);
}

void GL::enableZBuffer ()
{
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);
  glDepthRange (10.0, -10.0);
  zbuffer = true;
}

void GL::disableZBuffer ()
{
  glDisable (GL_DEPTH_TEST);
  zbuffer = false;
}

void GL::shadeFlat ()
{
  glShadeModel (GL_FLAT);
  shading = 1;
}

void GL::shadeSmooth ()
{
  glShadeModel (GL_SMOOTH);
  shading = 2;
}

void GL::enableFog (float view)
{
  float fcol [3];
  fcol [0] = fogcolor [0] * foglum;
  fcol [1] = fogcolor [1] * foglum;
  fcol [2] = fogcolor [2] * foglum;
  glEnable (GL_FOG);
  glFogfv (GL_FOG_COLOR, fcol);
  glFogf (GL_FOG_DENSITY, 0.1);
  glFogi (GL_FOG_MODE, GL_LINEAR);
  if (quality <= 5)
    glHint (GL_FOG_HINT, GL_FASTEST);
  else
    glHint (GL_FOG_HINT, GL_NICEST);
  glFogf (GL_FOG_START, 1.0);
  glFogf (GL_FOG_END, view);
}



/* Frustum Culling according to OpenGL Page */
void GL::extractFrustum()
{
  float proj[16];
  float modl[16];
  float clip[16];
  float t;

  /* extract transformation matrices */
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

  /* extract numbers for the RIGHT plane */
  frustum[0][0] = clip[3] - clip[0];
  frustum[0][1] = clip[7] - clip[4];
  frustum[0][2] = clip[11] - clip[8];
  frustum[0][3] = clip[15] - clip[12];

  /* normalize the result */
  t = sqrt(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2]);
  frustum[0][0] /= t;
  frustum[0][1] /= t;
  frustum[0][2] /= t;
  frustum[0][3] /= t;

  /* extract the numbers for the LEFT plane */
  frustum[1][0] = clip[3] + clip[0];
  frustum[1][1] = clip[7] + clip[4];
  frustum[1][2] = clip[11] + clip[8];
  frustum[1][3] = clip[15] + clip[12];

  /* normalize the result */
  t = sqrt(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2]);
  frustum[1][0] /= t;
  frustum[1][1] /= t;
  frustum[1][2] /= t;
  frustum[1][3] /= t;

  /* extract the BOTTOM plane */
  frustum[2][0] = clip[3] + clip[1];
  frustum[2][1] = clip[7] + clip[5];
  frustum[2][2] = clip[11] + clip[9];
  frustum[2][3] = clip[15] + clip[13];

  /* normalize the result */
  t = sqrt(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2]);
  frustum[2][0] /= t;
  frustum[2][1] /= t;
  frustum[2][2] /= t;
  frustum[2][3] /= t;

  /* extract the TOP plane */
  frustum[3][0] = clip[3] - clip[1];
  frustum[3][1] = clip[7] - clip[5];
  frustum[3][2] = clip[11] - clip[9];
  frustum[3][3] = clip[15] - clip[13];

  /* normalize the result */
  t = sqrt(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2]);
  frustum[3][0] /= t;
  frustum[3][1] /= t;
  frustum[3][2] /= t;
  frustum[3][3] /= t;

  /* extract the FAR plane */
  frustum[4][0] = clip[3] - clip[2];
  frustum[4][1] = clip[7] - clip[6];
  frustum[4][2] = clip[11] - clip[10];
  frustum[4][3] = clip[15] - clip[14];

  /* normalize the result */
  t = sqrt(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2]);
  frustum[4][0] /= t;
  frustum[4][1] /= t;
  frustum[4][2] /= t;
  frustum[4][3] /= t;

  /* extract the NEAR plane */
  frustum[5][0] = clip[3] + clip[2];
  frustum[5][1] = clip[7] + clip[6];
  frustum[5][2] = clip[11] + clip[10];
  frustum[5][3] = clip[15] + clip[14];

  /* normalize the result */
  t = sqrt(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2]);
  frustum[5][0] /= t;
  frustum[5][1] /= t;
  frustum[5][2] /= t;
  frustum[5][3] /= t;
}

bool GL::isPointInFrustum( float x, float y, float z )
{
  int i;
  for (i = 0; i < 6; i ++)
    if (frustum[i][0] * x + frustum[i][1] * y + frustum[i][2] * z + frustum[i][3] <= 0)
      return false;
  return true;
}

bool GL::isSphereInFrustum( float x, float y, float z, float radius )
{
  int i;
  for (i = 0; i < 6; i ++)
    if (frustum[i][0] * x + frustum[i][1] * y + frustum[i][2] * z + frustum[i][3] <= -radius)
      return false;
  return true;
}

bool GL::isCubeInFrustum( float x, float y, float z, float size )
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

