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

/* This file includes the memory representation of any 3D model.
   For a detailed description of the data structure look at model.h */

#ifndef IS_MODEL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"
#include "gl.h"
#include "mathtab.h"
#include "loader_tga.h"



Color::Color ()
{
  memset (c, 0xFF, 4 * sizeof (unsigned char));
}

Color::Color (const Color &color)
{
  set (color);
}

Color::Color (int red, int green, int blue, int alpha)
{
  set (red, green, blue, alpha);
}

void Color::set (const Color &color)
{
  memcpy (c, color.c, 4 * sizeof (unsigned char));
}

void Color::set (int red, int green, int blue, int alpha)
{
  c [0] = (unsigned char) red;
  c [1] = (unsigned char) green;
  c [2] = (unsigned char) blue;
  c [3] = (unsigned char) alpha;
}

bool Color::isEqual (const Color &color) const
{
  return memcmp (c, color.c, 4 * sizeof (unsigned char)) == 0;
}

void Color::take (const Color &color)
{
  memcpy (c, color.c, 4 * sizeof (unsigned char));
}

/* Do NOT use overloaded operators, as we will lose the feeling
   which operators are slow and which are fast.
   E.g. a = b; c = d; but first is a pointer copy, second copies a huge data structure! */
/*void Color::operator = (const Color &color)
{
  memcpy (c, color.c, 4 * sizeof (unsigned char));
}*/



Texture::Texture ()
{
  data = NULL;
  name = "";
  mipmap = true;
  textureID = -1;
  width = 0;
  height = 0;
  texlight = 1.0F;
  texred = 1.0F;
  texgreen = 1.0F;
  texblue = 1.0F;
  alpha = false;
}

Texture::Texture (const std::string &filename, int alphaprogram, bool mipmap, bool alpha)
{
  texlight = 1.0F;
  texred = 1.0F;
  texgreen = 1.0F;
  texblue = 1.0F;

  loadFromTGA (filename, alphaprogram, mipmap, alpha);
}

Texture::~Texture ()
{
  if (data != NULL)
    delete data;
}

bool Texture::loadFromTGA (const std::string &filename, int alphaprogram, bool mipmap, bool alpha)
{
  int i, i2;

#ifdef LOADER_TGA_H
  data = LoadTga::load (const_cast<char *>(filename.c_str ()), &width, &height); // global 32 bpp texture buffer
  if (!data)
  {
    char buf [256];
    sprintf (buf, "Texture %s not found", filename.c_str ());
    display (buf, LOG_FATAL);
  }
#else
  unsigned char skip;
  FILE *in = fopen (filename.c_str (), "rb");
  if (!in)
  {
    sprintf (buf, "Texture %s not found", filename.c_str ());
    display (buf, LOG_FATAL);
  }
  fread (&skip, 1, 1, in);
  fseek (in, 12, SEEK_SET);
  fread (&width, 2, 1, in);
  fread (&height, 2, 1, in);
  fseek (in, 18 + skip, SEEK_SET);
  unsigned char *buf = (unsigned char *) malloc (width * height * 3); // preload file buffer
  if (buf == NULL) error_outofmemory ();
  data = (unsigned char *) malloc (width * height * 4); // global 32 bpp texture buffer
  if (data == NULL) error_outofmemory ();
  fread (buf, width * height * 3, 1, in);
#endif

  long texl = 0;
  long texr = 0;
  long texg = 0;
  long texb = 0;

  // analyse texture file
  for (i = 0; i < height; i ++)
    for (i2 = 0; i2 < width; i2 ++)
    {
      int n2 = (i * width + i2) * 4;

      texl += (int) data [n2 + 2] + data [n2 + 1] + data [n2];
      texr += (int) data [n2];
      texg += (int) data [n2 + 1];
      texb += (int) data [n2 + 2];

      if (alphaprogram == 0) // alpha=255 or 0
      {
        if (data [n2+0] + data [n2+1] + data [n2+2] < 30)
          data [n2+3] = 0;
        else
          data [n2+3] = 255;
      }
      else if (alphaprogram == 1) // alpha=maxcolor
      {
        int max = (data [n2+0] > data [n2+1] ? data [n2+0] : data [n2+1]);
        max = (max > data [n2+2] ? max : data [n2+2]);
        data [n2+3] = max;
      }
      else if (alphaprogram == 2) // alpha=red
      {
        data [n2+3] = data [n2];
      }
      else if (alphaprogram == 3) // alpha=midcolor*6 or 0
      {
        int test = (data [n2] + data [n2 + 1] + data [n2 + 2]) * 2;
        if (test > 255) test = 255;
        else if (test < 30) test = 0;
        data [n2+3] = (unsigned char) test;
      }
      else if (alphaprogram == 4) // alpha=red, color=white
      {
        data [n2+3] = data [n2+0];
        data [n2+0] = 255;
        data [n2+1] = 255;
        data [n2+2] = 255;
      }
      else if (alphaprogram == 5) // alpha=red*2, color=black
      {
        int test = data [n2+0] * 2;
        if (test > 255) test = 255;
        data [n2+3] = test;
        data [n2+0] = 0;
        data [n2+1] = 0;
        data [n2+2] = 0;
      }
      else if (alphaprogram == 6) // alpha=red, color=black
      {
        data [n2+3] = data [n2+0];
        data [n2+0] = 0;
        data [n2+1] = 0;
        data [n2+2] = 0;
      }
    }

#ifndef LOADER_TGA_H
  free (buf);
#endif

  texlight = (float) texl / width / height / 3 / 256; // average brightness
  texred = (float) texr / width / height / 256; // average red
  texgreen = (float) texg / width / height / 256; // average green
  texblue = (float) texb / width / height / 256; // average blue
  name = filename;
  this->mipmap = mipmap;
  this->alpha = alpha;

  textureID = gl->registerTexture (filename, data, width, height, mipmap);
  
  return true;
}

