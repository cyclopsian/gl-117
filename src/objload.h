/* This file includes a wavefront obj loader. */

#ifndef IS_OBJLOAD_H
#define IS_OBJLOAD_H

#include <stdio.h>
#include <stdlib.h>

#include "model.h"

// CLoad3DS handles all of the loading code (see NEHE's OpenGL tutorials)
class CLoadOBJ
{
  public:
  CLoadOBJ ();
  bool ImportOBJ (CModel *model, char *filename);
  
  private:
  char *data; // file content data array
  long size; // size of data array = file size
  long filepointer; // pointer (index) to data array
  int aktobject; // object being loaded
  int verticesread; // vertices read while loading
  int normalread;
  int ReadInteger (int offset);
  float ReadValue (int offset);
  void PreLoad (char *filename);
  void ComputeNormals (CModel *model);
  void LoadObject (CModel *model);
};

#endif
