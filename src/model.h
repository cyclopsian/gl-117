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

/* Coding:
   - do not overload operators, it's crucial to differ from pointer assignments and fast ops
   - do not write destructors, as arrays of these classes should be treatable as float* array
     thus never make a destructor virtual as the virtptr is also somewhere in memory */

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
  
    void set (const CColor &color);
    void set (int red, int green, int blue, int alpha = 255);
    bool isEqual (const CColor &color) const; // compare colors
    void take (const CColor &color);
};

/**
* CTexture loads and stores a texture to memory.
* To use a texture, it must be loaded and added to the OpenGL texture list using
*   gl->genTextureTGA ();
* instead of this class!
*/
class CTexture
{
  public:

    unsigned char *data; ///< texture color data as array, 32bpp, RGBA
    std::string name;    ///< texture file name
    bool mipmap;         ///< mipmapping on/off depending on gluBuildMipmaps
    int textureID;       ///< texture ID of native GL code
    int width;           ///< texture width
    int height;          ///< texture height
    // average values to substitute textured quads by colors for LOD
    float texlight;      ///< average of texture's overall brightness
    float texred;        ///< average of texture's red
    float texgreen;      ///< average of texture's green
    float texblue;       ///< average of texture's blue
    bool alpha;          ///< alpha blending on/off
  
    CTexture ();
    CTexture (const std::string &filename, int alphaprogram = -1,
              bool mipmap = true, bool alpha = false); // TODO: alpha really necessary?
    ~CTexture (); // necessary
  
    /// loadFromTGA is called via gl->genTextureTGA() to not load the same texture twice
    bool loadFromTGA (const std::string &filename, int alphaprogram = -1,
                      bool mipmap = true, bool alpha = false);
    /// get color of a special pixel
    void getColor (CColor *c, int x, int y) const;
    /// set to linear shading between texels (expensive esp. on old graphic cards)
    void shadeLinear () const;
    /// set to const shading between texels (fast)
    void shadeConst () const;
};

/**
* CVector3 stores the components of a 3D vector (x,y,z).
* Access x,y,z directly.
*/
class CVector3 
{
  public:
  
    float x; ///< x coordinate
    float y; ///< y coordinate
    float z; ///< z coordinate
  
    CVector3 ();
    CVector3 (float x, float y, float z);
    CVector3 (const CVector3 &v);
  
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
};

/**
* CVector2 stores the components of a 2D vector (x,y).
* Access x,y directly.
*/
class CVector2 
{
  public:
    
    float x; ///< x coordinate
    float y; ///< y coordinate
  
    CVector2 ();
    CVector2 (float x, float y);
    CVector2 (const CVector2 &v);
  
    void set (const CVector2 &v);
    void set (float x, float y);
    /// exactly equal in memory (no sense for comparisons)
    bool isEqual (const CVector2 &v) const;
    /// numerically equal, use a tolerance like 1E-8
    bool isEqual (const CVector2 &v, float tolerance) const;
};

/**
* CVertex represents a vertex which may take information about
* color, location, normal, texture, number of surrounding triangles.
*/
class CVertex
{
  public:

    int triangles;   ///< number of triangles to which this vertex belongs to
    CColor color;    ///< color, used when displayed without textures
    CVector3 vector; ///< position
    CVector3 normal; ///< normal vector, interpolation of all surrounding triangles
    CVector2 tex;    ///< 2D texture coordinates
  
    CVertex ();
    CVertex (const CVertex &v);
  
    /// copy data from v
    void set (const CVertex &v);
    /// the normal vector of a vertex can be calculated as average of all adjacent plane normals
    void addNormal (const CVector3 &n); 
    /// the color of a vertex can be calculated as average of all adjacent plane colors
    void addColor (const CColor &c);
};

/**
* CRotation stores one (x,y,z)-rotation.
*/
class CRotation
{
  public:
  
    float a; ///< rotation angle for a plane of the coordinate system
    float b; ///< rotation angle for a plane of the coordinate system
    float c; ///< rotation angle for a plane of the coordinate system
  
    CRotation ();
  
    void set (const CRotation &r);
    void set (float a, float b, float c);
    void add (const CRotation &r);
    void add (float a, float b, float c);
/*    void calculate (); // calculate the rotation matrix
    float rotateX (const CVector3 &v) const;
    float rotateY (const CVector3 &v) const;
    float rotateZ (const CVector3 &v) const;*/

  protected:

    // the rotation matrix of this rotation
//    float rot [3] [3];
};

/**
* CTriangle stores references to the vertices of the triangle/face.
*/
class CTriangle
{
  public:
    
    CVertex *v [3]; ///< references to the three vertices
  
    CTriangle ();
  
    void calcNormal (CVector3 *n); ///< return normal n
    void setVertices (CVertex *a, CVertex *b, CVertex *c); // not const as a,b,c may be altered
};

/**
* CQuad stores references to the vertices of the quad/square/face.
*/
class CQuad
{
  public:

    CVertex *v [4]; ///< references to the four vertices
  
    CQuad ();

    void calcNormal (CVector3 *n); ///< return normal n
    void setVertices (CVertex *a, CVertex *b, CVertex *c, CVertex *d); // not const as a,b,c,d may be altered
};

/**
* CMaterial stores the name, filename, color, and texture of a material.
*/
class CMaterial
{
  public:

    std::string name;     ///< unique name
    std::string filename; ///< unique file name
    CColor color;         ///< uniform color
    CTexture *texture;    ///< reference to a texture (or NULL if there is no texture)
    float utile;          ///< tiling coordinates
    float vtile;          ///< tiling coordinates
    float uoffset;        ///< texture offsets (the importer must calculate u/v due to offsets)
    float voffset;        ///< texture offsets (the importer must calculate u/v due to offsets)
    float uscale;         ///< texture scaling (the importer must calculate u/v due to scaling)
    float vscale;         ///< texture scaling (the importer must calculate u/v due to scaling)
    float wrot;           ///< rotation in degree (the importer must calculate u/v due to wrot)
  
    CMaterial ();
};

/**
* CObject stores the material, vertices, and faces (triangles, quads) of an object.
*/
class CObject
{
  public:

    Uint16 numVertices;
    Uint16 numTriangles;
    Uint16 numQuads;
    Uint16 numTexVertex;
    std::string name;    ///< unique object name
    CMaterial *material; ///< an object has one unique material
    bool hasTexture;     ///< an object can have one unique texture
    CVertex *vertex;     ///< vertex list
    CTriangle *triangle; ///< triangle list
    CQuad *quad;         ///< quad list

    CObject ();

    int addVertex (const CVertex &w); ///< used to construct objects
    void setColor (const CColor &col);
};

/**
* CModel stores the materials and objects of a model.
* Its data structure is optimized esp. for 3DS files.
* The drawing methods take the following parameters:
*   tl+tl2=translation,
*   rot=rotation,
*   lum=luminance (default 1.0),
*   explode=radial translation (default 0)
*/
class CModel 
{
  public:

    std::string name;    ///< unique model name like "GL-117"
    int shading;         ///< shading can be set to FLAT (0) or SMOOTH/GOURAUD (1)
    Uint16 numObjects;   ///< number of objects (which have a unique material)
    Uint16 numMaterials; ///< number of materials (should be the same as the number of objects)
    bool displaylist;    ///< enable using a display list
    CMaterial *material [100]; ///< materials, at most 100 (these are only pointers)
    CObject *object [100];     ///< objects, at most 100 (these are only pointers)
    bool nolight;        ///< do not use light?
    bool alpha;          ///< use alpha blending?
    int numRefpoints;    ///< number of reference points for missiles
    CVector3 *refpoint;  ///< multiple references to missile rack positions
    float scale;         ///< overall scaling of original 3DS coords
    CVector3 vscale;     ///< scaling of the original 3DS coords in x,y,z
    // Note: the model is stored to (-1,-1,-1)-(1,1,1) in RAM, the SpaceObj tells about the scaling of a model
    CVector3 cube;       ///< surrounding cube (or radius of sphere) for simplified collision detection
    int list1, list2, list3;   ///< display lists already generated for each type of draw() method

    CModel ();
    ~CModel ();
  
    void setName (const std::string name);
    void addMaterial (const CMaterial &material);
    void addObject (const CObject &object);
    void addRefPoint (const CVector3 &tl);
    void setColor (const CColor &col);
    void drawVertexNormals (const CObject &cm, float zoom);
    int rotateColor (int n);
    void scaleTexture (float fx, float fy);
    /// draw everything
    void draw (const CVector3 &tl, const CVector3 &tl2, const CRotation &rot,
		       float zoom = 1.0F, float lum = 1.0F, int explode = 0);
    /// draw without GL lighting, no luminance => VERY FAST (uses display list)
    void drawNoLight (const CVector3 &tl, const CVector3 &tl2, const CRotation &rot,
		              float zoom = 1.0F, int explode = 0);
    /// draw without textures, different luminance
    void drawNoTexture (const CVector3 &tl, const CVector3 &tl2, const CRotation &rot,
		                float zoom = 1.0F, float lum = 1.0F, int explode = 0);

  private:

    // all private members are only used "temporarily" at runtime!
    int rotcol; ///< very special for flickering light sources, e.g. the engine's bright yellow color is rotated
    VertexArray *va; ///< using a vertex array means more memory, but better performance
};

/**
* CSphere represents an ellipsoid according to the CModel data structure.
*/
class CSphere : public CModel
{
  public:

    int segments;     ///< segments on lateral angle, doubled for longitudinal angle
    float radius;
    float dx, dy, dz; ///< ellipsoid measures in the carthesian cosy

    CSphere ();
    CSphere (float radius, int segments, float dx, float dy, float dz);
    ~CSphere ();

    void init (float radius, int segments, float dx = 1.0F, float dy = 1.0F, float dz = 1.0F, int randomized = 0);
    void invertNormals (); ///< point outside/inside
    void setNorthPoleColor (const CColor &c, float w); ///< see setPoleColor, phi=0, theta=90
    void setSouthPoleColor (const CColor &c, float w);
    void setPoleColor (int phi, int theta, const CColor &c, float w); ///< shade color over any pole

  private:

    int random (int n);
};

/**
* Part of an ellipsoid.
*/
class CSpherePart : public CModel
{
  public:

    int segments; ///< segments on lateral angle, doubled for longitudinal angle
    float radius;

    CSpherePart ();
    CSpherePart (float radius, int segments, float phi);
    ~CSpherePart ();

    void init (float radius, int segments, float phi = 10.0F);
    void setNorthPoleColor (const CColor &c, float w); ///< see setPoleColor, phi=0, theta=90
    void setSouthPoleColor (const CColor &c, float w);
    void setPoleColor (int phi, int theta, const CColor &c, float w); ///< shade color over any pole
};

// More models could be added here, esp. for importing complex non-polygonal formats (obj, wrl)

#endif