void Texture::getColor (Color *color, int x, int y) const
{
  if (x < 0) x = (int) -x % width;
  if (y < 0) y = (int) -y % height;
  if (x >= width) x = (int) x % width;
  if (y >= height) y = (int) y % height;
  
  int offset = y * width + x;
  offset <<= 2;
  
  color->c [0] = data [offset];
  color->c [1] = data [offset + 1];
  color->c [2] = data [offset + 2];
  color->c [3] = data [offset + 3];
}

void Texture::shadeLinear () const
{
  gl->enableLinearTexture (textureID, mipmap);
}

void Texture::shadeConst () const
{
  gl->disableLinearTexture (textureID, mipmap);
}



Vector3::Vector3 ()
{
  set (0, 0, 0);
}

Vector3::Vector3 (float x, float y, float z)
{
  set (x, y, z);
}

Vector3::Vector3 (const Vector3 &v)
{
  set (v);
}

void Vector3::set (const Vector3 &v)
{
  x = v.x;
  y = v.y;
  z = v.z;
}

void Vector3::set (float x, float y, float z)
{
  this->x = x;
  this->y = y;
  this->z = z;
}

void Vector3::neg ()
{
  x = -x; y = -y; z = -z;
}

void Vector3::add (const Vector3 &v)
{
  x += v.x; y += v.y; z += v.z;
}

void Vector3::sub (const Vector3 &v)
{
  x -= v.x; y -= v.y; z -= v.z;
}

void Vector3::mul (float fac)
{
  x *= fac; y *= fac; z *= fac;
}

void Vector3::crossproduct (const Vector3 &v)
{
  float nx = y * v.z - z * v.y;
  float ny = z * v.x - x * v.z;
  float nz = x * v.y - y * v.x;
  x = nx;
  y = ny;
  z = nz;
}

float Vector3::dotproduct (const Vector3 &v) const
{
  return x * v.x + y * v.y + z * v.z;
}

float Vector3::length () const
{
  return (float) sqrt (x * x + y * y + z * z);
}

void Vector3::norm ()
{
  float d = sqrt (x * x + y * y + z * z);
  if (d == 0)
    d = 1E-10;
  x /= d;
  y /= d;
  z /= d;
}

bool Vector3::isEqual (const Vector3 &v) const
{
  return x == v.x && y == v.y && z == v.z;
}

bool Vector3::isEqual (const Vector3 &v, float tolerance) const
{
  return x >= v.x - tolerance && x <= v.x + tolerance &&
         y >= v.y - tolerance && y <= v.y + tolerance &&
         z >= v.z - tolerance && z <= v.z + tolerance;
}



Vector2::Vector2 ()
{
  set (0, 0);
}

Vector2::Vector2 (float x, float y)
{
  set (x, y);
}

Vector2::Vector2 (const Vector2 &v)
{
  set (v);
}

void Vector2::set (const Vector2 &v)
{
  x = v.x;
  y = v.y;
}

void Vector2::set (float x, float y)
{
  this->x = x;
  this->y = y;
}

bool Vector2::isEqual (const Vector2 &v) const
{
  return x == v.x && y == v.y;
}

bool Vector2::isEqual (const Vector2 &v, float tolerance) const
{
  return x >= v.x - tolerance && x <= v.x + tolerance &&
         y >= v.y - tolerance && y <= v.y + tolerance;
}



Vertex::Vertex ()
{
  triangles = 0;
}

Vertex::Vertex (const Vertex &v)
{
  set (v);
}

void Vertex::set (const Vertex &v)
{
  vector.set (v.vector);
  normal.set (v.normal);
  color.set (v.color);
  triangles = v.triangles;
}

void Vertex::addNormal (const Vector3 &n)
{
  triangles ++;
  normal.x = (normal.x * (triangles - 1) + n.x) / (float) triangles;
  normal.y = (normal.y * (triangles - 1) + n.y) / (float) triangles;
  normal.z = (normal.z * (triangles - 1) + n.z) / (float) triangles;
}

void Vertex::addColor (const Color &color)
{
  triangles ++;
  for (int i = 0; i < 4; i ++)
  {
    this->color.c [i] = (unsigned char) (((float) this->color.c [i] * (triangles - 1)
                                         + color.c [i]) / (float) triangles);
  }
}



//double pitab;
//float sintab [360], costab [360];

Rotation::Rotation ()
{
  a = 0;
  b = 0;
  c = 0;
//  calculate ();
}

void Rotation::set (const Rotation &r)
{
  a = r.a;
  b = r.b;
  c = r.c;
}

void Rotation::set (float a, float b, float c)
{
  this->a = a;
  this->b = b;
  this->c = c;
}

void Rotation::add (const Rotation &r)
{
  a += r.a;
  b += r.b;
  c += r.c;
}

void Rotation::add (float a, float b, float c)
{
  this->a += a;
  this->b += b;
  this->c += c;
}

