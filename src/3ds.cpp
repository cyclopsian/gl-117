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

#include "3ds.h"

BinaryFile::BinaryFile (char *filename)
{
  in = fopen (filename, "rb");
  if (in == NULL)
  {
    fprintf (stderr, "\nCannot open file %s!", filename);
    fflush (stderr);
    return;
  }
  fseek (in, 0, SEEK_END);
  size = ftell (in);
  fseek (in, 0, SEEK_SET);
  data = new unsigned char [size];
  Uint32 z = 0;
  while (!feof (in))
  {
    fread (&data [z], 1, 4096, in);
    z += 4096;
  }
  fclose (in);
  filepointer = 0;
}

BinaryFile::~BinaryFile ()
{
  delete data;
}

int BinaryFile::readFloat (float *f)
{
#ifdef WORDS_BIGENDIAN
  ret [0] = data [filepointer + 3];
  ret [1] = data [filepointer + 2];
  ret [2] = data [filepointer + 1];
  ret [3] = data [filepointer];
  ret [4] = 0;
  *f = *((float *) ret);
#else
  *f = *((float *) &data [filepointer]);
#endif
  filepointer += 4;
  return 4;
}

int BinaryFile::readFloat (float *f, int n)
{
  int i;
  for (i = 0; i < n; i ++)
  {
    readFloat (&f [i]);
  }
  return n * 4;
}

int BinaryFile::readUInt32 (Uint32 *i)
{
#ifdef WORDS_BIGENDIAN
  ret [0] = data [filepointer + 3];
  ret [1] = data [filepointer + 2];
  ret [2] = data [filepointer + 1];
  ret [3] = data [filepointer];
  ret [4] = 0;
  *i = *((Uint32 *) ret);
#else
  *i = *((Uint32 *) &data [filepointer]);
#endif
  filepointer += 4;
  return 4;
}

int BinaryFile::readUInt16 (Uint16 *i)
{
#ifdef WORDS_BIGENDIAN
  ret [0] = data [filepointer + 1];
  ret [1] = data [filepointer + 0];
  ret [2] = 0;
  *i = *((Uint16 *) ret);
#else
  *i = *((Uint16 *) &data [filepointer]);
#endif
  filepointer += 2;
  return 2;
}

int BinaryFile::readString (char *ptr, int n)
{
  if (filepointer + n > size)
    n = size - filepointer;
  memcpy (ptr, &data [filepointer], n);
  filepointer += n;
  return n;
}

int BinaryFile::readString (char *ptr)
{
  int i = 0;
  while (data [filepointer] != 0 && filepointer < size)
  {
    ptr [i] = data [filepointer];
    i ++;
    filepointer ++;
  }
  ptr [i] = 0;
  filepointer ++;
  i ++;
  return i;
}

/****************************************************************************
3DS LOADER
****************************************************************************/

// Contructor -> initialization
CLoad3DS::CLoad3DS ()
{
  currentChunk = new Chunk;
  tempChunk = new Chunk;
}

// Load the whole 3DS file using the private functions below
bool CLoad3DS::Import3DS (CModel *model, char *filename)
{
  char message [255] = {0};

  file = new BinaryFile (filename);

  // Read the first chuck
  ReadChunk(currentChunk);
  // Make sure this is a 3DS file
  if (currentChunk->ID != PRIMARY)
  {
    sprintf (message, "\nUnable to load PRIMARY chuck from file: %s!", filename);
    fprintf (stderr, "%s", message);
    fflush (stdout);
    return false;
  }
  // Load objects recursively
  ProcessNextChunk (model, currentChunk);
  // Merge equal vertices
  Compile (model);
  // Calculate vertex normals
  ComputeNormals (model);
  // Load textures
  LoadTextures (model);
  // Set vertex colors using textures
  ComputeColors (model);
  // Set vertices to [(-1,-1,-1);(1,1,1)]
  Normalize (model);
  // Clean up
  CleanUp();

  return true;
}

