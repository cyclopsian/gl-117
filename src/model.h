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

/* This file includes the memory representation of any 3D model. */

#ifndef IS_MODEL_H
#define IS_MODEL_H

#include "common.h" // ok

#include "vertexarray.h" // ok

#include <iostream>

/* Currently models are normalized to the (-1,-1,-1)-(1,1,1) cube and static!
   The model represents the "class" description of a static model's geometry and colors.
   It is "instanciated" using a DynamicObj which has a reference to the model,
   scaling, rotation, translation, physical attributes...
   A model consists of several objects.
   Objects consist of vertices, quads, triangles, and one material. */

/**
* CColor stores color information for red, green, blue, alpha, each 0-255.
* Access is granted directly through array c with c[0]=red, c[1]=green, etc.
*/
class CColor
{
  public:
  /// color information as vector, 32 bpp (floats would be faster for the vertex arrays)
  unsigned char c [4];
  
  CColor ();
  CColor (const CColor &color);
  CColor (int red, int green, int blue, int alpha = 255);
  virtual ~CColor ();
  
  void setColor (const CColor &color);
  void setColor (int red, int green, int blue, int alpha = 255);
  bool isEqual (const CColor &color) const; // compare colors
  void take (const CColor &color);
//  void operator = (const CColor &color);
};

/**
* CTexture loads and stores a texture to memory.
* To use a texture, it must be loaded and added to the OpenGL texture list using
*   gl->genTextureTGA();
* instead of this class!
*/
class CTexture
{
  public:
  /// texture color data as array, 32 bpp (RGBA? => see tga loader)
  unsigned char *data;
  /// texture file name
  std::string name;
  /// mipmapping on/off depending on gluBuildMipmaps
  bool mipmap;
  /// texture ID of native GL code
  int textureID;
  /// texture width
  int width;
  /// texture height
  int height;
  /// average values to substitute textured quads by colors for LOD
  /// average of texture's overall brightness
  float texlight;
  /// average of texture's red
  float texred;
  /// average of texture's green
  float texgreen;
  /// average of texture's blue
  float texblue;
  /// alpha blending necessary?
  bool alpha;
  
  CTexture ();
  virtual ~CTexture ();
  
  /// loadFromTGA is called via gl->genTextureTGA() to not load the same texture twice
  bool loadFromTGA (std::string &filename, int alphatype, bool mipmap);
  /// get color of a special pixel
  void getColor (CColor *c, int x, int y) const;
};

/**
* CVector3 stores the components of a 3D vector (x,y,z).
* Access x,y,z directly.
*/
class CVector3 
{
  public:
  /// x coordinate
  float x;
  /// y coordinate
  float y;
  /// z coordinate
  float z;
  
  CVector3 ();
  CVector3 (float x, float y, float z);
  CVector3 (const CVector3 &v);
  virtual ~CVector3 ();
  
  void set (const CVector3 &v);
  void set (float x, float y, float z);
  void neg ();
  void add (const CVector3 &v);
  void sub (const CVector3 &v);
  void mul (float fac);
  void crossproduct (const CVector3 &v);
  float dotproduct (const CVector3 &v) const;
  float length () const;
  void norm ();
  /// exactly equal in memory? (e.g. after loading from file)
  bool isEqual (const CVector3 &v) const;
  /// numerically equal, use a tolerance like 1E-8
  bool isEqual (const CVector3 &v, float tolerance) const;
  void take (const CVector3 &v); // copy data from v
};

/**
* CVector2 stores the components of a 2D vector (x,y).
* Access x,y directly.
*/
class CVector2 
{
  public:
  /// x coordinate
  float x;
  /// y coordinate
  float y;
  
  CVector2 ();
  CVector2 (float x, float y);
  CVector2 (const CVector2 &v);
  virtual ~CVector2 ();
  
  void set (const CVector2 &v);
  void set (float x, float y);
  /// exactly equal in memory (no sense for comparisons)
  bool isEqual (const CVector2 &v) const;
  /// numerically equal, use a tolerance like 1E-8
  bool isEqual (const CVector2 &v, float tolerance) const;
  /// copy data from v
  void take (const CVector2 &v);
};