/*void Rotation::calculate ()
{
  rot [0] [0] = COS(c) * COS(b);
  rot [0] [1] = SIN(a) * SIN(b) * COS(c) - SIN(c) * COS(a);
  rot [0] [2] = SIN(a) * SIN(c) + COS(a) * SIN(b) * COS(c);
  rot [1] [0] = SIN(c) * COS(b);
  rot [1] [1] = COS(c) * COS(a) + SIN(a) * SIN(b) * SIN(c);
  rot [1] [2] = COS(a) * SIN(b) * SIN(c) - SIN(a) * COS(c);
  rot [2] [0] = -SIN(b);
  rot [2] [1] = SIN(a) * COS(b);
  rot [2] [2] = COS(a) * COS(b);
}

float Rotation::rotateX (const Vector3 &v) const
{
  return v.x * rot [0] [0] + v.y * rot [0] [1] + v.z * rot [0] [2];
}

float Rotation::rotateY (const Vector3 &v) const
{
  return v.x * rot [1] [0] + v.y * rot [1] [1] + v.z * rot [1] [2];
}

float Rotation::rotateZ (const Vector3 &v) const
{
  return v.x * rot [2] [0] + v.y * rot [2] [1] + v.z * rot [2] [2];
}*/



Triangle::Triangle ()
{
  v [0] = NULL;
  v [1] = NULL;
  v [2] = NULL;
}

void Triangle::calcNormal (Vector3 *n)
{
  Vector3 dummy;
  n->set (v [1]->vector);
  n->sub (v [0]->vector);
  dummy.set (v [2]->vector);
  dummy.sub (v [0]->vector);
  n->crossproduct (dummy);
}

void Triangle::setVertices (Vertex *a, Vertex *b, Vertex *c)
{
  int i;
  Vector3 dummy;
  v [0] = a;
  v [1] = b;
  v [2] = c;
  calcNormal (&dummy);
  dummy.norm ();
  if (dummy.z > 0)
    dummy.neg ();
  for (i = 0; i < 3; i ++)
  {
    v [i]->addNormal (dummy);
  }
}



Quad::Quad ()
{
  v [0] = NULL;
  v [1] = NULL;
  v [2] = NULL;
  v [3] = NULL;
}

void Quad::calcNormal (Vector3 *n)
{
  Vector3 dummy;
  n->set (v [1]->vector);
  n->sub (v [0]->vector);
  dummy.set (v [3]->vector);
  dummy.sub (v [0]->vector);
  n->crossproduct (dummy);
}

void Quad::setVertices (Vertex *a, Vertex *b, Vertex *c, Vertex *d)
{
  int i;
  Vector3 dummy;
  v [0] = a;
  v [1] = b;
  v [2] = c;
  v [3] = d;
  calcNormal (&dummy);
  dummy.norm ();
  if (dummy.z > 0)
    dummy.neg ();
  for (i = 0; i < 4; i ++)
  {
    v [i]->addNormal (dummy);
  }
}



Material::Material ()
{
  uscale = 1;
  vscale = 1;
  uoffset = 0;
  voffset = 0;
  wrot = 0;
  filename = "";
  name = "";
}



Object3d::Object3d ()
{
  numVertices = 0;
  numTriangles = 0;
  numQuads = 0;
  numTexVertex = 0;
  material = NULL;
  hasTexture = false;
}

int Object3d::addVertex (const Vertex &w)
{
  int i;
  for (i = 0; i < numVertices; i ++)
    if (w.vector.isEqual (vertex [i].vector, 1e-3F) && w.color.isEqual (vertex [i].color)) break;
  if (i == numVertices)
  vertex [numVertices ++].set (w);
  return i;
}

void Object3d::setColor (const Color &col)
{
  int i;
  for (i = 0; i < numVertices; i ++)
    memcpy (vertex [i].color.c, (const void *) &col, 4 * sizeof (unsigned char));
}



Model3d::Model3d ()
{
  numObjects = 0;
  numMaterials = 0;
  shading = 0;
  displaylist = true;
  list1 = -1;
  list2 = -1;
  list3 = -1;
  scale = 1.0F;
  name = "";
  nolight = false;
  alpha = false;
/*  light_ambient [0] = 0.3; light_ambient [1] = 0.3; light_ambient [2] = 0.3; light_ambient [3] = 1;
  light_diffuse [0] = 0.9; light_diffuse [1] = 0.9; light_diffuse [2] = 0.9; light_diffuse [3] = 1;
  light_ambient2 [0] = 0.2; light_ambient2 [1] = 0.2; light_ambient2 [2] = 0.2; light_ambient2 [3] = 1;
  light_diffuse2 [0] = 0.1; light_diffuse2 [1] = 0.1; light_diffuse2 [2] = 0.1; light_diffuse [3] = 1;*/
  numRefpoints = 0;
  refpoint = NULL;
  va = new VertexArray (VERTEXARRAY_V3N3C4T2);
}

void Model3d::setName (const std::string name)
{
  this->name = name;
}

void Model3d::addMaterial (const Material &material)
{
  this->material [numMaterials] = new Material;
  if (this->material [numMaterials] == NULL) exit (100);
  memcpy (this->material [numMaterials], &material, sizeof (Material));
  numMaterials ++;
}

void Model3d::addObject (const Object3d &object)
{
  this->object [numObjects] = new Object3d;
  if (this->object [numObjects] == NULL) exit (101);
  memcpy (this->object [numObjects], &object, sizeof (Object3d));
  numObjects ++;
  rotcol = 0;
}

void Model3d::addRefPoint (const Vector3 &tl)
{
  int i, i2;
  if (refpoint == NULL)
  {
    refpoint = new Vector3 [10];
  }
  for (i = 0; i < numRefpoints; i ++)
  {
    if (tl.z < refpoint [i].z)
    {
      for (i2 = numRefpoints; i2 > i; i2 --)
      {
        refpoint [i2].set (refpoint [i2 - 1]);
      }
      refpoint [i].set (tl);
      goto fertigref1;
    }
  }
  refpoint [numRefpoints].set (tl);
fertigref1:;
  numRefpoints ++;
}

