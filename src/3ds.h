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

/* This file includes a 3ds loader. Thanks to the great NeHe Tutorials! */

#ifndef IS_3DS_H
#define IS_3DS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "model.h"
#include "gl.h"

// Primary Chunk
#define PRIMARY       0x4D4D

// Main Chunks
#define OBJECTINFO    0x3D3D
#define VERSION       0x0002
#define EDITKEYFRAME  0xB000

// Sub defines of OBJECTINFO
#define MATERIAL	  0xAFFF
#define OBJECT		  0x4000

// Sub defines of MATERIAL
#define MATNAME       0xA000
#define MATDIFFUSE    0xA020
#define MATMAP        0xA200
#define MATMAPFILE    0xA300

// Read new object
#define OBJECT_MESH   0x4100

// Sub defines of OBJECT_MESH
#define OBJECT_VERTICES     0x4110
#define OBJECT_FACES		0x4120
#define OBJECT_MATERIAL		0x4130
#define OBJECT_UV			0x4140

// Structure for the 3DS indicies
struct Indices {
  unsigned short a, b, c, bVisible;
};

// Chunk info
struct Chunk
{
  unsigned short int ID;
  unsigned int length;
  unsigned int bytesRead;
};

// CLoad3DS handles all of the loading code (see NEHE's OpenGL tutorials)
class CLoad3DS
{
  public:
  CLoad3DS ();
  bool Import3DS (CModel *model, char *filename);
  
  private:
  int GetString (char *);
  void ReadChunk (Chunk *);
  void ProcessNextChunk (CModel *model, Chunk *);
  void ProcessNextObjectChunk (CModel *model, CObject *object, Chunk *);
  void ProcessNextMaterialChunk (CModel *model, Chunk *);
  void ReadColorChunk (CMaterial *material, Chunk *pChunk);
  void ReadVertices (CObject *object, Chunk *);
  void ReadVertexIndices (CObject *object, Chunk *);
  void ReadUVCoordinates (CObject *object, Chunk *);
  void ReadObjectMaterial (CModel *model, CObject *object, Chunk *previousChunk);
  void Compile (CModel *model);
  void ComputeNormals (CModel *model);
  void ComputeColors (CModel *model);
  void LoadTextures (CModel *model);
  void Normalize (CModel *model);
  void CleanUp ();
  FILE *filepointer;
  Chunk *currentChunk;
  Chunk *tempChunk;
};

#endif

