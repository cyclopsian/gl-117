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
#define IS_GL_H

#include <iostream>

class GlPrimitives
{
  public:

    unsigned char fogColor [3]; ///< the fog color in RGB (each 1 byte)
    float fogLuminance;         ///< the fog luminance in [0;1], multiplied with fogColor

    GlPrimitives ();
    ~GlPrimitives ();

    /// clear all buffers
    void clearBuffers () const;
    /// swap buffers to display the next buffer's content
    void swapBuffers () const;
    /// rotate using glRotatef for the 3 axes (1,0,0),(0,1,0),(0,0,1)
    void rotate (float x, float y, float z);
    /// call display list => draw content immediately
    void callList (int list) const;
    /// create a new display list and return the list number (identifier)
    int createList ();
    /// bind texture and enable linear shading between texels (slow)
    void enableLinearTexture (int texnum, bool mipmap) const;
    /// bind texture and enable const shading between texels (fast)
    void disableLinearTexture (int texnum, bool mipmap) const;
    /// test if the texture already exists, in that case return the OpenGL texture id,
    /// otherwise create an OpenGL texture and return the id
    int registerTexture (const std::string &name, const unsigned char *data,
                         int width, int height, bool mipmap);
    /// enable standard antialiasing on points, lines, etc.
    void enableAntiAliasing () const;
    void disableAntiAliasing () const;
    /// enable standard alpha blending
    void enableAlphaBlending () const;
    void disableAlphaBlending () const;
    /// enable alpha testing, which means texels less than "intensity" are NOT rendered
    void enableAlphaTesting (float intensity) const;
    void disableAlphaTesting () const;
    /// bind texture and enable texture mapping
    void enableTexture (int num) const;
    void enableLighting () const;
    void disableLighting () const;
    /// enable standard z-buffer operation
    void enableZBuffer () const;
    void disableZBuffer () const;
    void shadeFlat () const;
    void shadeSmooth () const;
    void enableFog (float view, bool fast = true) const;
    void setFogColor (int r, int g, int b);
  
    void extractFrustum ();
    bool isPointInFrustum (float x, float y, float z);
    bool isSphereInFrustum (float x, float y, float z, float radius);
    bool isCubeInFrustum (float x, float y, float z, float size);

  private:

    std::string texList [200]; ///< texture names
    int numTextures;           ///< texture counter
    int numLists;              ///< list counter
    float frustum [6] [4];     ///< the six frustum (cone) planes
};

extern GlPrimitives *gl;

#endif