Model3d::~Model3d ()
{
  int i;
  for (i = 0; i < numMaterials; i ++)
    delete material [i];
  for (i = 0; i < numObjects; i ++)
    delete object [i];
  if (refpoint)
  {
    delete refpoint;
  }
}

void Model3d::setColor (const Color &col)
{
  int i;
  for (i = 0; i < numObjects; i++)
  {
    object [i]->setColor (col);
  }
}

void Model3d::drawVertexNormals (const Object3d &cm, float zoom)
{
  glColor3ub (255, 0, 0);
  glBegin (GL_LINES);
  for (int j = 0; j < cm.numVertices; j++)
  if (cm.vertex [j].triangles > 0)
  {
    glVertex3f (cm.vertex [j].vector.x*zoom, cm.vertex [j].vector.y*zoom, cm.vertex [j].vector.z*zoom);
    glVertex3f ((cm.vertex [j].vector.x + cm.vertex [j].normal.x / 5)*zoom, (cm.vertex [j].vector.y + cm.vertex [j].normal.y / 5)*zoom, (cm.vertex [j].vector.z + cm.vertex [j].normal.z / 5)*zoom);
  }
  glEnd ();
}

int Model3d::rotateColor (int n)
{
  if (n == 0) return 0;
  rotcol ++;
  if (rotcol > n) rotcol = 0;
  return rotcol;
}

void Model3d::scaleTexture (float fx, float fy)
{
  int i;
  for (i = 0; i < numObjects; i ++)
  {
    Object3d *o = object [i];
    int i2;
    for (i2 = 0; i2 < o->numVertices; i2 ++)
    {
      o->vertex [i2].tex.x *= fx;
      o->vertex [i2].tex.y *= fy;
    }
  }
}