/**
* CVertex represents a vertex which may take information about
* color, location, normal, texture, number of surrounding triangles.
*/
class CVertex
{
  public:
  /// number of triangles to which this vertex belongs to
  int triangles;
  /// color, used when displayed without textures
  CColor color;
  /// position
  CVector3 vector;
  /// normal vector, interpolation of all surrounding triangles
  CVector3 normal;
  /// 2D texture coordinates
  CVector2 tex;
  
  CVertex ();
  CVertex (const CVertex &v);
  virtual ~CVertex ();
  
  /// the normal vector of a vertex can be calculated as average of all adjacent plane normals
  void addNormal (const CVector3 &n); 
  /// the color of a vertex can be calculated as average of all adjacent plane colors
  void addColor (const CColor &color);
  /// copy data from v
  void take (const CVertex &v);
};

/**
* CRotation stores one (x,y,z)-rotation.
* Access angles a,b,c directly.
*/
class CRotation
{
  public:
  /// rotation angle for a plane of the coordinate system
  float a;
  /// rotation angle for a plane of the coordinate system
  float b;
  /// rotation angle for a plane of the coordinate system
  float c;
  
  CRotation ();
  virtual ~CRotation ();
  
  void setAngles (float a, float b, float c);
  void addAngles (float a, float b, float c);
  void calcRotation ();
  float rotateX (const CVector3 &v) const;
  float rotateY (const CVector3 &v) const;
  float rotateZ (const CVector3 &v) const;
  void take (const CRotation &r);

  protected:
  /// the rotation matrix of this rotation
  float rot [3] [3];
};

/**
* CTriangle stores references to the vertices of the triangle/face.
*/
class CTriangle
{
  public:
  /// references to the three vertices
  CVertex *v [3];
  
  CTriangle ();
  virtual ~CTriangle ();
  
  void calcNormal (CVector3 *n);
  void setVertices (CVertex *a, CVertex *b, CVertex *c); // not const as a,b,c may be altered
};

/**
* CQuad stores references to the vertices of the quad/square/face.
*/
class CQuad
{
  public:
  CVertex *v [4]; // references to the four vertices
  
  CQuad ();
  virtual ~CQuad ();

  void calcNormal (CVector3 *n);
  void setVertices (CVertex *a, CVertex *b, CVertex *c, CVertex *d); // not const as a,b,c,d may be altered
};

/**
* Material stores the name, filename, color, and texture of a material
*/
class CMaterial
{
  public:
  /// unique name
  std::string name;
  /// unique file name
  std::string filename;
  /// uniform color
  CColor color;
  /// pointer to a texture (or NULL if there is no texture)
  CTexture *texture;
  /// tiling coordinates
  float utile;
  /// tiling coordinates
  float vtile;
  /// texture offset (the importer must calculate u due to offsets)
  float uoffset;
  /// texture offset (the importer must calculate v due to offsets)
  float voffset;
  /// texture scaling (the importer must calculate u due to scaling)
  float uscale;
  /// texture scaling (the importer must calculate v due to scaling)
  float vscale;
  /// rotation in degree (the importer must calculate u/v due to wrot)
  float wrot;
  
  CMaterial ();
  virtual ~CMaterial ();
};

/**
* CObject stores the material, vertices, and faces (triangles, quads) of an object
* which is a part of the whole model.
*/
class CObject
{
  public:
  Uint16 numVertices;
  Uint16 numTriangles;
  Uint16 numQuads;
  Uint16 numTexVertex;
  /// an object has one unique material
  CMaterial *material;
  /// an object can have one unique texture
  bool hasTexture;
  /// unique object name
  std::string name;
  /// vertex list
  CVertex *vertex;
  /// triangle list
  CTriangle *triangle;
  /// quad list
  CQuad *quad;
  
  CObject ();
  virtual ~CObject ();
  
  //// used to construct objects
  int addVertex (const CVertex &v);
  void setColor (const CColor &color);
};

