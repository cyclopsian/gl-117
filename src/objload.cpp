/* This file includes a wavefront obj loader. */

#ifndef IS_OBJLOAD_H

#include "objload.h"

CLoadOBJ::CLoadOBJ ()
{
}

// trim string and return a floating point value
float CLoadOBJ::ReadValue (int offset)
{
  int i = offset;
  while (data [i] == ' ' || data [i] == '\t') i ++;
  offset = i;
  while ((data [i] >= '0' && data [i] <= '9') || data [i] == '.' || data [i] == '-') i ++;
  data [i] = 0;
  filepointer = i + 1;
  char *p1;
  char **p2 = &p1;
  return (float) strtod (&data [offset], p2);
}

// trim string and return an integer value
int CLoadOBJ::ReadInteger (int offset)
{
  int i = offset;
  while (data [i] == ' ' || data [i] == '\t') i ++;
  offset = i;
  while ((data [i] >= '0' && data [i] <= '9') || data [i] == '-') i ++;
  data [i] = 0;
  filepointer = i + 1;
  char *p1;
  char **p2 = &p1;
  return (int) atoi (&data [offset]);
}

// load the complete file to the "data" buffer
void CLoadOBJ::PreLoad (char *filename)
{
  FILE *in = fopen (filename, "rb");
  if (in == NULL) // file could not be opened
  {
    fprintf (stderr, "Kann Datei %s nicht laden!\n", filename);
    fflush (stderr);
    return;
  }
  fseek (in, 0, SEEK_END); // get file size
  size = ftell (in);
  fseek (in, 0, SEEK_SET);
  data = new char [size]; // new data array
  long z = 0;
  while (!feof (in)) // read file in 4096 block, fastest on most systems (esp. MSWindows)
  {
    fread (&data [z], 4096, sizeof (char), in);
    z += 4096;
  }
  fclose (in);
  filepointer = 0; // set filepointer of data array to the start
  verticesread = 0;
}