void Model3d::draw (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
                   float zoom, float lum, int explode)
{
  if (nolight) // if model wants to be rendered without light, call draw2
  {
    glDisable (GL_LIGHTING);
    drawNoLight (tl, tl2, rot, zoom, explode);
    glEnable (GL_LIGHTING);
    return;
  }

  int i, j;
  Object3d *cm;
  float la [4] = { 0.2, 0.2, 0.2, 1.0};
  if (lum >= 1)
  {
    float addl = lum;
    if (addl >= 5) addl = 5;
    la [0] = 0.2 * addl;
    la [1] = 0.2 * addl;
    la [2] = 0.2 * addl;
  }
  glLightfv (GL_LIGHT0, GL_AMBIENT, la);

  float ld [4] = { 0.7, 0.7, 0.7, 1.0};
  if (lum != 1)
  {
    ld [0] *= lum;
    ld [1] *= lum;
    ld [2] *= lum;
    if (ld [0] > 1.0) ld [0] = 1.0;
    if (ld [1] > 1.0) ld [1] = 1.0;
    if (ld [2] > 1.0) ld [2] = 1.0;
  }
  glLightfv (GL_LIGHT0, GL_DIFFUSE, ld);

  for (i = 0; i < numObjects; i ++)
  {
    if (numObjects <= 0) break;
    cm = object [i];
    if (cm->hasTexture)
    {
      if (antialiasing)
        cm->material->texture->shadeLinear ();
      else
        cm->material->texture->shadeConst ();
    }
  }

  zoom *= scale;
  glPushMatrix ();
  glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
  float explodefac = (float) explode / 10 / timestep;

  if (showcollision)
  {
    glPushMatrix ();
    glScalef (cube.x, cube.y, cube.z);
    glColor3ub (255, 0, 0);
    glBegin (GL_LINE_STRIP);
    glVertex3f (1, 1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (1, 1, 1);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex3f (-1, 1, 1);
    glVertex3f (-1, 1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (-1, -1, 1);
    glVertex3f (-1, 1, 1);
    glEnd ();
    glBegin (GL_LINES);
    glVertex3f (1, 1, 1);
    glVertex3f (-1, 1, 1);
    glVertex3f (1, -1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (-1, -1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (-1, 1, -1);
    glEnd ();
    glPopMatrix ();
  }

  glRotatef (rot.c+90, 0, -1, 0);
  glRotatef (-rot.a+90, 0, 0, 1);
  glRotatef (rot.b+180, 1, 0, 0);
  glScalef (zoom, zoom, zoom);

  if (shading == 1)
    glShadeModel (GL_FLAT);
  else
    glShadeModel (GL_SMOOTH);

  if (alpha)
  { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.2); }

  for (i = 0; i < numObjects; i ++)
  {
    if (numObjects <= 0) break;
    cm = object [i];
    if (cm->hasTexture)
    {
      glEnable (GL_TEXTURE_2D);
      glColor4f (1, 1, 1, 1);
      glBindTexture (GL_TEXTURE_2D, cm->material->texture->textureID);
    }
    else
    {
      glDisable (GL_TEXTURE_2D);
      glColor4f (1, 1, 1, 1);
    }

    if (cm->material != NULL)
    {
      if (cm->material->color.c [0] > 190 && cm->material->color.c [1] > 190 && cm->material->color.c [2] < 20)
        glDisable (GL_LIGHTING);
      else
        glEnable (GL_LIGHTING);
    }

    Vector3 shift;

    va->glBegin (GL_TRIANGLES);
    for (j = 0; j < cm->numTriangles; j++)
    {
      Vertex *v = cm->triangle [j].v [0];
      if (explode > 0)
      {
        shift.x = v->normal.x * explodefac;
        shift.y = v->normal.y * explodefac;
        shift.z = v->normal.z * explodefac;
      }
      for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
      {
        v = cm->triangle [j].v [whichVertex];
        va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        if (cm->hasTexture)
        {
          if (cm->vertex)
          {
            va->glTexCoord2f (v->tex.x, v->tex.y);
            va->glColor4f (1, 1, 1, 1);
          }
        }
        else
        {
          if (numMaterials/* && cm->material->textureID >= 0*/) 
          {
            unsigned char *color = cm->material->color.c; /*material[cm->material->textureID]->color.c;*/
            if (color [0] > 190 && color [1] > 190 && color [2] < 20)
            {
              rotateColor (30);
              va->glColor4ub (color [0] + rotcol, color [1] + rotcol, color [2] + rotcol * 3, 255);
            }
            else
            {
              va->glColor4ub (color [0], color [1], color [2], color [3]);
            }
          }
        }
  //    glColor3ub (255, 255, 0);
        va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
      }
    }
    va->glEnd ();

    va->glBegin (GL_QUADS);
    for (j = 0; j < cm->numQuads; j++)
    {
      Vertex *v = cm->quad [j].v [0];
      if (explode > 0)
      {
        shift.x = v->normal.x * explodefac;
        shift.y = v->normal.y * explodefac;
        shift.z = v->normal.z * explodefac;
      }
      for (int whichVertex = 0; whichVertex < 4; whichVertex++)
      {
        v = cm->quad [j].v [whichVertex];
        va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        if (cm->hasTexture)
        {
          if (cm->vertex)
          {
            va->glTexCoord2f (v->tex.x, v->tex.y);
            va->glColor4f (1, 1, 1, 1);
          }
        }
        else
        {
          if (numMaterials && cm->material->texture->textureID >= 0) 
          {
            unsigned char *pColor = material [cm->material->texture->textureID]->color.c;
            va->glColor4ub (pColor [0], pColor [1], pColor [2], pColor [3]);
          }
        }
        va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
      }
    }
    va->glEnd ();
  }

  if (alpha)
  { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

  glPopMatrix ();
}

void Model3d::drawNoLight (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
                          float zoom, int explode)
{
  int i, j;
  Object3d *cm;

  for (i = 0; i < numObjects; i ++)
  {
    if (numObjects <= 0) break;
    cm = object [i];
    if (cm->hasTexture)
    {
      if (antialiasing)
        cm->material->texture->shadeLinear ();
      else
        cm->material->texture->shadeConst ();
    }
  }

  zoom *= scale;
  glPushMatrix ();
  glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
  float explodefac = (float) explode / 10 / timestep;

  if (showcollision)
  {
    glPushMatrix ();
    glScalef (cube.x, cube.y, cube.z);
    glColor3ub (255, 0, 0);
    glBegin (GL_LINE_STRIP);
    glVertex3f (1, 1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (1, 1, 1);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex3f (-1, 1, 1);
    glVertex3f (-1, 1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (-1, -1, 1);
    glVertex3f (-1, 1, 1);
    glEnd ();
    glBegin (GL_LINES);
    glVertex3f (1, 1, 1);
    glVertex3f (-1, 1, 1);
    glVertex3f (1, -1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (-1, -1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (-1, 1, -1);
    glEnd ();
    glPopMatrix ();
  }

  glRotatef (rot.c+90, 0, -1, 0);
  glRotatef (-rot.a+90, 0, 0, 1);
  glRotatef (rot.b+180, 1, 0, 0);
  glScalef (zoom, zoom, zoom);

  bool listgen = false;
  if (list2 == -1 && explode <= 0 && displaylist)
  {
    listgen = true;
    list2 = gl->createList ();
  }
  if (listgen || explode > 0 || !displaylist)
  {

    if (shading == 1)
      glShadeModel (GL_FLAT);
    else
      glShadeModel (GL_SMOOTH);

    if (alpha)
    { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.01); }

    for (i = 0; i < numObjects; i++)
    {
      if (numObjects <= 0) break;
      cm = object [i];
      if (cm->hasTexture)
      {
        glEnable (GL_TEXTURE_2D);
        glColor4ub (255, 255, 255, 255);
        glBindTexture (GL_TEXTURE_2D, cm->material->texture->textureID);
      }
      else
      {
        glDisable (GL_TEXTURE_2D);
        glColor4ub (255, 255, 255, 255);
      }

      Vector3 shift;

      va->glBegin (GL_TRIANGLES);
      for (j = 0; j < cm->numTriangles; j++)
      {
        Vertex *v = cm->triangle [j].v [0];
        if (explode > 0)
        {
          shift.x = v->normal.x * explodefac;
          shift.y = v->normal.y * explodefac;
          shift.z = v->normal.z * explodefac;
        }
        for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
        {
          v = cm->triangle [j].v [whichVertex];
          va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
          if (cm->hasTexture)
          {
            if (cm->vertex)
            {
              va->glTexCoord2f (v->tex.x, v->tex.y);
              va->glColor4f (1, 1, 1, 1);
            }
          }
          else
          {
            unsigned char *pColor = v->color.c;
            va->glColor4ub (pColor [0], pColor [1], pColor [2], pColor [3]);
          }
          va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
        }
      }
      va->glEnd ();

      va->glBegin (GL_QUADS);
      for (j = 0; j < cm->numQuads; j++)
      {
        Vertex *v = cm->quad [j].v [0];
        if (explode > 0)
        {
          shift.x = v->normal.x * explodefac;
          shift.y = v->normal.y * explodefac;
          shift.z = v->normal.z * explodefac;
        }
        for (int whichVertex = 0; whichVertex < 4; whichVertex ++)
        {
          v = cm->quad [j].v [whichVertex];
          va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
          if (cm->hasTexture)
          {
            if (cm->vertex)
            {
              va->glTexCoord2f (v->tex.x, v->tex.y);
              va->glColor4f (1, 1, 1, 1);
            }
          }
          else
          {
            unsigned char *pColor = v->color.c;
            va->glColor4ub (pColor [0], pColor [1], pColor [2], pColor [3]);
          }
          va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
        }
      }
      va->glEnd ();
    }

    if (alpha)
    { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

    if (listgen) glEndList ();
  }
  else glCallList (list2);

  glPopMatrix ();
}

void Model3d::drawNoTexture (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
                            float zoom, float lum, int explode)
{
  int i, j;
  Object3d *cm;
//  float mx=0, my=0, mz=0, ix=0, iy=0, iz=0;
  zoom *= scale;
  glPushMatrix ();
  glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
  float explodefac = (float) explode / 10 / timestep;

  if (showcollision)
  {
    glPushMatrix ();
    glScalef (cube.x, cube.y, cube.z);
    glColor3ub (255, 0, 0);
    glBegin (GL_LINE_STRIP);
    glVertex3f (1, 1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (1, 1, 1);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex3f (-1, 1, 1);
    glVertex3f (-1, 1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (-1, -1, 1);
    glVertex3f (-1, 1, 1);
    glEnd ();
    glBegin (GL_LINES);
    glVertex3f (1, 1, 1);
    glVertex3f (-1, 1, 1);
    glVertex3f (1, -1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (-1, -1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (-1, 1, -1);
    glEnd ();
    glPopMatrix ();
  }

  glRotatef (rot.c+90, 0, -1, 0);
  glRotatef (-rot.a+90, 0, 0, 1);
  glRotatef (rot.b+180, 1, 0, 0);
  glScalef (zoom, zoom, zoom);

  if (alpha)
  { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.2); }

  for (i = 0; i < numObjects; i++)
  {
    if (numObjects <= 0) break;
    cm = object [i];
    glDisable (GL_TEXTURE_2D);
    glColor3ub (255, 255, 255);

    Vector3 shift;

    va->glBegin (GL_TRIANGLES);
    for (j = 0; j < cm->numTriangles; j++)
    {
      Vertex *v = cm->triangle [j].v [0];
      if (explode > 0)
      {
        shift.x = v->normal.x * explodefac;
        shift.y = v->normal.y * explodefac;
        shift.z = v->normal.z * explodefac;
      }
      for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
      {
        v = cm->triangle [j].v [whichVertex];
  //      glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        unsigned char *pColor = v->color.c;
        float red = lum * pColor [0] / 256;
        float green = lum * pColor [1] / 256;
        float blue = lum * pColor [2] / 256;
        if (red >= 1.0) red = 1.0;
        if (green >= 1.0) green = 1.0;
        if (blue >= 1.0) blue = 1.0;
        va->glColor3f (red, green, blue);
        va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
      }
    }
    va->glEnd ();

    va->glBegin (GL_QUADS);
    for (j = 0; j < cm->numQuads; j++)
    {
      Vertex *v = cm->quad [j].v [0];
      if (explode > 0)
      {
        shift.x = v->normal.x * explodefac;
        shift.y = v->normal.y * explodefac;
        shift.z = v->normal.z * explodefac;
      }
      for (int whichVertex = 0; whichVertex < 4; whichVertex ++)
      {
        v = cm->quad [j].v [whichVertex];
  //      glNormal3f (v->normal.x, v->normal.y, v->normal.z);
        unsigned char *pColor = v->color.c;
        float red = lum * pColor [0] / 256;
        float green = lum * pColor [1] / 256;
        float blue = lum * pColor [2] / 256;
        if (red >= 1.0) red = 1.0;
        if (green >= 1.0) green = 1.0;
        if (blue >= 1.0) blue = 1.0;
        va->glColor3f (red, green, blue);
        va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
      }
    }
    va->glEnd ();

  }

  if (alpha)
  { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

  glPopMatrix ();
}

/*void Model3d::draw3 (const Vector3 &tl, const Vector3 &tl2, const Rotation &rot,
					float zoom, int explode)
{
  int i, j;
  Object3d *cm;
//  float mx=0, my=0, mz=0, ix=0, iy=0, iz=0;
  zoom *= scale;
  glPushMatrix ();
  glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
  float explodefac = (float) explode / 10 / timestep;

  if (showcollision)
  {
    glPushMatrix ();
    glScalef (cube.x, cube.y, cube.z);
    glColor3ub (255, 0, 0);
    glBegin (GL_LINE_STRIP);
    glVertex3f (1, 1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (1, 1, 1);
    glEnd ();
    glBegin (GL_LINE_STRIP);
    glVertex3f (-1, 1, 1);
    glVertex3f (-1, 1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (-1, -1, 1);
    glVertex3f (-1, 1, 1);
    glEnd ();
    glBegin (GL_LINES);
    glVertex3f (1, 1, 1);
    glVertex3f (-1, 1, 1);
    glVertex3f (1, -1, -1);
    glVertex3f (-1, -1, -1);
    glVertex3f (1, -1, 1);
    glVertex3f (-1, -1, 1);
    glVertex3f (1, 1, -1);
    glVertex3f (-1, 1, -1);
    glEnd ();
    glPopMatrix ();
  }

  glRotatef (rot.c+90, 0, -1, 0);
  glRotatef (-rot.a+90, 0, 0, 1);
  glRotatef (rot.b+180, 1, 0, 0);
  glScalef (zoom, zoom, zoom);

  bool listgen = false;
  if (list3 == -1 && explode <= 0 && displaylist)
  {
    listgen = true;
    list3 = gl->createList ();
  }
  if (listgen || explode > 0 || !displaylist)
  {

    if (shading == 1)
      glShadeModel (GL_FLAT);
    else
      glShadeModel (GL_SMOOTH);

    if (alpha)
    { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.2); }

    for (i = 0; i < numObjects; i++)
    {
      if (numObjects <= 0) break;
      cm = object [i];
      glDisable (GL_TEXTURE_2D);
      glColor3ub (255, 255, 255);

      Vector3 shift;

      va->glBegin (GL_TRIANGLES);
      for (j = 0; j < cm->numTriangles; j++)
      {
        Vertex *v = cm->triangle [j].v [0];
        if (explode > 0)
        {
          shift.x = v->normal.x * explodefac;
          shift.y = v->normal.y * explodefac;
          shift.z = v->normal.z * explodefac;
        }
        for (int whichVertex = 0; whichVertex < 3; whichVertex ++)
        {
          v = cm->triangle [j].v [whichVertex];
          va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
          if (cm->hasTexture && false)
          {
            if (cm->vertex)
            {
              va->glTexCoord2f (v->tex.x, v->tex.y);
              va->glColor4f (1, 1, 1, 1);
            }
          }
          else
          {
            unsigned char *pColor = v->color.c;
            va->glColor3ub (pColor[0], pColor[1], pColor[2]);
          }
          va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
        }
      }
      va->glEnd ();

      va->glBegin (GL_QUADS);
      for (j = 0; j < cm->numQuads; j++)
      {
        Vertex *v = cm->quad [j].v [0];
        if (explode > 0)
        {
          shift.x = v->normal.x * explodefac;
          shift.y = v->normal.y * explodefac;
          shift.z = v->normal.z * explodefac;
        }
        for (int whichVertex = 0; whichVertex < 4; whichVertex ++)
        {
          v = cm->quad [j].v [whichVertex];
          va->glNormal3f (v->normal.x, v->normal.y, v->normal.z);
          if (cm->hasTexture && false)
          {
            if (cm->vertex)
            {
              va->glTexCoord2f (v->tex.x, v->tex.y);
              va->glColor4f (1, 1, 1, 1);
            }
          }
          else
          {
            unsigned char *pColor = v->color.c;
            va->glColor3ub (pColor[0], pColor[1], pColor[2]);
          }
          va->glVertex3f (v->vector.x + shift.x, v->vector.y + shift.y, v->vector.z + shift.z);
        }
      }
      va->glEnd ();
    }

    if (alpha)
    { glDisable (GL_BLEND); glDisable (GL_ALPHA_TEST); }

    if (listgen) glEndList ();
  }
  else glCallList (list3);

  glPopMatrix ();
}*/



Sphere::Sphere () {}

Sphere::Sphere (float radius, int segments, float dx, float dy, float dz)
{
  init (radius, segments, dx, dy, dz, 0);
}

Sphere::~Sphere () {}

int Sphere::random (int n)
{
  if (n == 0) return 0;
  return rand () % n;
}

void Sphere::init (float radius, int segments, float dx, float dy, float dz, int randomized)
{
  Object3d co;
  co.vertex = new Vertex [segments * segments * 2 + 2];
  if (co.vertex == NULL) exit (100);
  co.triangle = new Triangle [segments * 4];
  if (co.triangle == NULL) exit (100);
  co.quad = new Quad [segments * segments * 2];
  if (co.quad == NULL) exit (100);

  this->radius = radius;
  this->segments = segments;
  this->dx = dx;
  this->dy = dy;
  this->dz = dz;
  
  int p [4];
  float step = 180.0 / segments;
  Vertex w;

  for (float i = 0; i < 180; i += step)
    for (float i2 = 0; i2 < 360; i2 += step)
    {
      int a = ((int) i) % 360, b = ((int) i2) % 360;
      float si = SIN(a), ci = COS(a);
      float si2 = SIN(b), ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      p [0] = co.addVertex (w);
      a = ((int) (i + step)) % 360;
      si = SIN(a); ci = COS(a);
      si2 = SIN(b); ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      if (a < 179 || i2 == 0) p [1] = co.addVertex (w);
      b = ((int) (i2 + step)) % 360;
      si = SIN(a); ci = COS(a);
      si2 = SIN(b); ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      if (a < 179) p [2] = co.addVertex (w);
      a = ((int) i) % 360;
      si = SIN(a); ci = COS(a);
      si2 = SIN(b); ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      p [3] = co.addVertex (w);
      if (i == 0 || i >= 180 - step - 1)
      {
        if (!random (randomized))
        {
          if (i == 0) co.triangle [co.numTriangles ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [2]]);
          else co.triangle [co.numTriangles ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [3]]);
        }
      }
      else
      {
        if (!random (randomized))
          co.quad [co.numQuads ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [2]], &co.vertex [p [3]]);
      }
    }

  addObject (co);
  setColor (Color (128, 128, 128, 255));
  for (int i2 = 0; i2 < object [0]->numVertices / 2; i2 ++)
  {
    object [0]->vertex [i2].normal.neg ();
  }
}

void Sphere::invertNormals ()
{
  for (int i = 0; i < object [0]->numVertices; i ++)
  {
    object [0]->vertex [i].normal.neg ();
  }
}

void Sphere::setNorthPoleColor (const Color &c, float w)
{
  int i, i2;
  for (i = 0; i < 4; i ++)
    object [0]->vertex [0].color.c [i] = c.c [i];
  int num = (int) (w * segments * segments / 2);
  num /= (segments * 2);
  num *= (segments * 2);
  for (i = 1; i <= num; i ++)
  {
    float weight = 1.0F - (float) ((int) ((i - 1) / (segments * 2)) * segments * 2) / (float) num;
    for (i2 = 0; i2 < 4; i2 ++)
      object [0]->vertex [i].color.c [i2] = (short) ((1.0F - weight) * object [0]->vertex [i].color.c [i2] + weight * c.c [i2]);
  }
}

void Sphere::setSouthPoleColor (const Color &c, float w)
{
  int i, i2;
  int max = (segments - 1) * segments * 2 + 1;
  for (i = 0; i < 4; i ++)
    object [0]->vertex [max].color.c [i] = c.c [i];
  int num = (int) (w * segments * segments / 2);
  num /= (segments * 2);
  num *= (segments * 2);
  for (i = 1; i <= num; i ++)
  {
    float weight = 1.0F - (float) ((int) ((i - 1) / (segments * 2)) * segments * 2) / (float) num;
    for (i2 = 0; i2 < 4; i2 ++)
      object [0]->vertex [max - i].color.c [i] = (short) ((1.0F - weight) * object [0]->vertex [max - i].color.c [i2] + weight * c.c [i2]);
  }
}

void Sphere::setPoleColor (int phi, int theta, const Color &c, float w)
{
  int i, i2;
  for (i = 0; i < object [0]->numVertices; i ++)
  {
    int phi2 = ((i - 1) % (segments * 2)) * 360 / (segments * 2);
    int theta2 = ((i - 1) / (segments * 2) + 1) * 360 / (segments * 2);
    if (i == 0) { theta2 = 0; phi2 = 0; }
    int dphi = phi - phi2;
    if (dphi < -180) dphi += 360;
    else if (dphi > 180) dphi -= 360;
    int dtheta = theta - theta2;
    if (dtheta < -180) dtheta += 360;
    else if (dtheta > 180) dtheta -= 360;
    float alpha = sqrt ((float) (dphi*dphi+dtheta*dtheta));
    if (alpha < 180 * w)
    {
      float weight = 1.0 - alpha / 180.0 / w;
      for (i2 = 0; i2 < 4; i2 ++)
        object [0]->vertex [i].color.c [i2] = (short) ((1.0F - weight) * object [0]->vertex [i].color.c [i2] + weight * c.c [i2]);
    }
  }
}



SpherePart::SpherePart () {}

SpherePart::SpherePart (float radius, int segments, float phi)
{
  init (radius, segments, phi);
}

SpherePart::~SpherePart () {}

void SpherePart::init (float radius, int segments, float phi)
{
  Object3d co;
  co.vertex = new Vertex [segments * 4 + 1];
  co.triangle = new Triangle [segments];
  co.quad = new Quad [segments * 3];
  this->radius = radius;
  this->segments = segments;
  float dx = 1, dy = 1, dz = 1;
  int p [4];
  float step = 360.0 / segments;
  float step2 = phi / 4;
  Vertex w;

  for (float i = 0; i < phi; i += step2)
    for (float i2 = 0; i2 < 360; i2 += step)
    {
      int a = ((int) i) % 360, b = ((int) i2) % 360;
      float si = SIN(a), ci = COS(a);
      float si2 = SIN(b), ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      p [0] = co.addVertex (w);
      a = ((int) (i + step2)) % 360;
      si = SIN(a); ci = COS(a);
      si2 = SIN(b); ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      if (a < 179 || i2 == 0) p [1] = co.addVertex (w);
      b = ((int) (i2 + step)) % 360;
      si = SIN(a); ci = COS(a);
      si2 = SIN(b); ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      if (a < 179) p [2] = co.addVertex (w);
      a = ((int) i) % 360;
      si = SIN(a); ci = COS(a);
      si2 = SIN(b); ci2 = COS(b);
      w.vector.x = radius * si * ci2 * dx; w.vector.y = radius * si * si2 * dy; w.vector.z = radius * ci * dz;
      p [3] = co.addVertex (w);
      if (i == 0 || i >= 180 - step2 - 0.2)
      {
        if (i == 0) co.triangle [co.numTriangles ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [2]]);
        else co.triangle [co.numTriangles ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [3]]);
      }
      else
      {
        co.quad [co.numQuads ++].setVertices (&co.vertex [p [0]], &co.vertex [p [1]], &co.vertex [p [2]], &co.vertex [p [3]]);
      }
    }
  
  addObject (co);
  setColor (Color (128, 128, 128, 255));
}

void SpherePart::setNorthPoleColor (const Color &c, float w)
{
  int i, i2;
  for (i = 0; i < 4; i ++)
    object [0]->vertex [0].color.c [i] = c.c [i];
  int num = (int) (w * segments * 4 / 2);
  num /= (segments * 2);
  num *= (segments * 2);
  for (i = 1; i <= num; i ++)
  {
    float weight = 1.0F - (float) ((int) ((i - 1) / (segments * 2)) * 4 * 2) / (float) num;
    for (i2 = 0; i2 < 4; i2 ++)
      object [0]->vertex [i].color.c [i2] = (short) ((1.0F - weight) * object [0]->vertex [i].color.c [i2] + weight * c.c [i2]);
  }
}

void SpherePart::setSouthPoleColor (const Color &c, float w)
{
}

void SpherePart::setPoleColor (int phi, int theta, const Color &c, float w)
{
}

#endif