/**
* CModel stores the materials and objects of a model, the data structure is optimized for 3DS files.
* TODO: make a general "Model" and a derived "Model3ds". Split draw() methods into a "Realizer".
*/
class CModel 
{
  public:
  /// unique model name like "GL-117"
  char name [20];
  int shading; // shading can be set to FLAT (0) or SMOOTH/GOURAUD (1)
  Uint16 numObjects; // number of objects (which have a unique material)
  Uint16 numMaterials; // number of materials (should be the same as the number of objects)
  bool displaylist; // enable using a display list
  CMaterial *material [100]; // materials, at most 100 (these are only pointers)
  CObject *object [100]; // objects, at most 100 (these are only pointers)
  bool nolight; // do not use light?
  bool alpha; // use alpha blending?
  int numRefpoints; // reference points for missiles
  CVector3 *refpoint; // obsolete
  float scale, scalex, scaley, scalez; // overall scaling of original 3DS coords
  // Note: the model is stored to (-1,-1,-1)-(1,1,1) in RAM, the SpaceObj tells about the scaling of a model
  float cubex, cubey, cubez; // surrounding cube (or radius of sphere) for simplified collision detection
  int list1, list2, list3; // display lists already generated for each type of draw() method

  CModel ();
  void setName (char *name);
  void addMaterial (CMaterial *material);
  void addObject (CObject *object);
  void addRefPoint (CVector3 *tl);
  ~CModel ();
  void setColor (CColor *col);
  void drawVertexNormals (CObject *cm, float zoom);
  int rotateColor (int n);
  void scaleTexture (float fx, float fy);
  // the drawing methods take the following parameters:
  // tl+tl2=translation, rot=rotation, lum=luminance (default 1.0), explode=radial translation (default 0)
  // draw everything
  void draw (CVector3 *tl, CVector3 *tl2, CRotation *rot, float zoom, float lum, int explode);
  // draw without GL lighting
  void draw2 (CVector3 *tl, CVector3 *tl2, CRotation *rot, float zoom, int explode);
  // draw without textures
  void draw3 (CVector3 *tl, CVector3 *tl2, CRotation *rot, float zoom, int explode);
  // draw without textures, different luminance
  void draw3 (CVector3 *tl, CVector3 *tl2, CRotation *rot, float zoom, float lum, int explode);

  protected:
  /// all private members are only used "temporarily" at runtime!
  int rotcol; // very special for flickering light sources, e.g. the engine's bright yellow color is rotated
  float light_ambient [4]; // special light source attributes
  float light_diffuse [4];
  float light_ambient2 [4];
  float light_diffuse2 [4];
  CVector3 tlnull;
  CRotation rotnull;
  VertexArray *va; // using a vertex array means more memory, but better performance
};

// CSphere represents an ellipsoid according to the CModel data structure
class CSphere : public CModel
{
  private:
  int random (int n);
  public:
  int segments; // segments on lateral angle, doubled for longitudinal angle
  float radius;
  float dx, dy, dz; // ellipsoid measures in the carthesian cosy
  CSphere ();
  CSphere (float radius, int segments, float dx, float dy, float dz);
  ~CSphere ();
  void init (float radius, int segments);
  void init (float radius, int segments, float dx, float dy, float dz, int randomized);
  void invertNormals (); // point outside/inside
  void setNorthPoleColor (CColor *c, float w); // see setPoleColor, phi=0, theta=90
  void setSouthPoleColor (CColor *c, float w);
  void setPoleColor (int phi, int theta, CColor *c, float w); // shade color over any pole
};

// Part of an ellipsoid
class CSpherePart : public CModel
{
  public:
  int segments; // segments on lateral angle, doubled for longitudinal angle
  float radius;
  CSpherePart ();
  CSpherePart (float radius, int segments, float phi);
  ~CSpherePart ();
  void init (float radius, int segments);
  void init (float radius, int segments, float phi);
  void setNorthPoleColor (CColor *c, float w); // see setPoleColor, phi=0, theta=90
  void setSouthPoleColor (CColor *c, float w);
  void setPoleColor (int phi, int theta, CColor *c, float w); // shade color over any pole
};

// More models could be added here, esp. for importing complex non-polygonal formats (obj, wrl)

#endif