// load the complete model
void CLoadOBJ::LoadObject (CModel *model)
{
  int i = filepointer;
  int vertices = 0, texturecoords = 0, normals = 0, faces = 0;

  // first count the vertices, faces, normals needed
  while (i < size)
  {
    while (data [i] != 'v' && data [i] != 'f' && i < size) i ++;
    if (data [i] == 'v')
    {
      if (data [i + 1] == ' ' || data [i + 1] == '\t') vertices ++;
      if (data [i + 1] == 't') texturecoords ++;
      if (data [i + 1] == 'n') normals ++;
    }
    if (data [i] == 'f')
    {
      if (data [i + 1] == ' ' || data [i + 1] == '\t')
      { faces ++; goto weiter1; }
    }
    i ++;
  }
weiter1:;
  while (i < size)
  {
    while (data [i] != 'v' && data [i] != 'f' && i < size) i ++;
    if (data [i] == 'f')
    {
      if (data [i + 1] == ' ' || data [i + 1] == '\t') faces ++;
    }
/*  if (data [i] == 'v')
    {
      if (data [i + 1] == ' ' || data [i + 1] == '\t') goto weiter2;
      if (data [i + 1] == 't') goto weiter2;
      if (data [i + 1] == 'n') goto weiter2;
    }*/
    i ++;
  }
//weiter2:;

  // create a new CObject
  CVector3 *n = new CVector3 [normals];
  model->object [aktobject] = new CObject;
  model->numObjects ++;
  CObject *object = model->object [aktobject];
  aktobject ++;
  object->numVertices = vertices;
  object->numTriangles = faces;
  object->numQuads = faces;

  // create array of vertices and faces (triangles, quads)
  object->vertex = new CVertex [object->numVertices];
  object->triangle = new CTriangle [object->numTriangles];
  object->quad = new CQuad [object->numQuads];

  // now load the vertices and faces
  i = filepointer;
  int z = 0, z2 = 0;
  while (i < size)
  {
    while (data [i] != 'v' && data [i] != 'f' && i < size) i ++;
    if (data [i] == 'v')
    {
      if (data [i + 1] == ' ' || data [i + 1] == '\t')
      {
        object->vertex [z].vector.x = ReadValue (i + 2);
        object->vertex [z].vector.y = ReadValue (filepointer);
        object->vertex [z].vector.z = ReadValue (filepointer);
        i = filepointer;
        z ++;
        i -= 2;
      }
      else if (data [i + 1] == 'n')
      {
        n [z2].x = ReadValue (i + 2);
        n [z2].y = ReadValue (filepointer);
        n [z2].z = ReadValue (filepointer);
        i = filepointer;
        z2 ++;
        i -= 2;
      }
//      if (data [i + 1] == 't') texturecoords ++;
//      if (data [i + 1] == 'n') normals ++;
    }
    if (data [i] == 'f')
    {
      if (data [i + 1] == ' ' || data [i + 1] == '\t')
      { i --; goto weiter3; }
    }
    i ++;
  }
weiter3:;
  int qn = 0, tn = 0;
  while (i < size)
  {
    while (data [i] != 'v' && data [i] != 'f' && i < size) i ++;
    if (data [i] == 'f')
    {
      if (data [i + 1] == ' ' || data [i + 1] == '\t')
      {
        int a = ReadInteger (i + 1) - verticesread;
        while (data [filepointer] != '/' && filepointer < size - 1) filepointer ++;
        if (filepointer >= size - 1)
          goto weiter4;
        int na = ReadInteger (filepointer + 1) - normalread;
        i = filepointer;
        while (data [i] == ' ' || data [i] == '\t') i ++;
        if (i >= size)
          i = i;
        int b = ReadInteger (i) - verticesread;
        while (data [filepointer] != '/' && filepointer < size - 1) filepointer ++;
        if (filepointer >= size - 1)
          goto weiter4;
        int nb = ReadInteger (filepointer + 1) - normalread;
        i = filepointer;
        while (data [i] == ' ' || data [i] == '\t') i ++;
        if (i >= size)
          i = i;
        int c = ReadInteger (i) - verticesread;
        while (data [filepointer] != '/' && filepointer < size - 1) filepointer ++;
        if (filepointer >= size - 1)
          goto weiter4;
        int nc = ReadInteger (filepointer + 1) - normalread;
        i = filepointer;
        while (data [i] == ' ' || data [i] == '\t') i ++;
        if (i >= size)
          i = i;
        int d = -1, nd = -1;
        if (data [i] != '\n')
        {
          if ((data [i] >= '0' && data [i] <= '9') || data [i] == '-')
          {
            d = ReadInteger (i) - verticesread;
            while (data [filepointer] != '/' && filepointer < size - 1) filepointer ++;
            if (filepointer >= size - 1) goto weiter4;
            nd = ReadInteger (filepointer + 1) - normalread;
          }
        }
        if (d >= 0) // map indices
        {
          object->quad [qn].v [0] = &object->vertex [a - 1];
          object->quad [qn].v [1] = &object->vertex [b - 1];
          object->quad [qn].v [2] = &object->vertex [c - 1];
          object->quad [qn].v [3] = &object->vertex [d - 1];
/*        object->quad [qn].v [0]->normal.add (&n [na - 1]);
          object->quad [qn].v [1]->normal.add (&n [nb - 1]);
          object->quad [qn].v [2]->normal.add (&n [nc - 1]);
          object->quad [qn].v [3]->normal.add (&n [nd - 1]);*/
          qn ++;
        }
        else
        {
          object->triangle [tn].v [0] = &object->vertex [a - 1];
          object->triangle [tn].v [1] = &object->vertex [b - 1];
          object->triangle [tn].v [2] = &object->vertex [c - 1];
/*        object->triangle [tn].v [0]->normal.add (&n [na - 1]);
          object->triangle [tn].v [1]->normal.add (&n [nb - 1]);
          object->triangle [tn].v [2]->normal.add (&n [nc - 1]);*/
          tn ++;
        }
      }
    }
    if (data [i] == 'v')
    {
      if (data [i + 1] == ' ' || data [i + 1] == '\t') goto weiter4;
      if (data [i + 1] == 't') goto weiter4;
      if (data [i + 1] == 'n') goto weiter4;
    }
    i ++;
  }
weiter4:;

  object->numTriangles = tn;
  object->numQuads = qn;
  filepointer = i;
  verticesread += vertices;
  normalread += normals;
  delete n;

/*  for (i = 0; i < object->numVertices; i ++)
    object->vertex [i].normal.norm ();*/
}

void CLoadOBJ::ComputeNormals (CModel *model)
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
    for (i2 = 0; i2 < object->numQuads; i2 ++)
    {
      object->quad [i2].getNormal (&n);
      for (i3 = 0; i3 < 4; i3 ++)
      {
        object->quad [i2].v [i3]->addNormal (&n);
      }
    }
    for (i2 = 0; i2 < object->numTriangles; i2 ++)
    {
      for (i3 = 0; i3 < 3; i3 ++)
      {
        object->triangle [i2].v [i3]->normal.norm ();
      }
    }
    for (i2 = 0; i2 < object->numQuads; i2 ++)
    {
      for (i3 = 0; i3 < 4; i3 ++)
      {
        object->quad [i2].v [i3]->normal.norm ();
      }
    }
  }
}

bool CLoadOBJ::ImportOBJ (CModel *model, char *filename)
{
  aktobject = 0;
  PreLoad (filename);
  while (filepointer < size)
    LoadObject (model);
  ComputeNormals (model);
  return true;
}

#endif