void CLoad3DS::CleanUp ()
{
  delete file;
//  fclose (filepointer);            // Close the current file pointer
/* Vorsicht: Speicherleck !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  //delete currentChunk;            // Free the current chunk
  //delete tempChunk;              // Free our temporary chunk
}

void CLoad3DS::ProcessNextChunk (CModel *model, Chunk *previousChunk)
{
  char version [10];
  CObject newObject;
  CMaterial newTexture;
  char buffer [50000] = {0};

  currentChunk = new Chunk;

  while (previousChunk->bytesRead < previousChunk->length)
  {
    ReadChunk(currentChunk);

    switch (currentChunk->ID)
    {
      case VERSION:
        currentChunk->bytesRead += file->readString (version, currentChunk->length - currentChunk->bytesRead);
        if (version [0] > 0x03)
        {
          fprintf (stderr, "\nWarning: This 3DS file is over version 3 so it may load incorrectly");
          fflush (stdout);
        }
        break;

      case OBJECTINFO:
        ReadChunk (tempChunk);
        tempChunk->bytesRead += file->readString (version, tempChunk->length - tempChunk->bytesRead);
        currentChunk->bytesRead += tempChunk->bytesRead;
        ProcessNextChunk (model, currentChunk);
        break;

      case MATERIAL:
        model->addMaterial (&newTexture);
        ProcessNextMaterialChunk (model, currentChunk);
        break;

      case OBJECT:
        model->addObject (&newObject);
        memset (&newObject, 0, sizeof (CObject));
        currentChunk->bytesRead += GetString (model->object [model->numObjects - 1]->name);
        ProcessNextObjectChunk (model, (model->object [model->numObjects - 1]), currentChunk);
        break;

      case EDITKEYFRAME:
        currentChunk->bytesRead += file->readString ((char *) buffer, currentChunk->length - currentChunk->bytesRead);
        break;

      default: 
        currentChunk->bytesRead += file->readString ((char *) buffer, currentChunk->length - currentChunk->bytesRead);
        break;
    }

    previousChunk->bytesRead += currentChunk->bytesRead;
  }
  delete currentChunk;
  currentChunk = previousChunk;
}

void CLoad3DS::ProcessNextObjectChunk (CModel *model, CObject *object, Chunk *previousChunk)
{
  char buffer[50000] = {0};

  currentChunk = new Chunk;

  while (previousChunk->bytesRead < previousChunk->length)
  {
    ReadChunk(currentChunk);

    switch (currentChunk->ID)
    {
      case OBJECT_MESH:
        ProcessNextObjectChunk (model, object, currentChunk);
        break;

      case OBJECT_VERTICES:
        ReadVertices (object, currentChunk);
        break;

      case OBJECT_FACES: // triangles
        ReadVertexIndices (object, currentChunk);
        break;

      case OBJECT_MATERIAL:
        ReadObjectMaterial (model, object, currentChunk);      
        break;

      case OBJECT_UV:
        ReadUVCoordinates (object, currentChunk);
        break;

      default:  
        currentChunk->bytesRead += file->readString ((char *) buffer, currentChunk->length - currentChunk->bytesRead);
        break;
    }

    previousChunk->bytesRead += currentChunk->bytesRead;
  }

  delete currentChunk;
  currentChunk = previousChunk;
}

void CLoad3DS::ProcessNextMaterialChunk (CModel *model, Chunk *previousChunk)
{
  char buffer[50000] = {0};

  currentChunk = new Chunk;

  while (previousChunk->bytesRead < previousChunk->length)
  {
    ReadChunk(currentChunk);

    switch (currentChunk->ID)
    {
      case MATNAME:
        currentChunk->bytesRead += file->readString (model->material [model->numMaterials - 1]->name, currentChunk->length - currentChunk->bytesRead);
        break;

      case MATDIFFUSE:
        ReadColorChunk ((model->material [model->numMaterials - 1]), currentChunk);
        break;
    
      case MATMAP:
        ProcessNextMaterialChunk (model, currentChunk);
        break;

      case MATMAPFILE:
        currentChunk->bytesRead += file->readString (model->material [model->numMaterials - 1]->filename, currentChunk->length - currentChunk->bytesRead);
        {
          char* str=model->material [model->numMaterials - 1]->filename;
          while (*str)
          {
            if (*str >= 'A' && *str <= 'Z')
              *str = tolower (*str);
            str++;
          }
        }
        break;
    
      default:  
        currentChunk->bytesRead += file->readString ((char *) buffer, currentChunk->length - currentChunk->bytesRead);
        break;
    }

    previousChunk->bytesRead += currentChunk->bytesRead;
  }

  delete currentChunk;
  currentChunk = previousChunk;
}

void CLoad3DS::ReadChunk (Chunk *pChunk)
{
//  unsigned char buf [10];
  pChunk->bytesRead = file->readUInt16 (&pChunk->ID);
  pChunk->bytesRead += file->readUInt32 (&pChunk->length);
}

int CLoad3DS::GetString (char *buffer)
{
  return file->readString (buffer);
}

void CLoad3DS::ReadColorChunk (CMaterial *material, Chunk *pChunk)
{
  ReadChunk (tempChunk);
  tempChunk->bytesRead += file->readString ((char *) material->color.c, tempChunk->length - tempChunk->bytesRead);
  pChunk->bytesRead += tempChunk->bytesRead;
}

void CLoad3DS::ReadVertexIndices (CObject *object, Chunk *previousChunk)
{
  Uint16 index = 0;
  previousChunk->bytesRead += file->readUInt16 ((Uint16 *) &object->numTriangles);

  object->triangle = new CTriangle [object->numTriangles];
  memset (object->triangle, 0, sizeof (CTriangle) * object->numTriangles);

  for (int i = 0; i < object->numTriangles; i ++)
  {
    for (int j = 0; j < 4; j++)
    {
      previousChunk->bytesRead += file->readUInt16 (&index);
      if (j < 3)
      {
        object->triangle [i].v [j] = &object->vertex [index];
      }
    }
  }
}


void CLoad3DS::ReadUVCoordinates (CObject *object, Chunk *previousChunk)
{
  previousChunk->bytesRead += file->readUInt16 ((Uint16 *) &object->numTexVertex);

  CVector2 *p = new CVector2 [object->numTexVertex];

  previousChunk->bytesRead += file->readFloat ((float *) p, (previousChunk->length - previousChunk->bytesRead) / 4);
  
  for (int i = 0; i < object->numTexVertex; i ++)
    object->vertex [i].tex.take (&p [i]);
  delete p;
}

void CLoad3DS::ReadVertices (CObject *object, Chunk *previousChunk)
{
  int i;
  previousChunk->bytesRead += file->readUInt16 ((Uint16 *) &object->numVertices);

  object->vertex = new CVertex [object->numVertices];
  memset (object->vertex, 0, sizeof (CVertex) * object->numVertices);

  CVector3 *p = new CVector3 [object->numVertices];

  previousChunk->bytesRead += file->readFloat ((float *) p, (previousChunk->length - previousChunk->bytesRead) / 4);
    
  for (i = 0; i < object->numVertices; i ++)
  {
    object->vertex [i].vector.take (&p [i]);
  }

  // Flip the y values with the z values as 3DMAX changed them and negate z
  for (i = 0; i < object->numVertices; i ++)
  {
    float fTempY = object->vertex [i].vector.y;
    object->vertex [i].vector.y = object->vertex [i].vector.z;
    object->vertex [i].vector.z = -fTempY;
  }
  delete p;
}

void CLoad3DS::ReadObjectMaterial (CModel *model, CObject *object, Chunk *previousChunk)
{
  char materialName [255] = {0};
  char buffer [50000] = {0};
  previousChunk->bytesRead += GetString (materialName);

  for (int i = 0; i < model->numMaterials; i ++)
  {
    if (strcmp (materialName, model->material [i]->name) == 0)
    {
      object->material = model->material [i];
      if (strlen (model->material [i]->filename) > 0)
        if (model->material [i]->filename [0] >= '0' && model->material [i]->filename [0] <= 'z')
        {
          object->hasTexture = true;
        }  
      break;
    }
    else
    {
      object->material = NULL;
    }
  }

  previousChunk->bytesRead += file->readString ((char *) buffer, previousChunk->length - previousChunk->bytesRead);
}      

void CLoad3DS::Compile (CModel *model)
{
  for (int i = 0; i < model->numObjects; i ++)
  {
    CObject *co = model->object [i];
    for (int i2 = 1; i2 < co->numVertices; i2 ++)
    {
      for (int i3 = 0; i3 < i2; i3 ++)
      {
        if (co->vertex [i2].vector.isEqual (&co->vertex [i3].vector))
        {
          for (int i4 = 0; i4 < co->numTriangles; i4 ++)
          {
            if (co->triangle [i4].v [0] == &co->vertex [i2]) co->triangle [i4].v [0] = &co->vertex [i3];
            if (co->triangle [i4].v [1] == &co->vertex [i2]) co->triangle [i4].v [1] = &co->vertex [i3];
            if (co->triangle [i4].v [2] == &co->vertex [i2]) co->triangle [i4].v [2] = &co->vertex [i3];
          }
        }
      }
    }
  }
}

void CLoad3DS::ComputeNormals (CModel *model)
{
  int i, i2, i3;

  if (model->numObjects <= 0)
    return;

  CVector3 n;
  for (i = 0; i < model->numObjects; i ++)
  {
    CObject *object = (model->object [i]);
    for (i2 = 0; i2 < object->numTriangles; i2 ++)
    {
      object->triangle [i2].getNormal (&n);
      for (i3 = 0; i3 < 3; i3 ++)
      {
        object->triangle [i2].v [i3]->addNormal (&n);
      }
    }
    for (i2 = 0; i2 < object->numTriangles; i2 ++)
    {
      for (i3 = 0; i3 < 3; i3 ++)
      {
        object->triangle [i2].v [i3]->normal.norm ();
      }
    }
  }

}

void CLoad3DS::LoadTextures (CModel *model)
{
  int i, i2;
  char str [256];
  for (i = 0; i < model->numObjects; i ++)
  {
    if (model->object [i]->hasTexture)
    {
      strcpy (str, dirs->getTextures (model->object [i]->material->filename));
//			strcat (str, model->object [i]->material->filename);
			for (i2 = (int) strlen (str) - 1; i2 >= 0; i2 --)
			{
        if (i2 > 2 && str [i2] == '.')
        {
          str [i2+1] = 't';
          str [i2+2] = 'g';
          str [i2+3] = 'a';
          str [i2+4] = 0;
          break;
        }
      }
      
      model->object [i]->material->texture = gl->genTextureTGA (str, 0, -1, 1, false);
    }
  }
}

void CLoad3DS::ComputeColors (CModel *model)
{
  int i, i2;

  if (model->numObjects <= 0)
    return;

  CColor c;
  for (i = 0; i < model->numObjects; i ++)
  {
    CObject *object = (model->object [i]);
    for (i2 = 0; i2 < object->numVertices; i2 ++)
    {
      if (object->hasTexture)
      {
        CVertex *v = &object->vertex [i2];
        CTexture *tex = object->material->texture;
        tex->getColor (&c, (int) (v->tex.x * tex->width), (int) (v->tex.y * tex->height));
        int val;
        if (c.c [0] < 200 || c.c [1] < 200)
        {
          val = c.c [0]; val = val * 2 / 3; c.c [0] = val;
          val = c.c [1]; val = val * 2 / 3; c.c [1] = val;
          val = c.c [2]; val = val * 2 / 3; c.c [2] = val;
        }
        v->color.take (&c);
      }
      else
      {
        object->vertex [i2].color.c [0] = object->material->color.c [0];
        object->vertex [i2].color.c [1] = object->material->color.c [1];
        object->vertex [i2].color.c [2] = object->material->color.c [2];
      }
    }
  }

}

void CLoad3DS::Normalize (CModel *model)
{
  int i, i2;
  float minx = 1E10, miny = 1E10, minz = 1E10;
  float maxx = -1E10, maxy = -1E10, maxz = -1E10;
  for (i = 0; i < model->numObjects; i ++)
  {
    CObject *object = (model->object [i]);
    for (i2 = 0; i2 < object->numVertices; i2 ++)
    {
      CVertex *v = &object->vertex [i2];
      if (v->vector.x > maxx) maxx = v->vector.x;
      if (v->vector.y > maxy) maxy = v->vector.y;
      if (v->vector.z > maxz) maxz = v->vector.z;
      if (v->vector.x < minx) minx = v->vector.x;
      if (v->vector.y < miny) miny = v->vector.y;
      if (v->vector.z < minz) minz = v->vector.z;
    }
  }

  float tlx = (maxx + minx) / 2.0;
  float tly = (maxy + miny) / 2.0;
  float tlz = (maxz + minz) / 2.0;
  float scx = (maxx - minx) * 0.5;
  float scy = (maxy - miny) * 0.5;
  float scz = (maxz - minz) * 0.5;
  float sc = scx > scy ? scx : scy;
  sc = scz > sc ? scz : sc; 

  for (i = 0; i < model->numObjects; i ++)
  {
    CObject *object = (model->object [i]);
    for (i2 = 0; i2 < object->numVertices; i2 ++)
    {
      CVertex *v = &object->vertex [i2];
      v->vector.x -= tlx;
      v->vector.x /= sc;
      v->vector.y -= tly;
      v->vector.y /= sc;
      v->vector.z -= tlz;
      v->vector.z /= sc;
    }
  }
};

#endif

