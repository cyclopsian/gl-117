/* This file includes a wavefront obj loader. */

#ifndef IS_OBJLOAD_H
#define IS_OBJLOAD_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define int32 int

class CFile
{
  public:
  FILE *in;
  int32 size;
  int32 filepointer;
  char *data;

  CFile (char *filename);
  ~CFile ();
  float readFloat (int32 offset);
  float readFloat ();
  int readInteger (int32 offset);
  int readInteger ();
  char *readWord (int32 offset);
  char *readWord ();
  char *getChar ();
  void nextWhite ();
  void skipWhite ();
};

#include "model.h"

// CLoad3DS handles all of the loading code (see NEHE's OpenGL tutorials)
class CLoadOBJ
{
  public:
  CLoadOBJ ();
  bool ImportOBJ (CModel *model, char *filename);
  
  private:
  CFile *file;
  void ComputeNormals (CModel *model);
  void ComputeColors (CModel *model);
  void Normalize (CModel *model);
  void LoadObject (CModel *model);
};

#endif
