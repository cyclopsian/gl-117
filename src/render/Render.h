/* -*- mode: c++; c-basic-offset: 2; encoding: iso8859-15 -*-
    GL-117
    Copyright 2004-2004 Jean-Marc Le Peuvedic

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

#ifndef IS_RENDER_H
#define IS_RENDER_H

#include <string>

class Space;
class Landscape;
class Vector3;

/** 
 * This interface exposes the API of an abstract renderer. The
 * capability of a renderer matches the internal game
 * representation stored in Landscape and Model objects. It can
 * however add effects, like water glittering, and does not
 * necessarily show all the information (optional smoke, sun flares,
 * blackout, redout, dynamic lighting, etc.).
 * The renderer shall cache geometry information internally for all
 * models. It can transform the geometry to have it at multiple
 * degrees of coarseness for LOD purposes. It can precalculate
 * information at that time too.
 * When renderering, the renderer interrogates models in order to
 * update their current position.
 */
class Renderer
{
public:
  /// Returns the number of available registered renderers
  static unsigned count(void);

  /// Returns an available renderer, maybe to offer a choice to the
  /// user. Argument varies between 0 and count()-1.
  static Renderer& get(unsigned);

  /// This static method obtains the best registered renderer
  /// available. Renderers can indicate to this function whether they
  /// are available or not. If more that one renderer is available,
  /// some performance indication can be used to help in selection.
  static Renderer& get(void);
  
private:
  /// private init function
  static void checkInitGlobal(void);
  
  // do not use this constructor
  Renderer(const Renderer&) {};
  
protected:
  /// Registers all renderers in a global list
  Renderer(void);
  
  /// Unregisters a renderer
  virtual ~Renderer();

  /// Get the performance indicator of a renderer
  virtual float performance(void) const {return 0.0f;};

  /// Determine availability of a renderer
  virtual bool isAvailable(void) const {return false;};
  
    
  
public:
  /// Initialize this renderer (precalculate things, cache geometry)
  virtual void init(const Space& _models, const Landscape& _landscape) {};
  
  /// Draw the scene
  virtual void draw(const Vector3 &cam, float phi, float gamma) const {};

  /// Renderer's name
  virtual std::string name(void) {
    return std::string("NO RENDER");
  };
  
};

#include "model3d/Model3d.h"

class Model3dRealizer
{
  public:

    void draw (const Model3d &model, const Transformation &trafo,
               float lum, int explode)
    {
      Vector3 tl2;
      Vector3 tl(trafo.translation);
      Rotation rot(trafo.rotation);
      float zoom = trafo.scaling.x;

      VertexArray va1;
      VertexArray *va = &va1;
      int alpha = 1;
      int nolight = 0;

      if (nolight) // if model wants to be rendered without light, call draw2
      {
        glDisable (GL_LIGHTING);
    //    drawNoLight (tl, tl2, rot, zoom, explode);
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

      for (i = 0; i < model.numObjects; i ++)
      {
        if (model.numObjects <= 0) break;
        cm = model.object [i];
        if (cm->hasTexture)
        {
          if (antialiasing)
            cm->material->texture->shadeLinear ();
          else
            cm->material->texture->shadeConst ();
        }
      }

      zoom *= model.scale;
      glPushMatrix ();
      glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
      float explodefac = (float) explode / 10 / timestep;

      if (model.showCollision)
      {
        glPushMatrix ();
        glScalef (model.cube.x, model.cube.y, model.cube.z);
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

      glRotatef (rot.phi+90, 0, -1, 0);
      glRotatef (-rot.gamma+90, 0, 0, 1);
      glRotatef (rot.theta+180, 1, 0, 0);
      glScalef (zoom, zoom, zoom);

      if (model.shading == 1)
        glShadeModel (GL_FLAT);
      else
        glShadeModel (GL_SMOOTH);

      if (alpha)
      { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.2); }

      for (i = 0; i < model.numObjects; i ++)
      {
        if (model.numObjects <= 0) break;
        cm = model.object [i];
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
              if (model.numMaterials && cm->material) 
              {
                unsigned char *color = cm->material->color.c;
                int rotcol = 0;
                if (color [0] > 190 && color [1] > 190 && color [2] < 20)
                {
    //              model.rotateColor (30); // TODO: rotate
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
              if (model.numMaterials && cm->material && cm->material->texture->textureID >= 0) 
              {
                unsigned char *pColor = model.material [cm->material->texture->textureID]->color.c;
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

    void drawNoLight (const Model3d &model, const Transformation &trafo,
                      int explode)
    {
      int i, j;
      Object3d *cm;
      Vector3 tl2;
      Vector3 tl(trafo.translation);
      Rotation rot(trafo.rotation);
      float zoom = trafo.scaling.x;
      int shading = 2;
      int showCollision = 0;

      VertexArray va1;
      VertexArray *va = &va1;
      int alpha = 1;
//      int nolight = 0;

      for (i = 0; i < model.numObjects; i ++)
      {
        if (model.numObjects <= 0) break;
        cm = model.object [i];
        if (cm->hasTexture)
        {
          if (antialiasing)
            cm->material->texture->shadeLinear ();
          else
            cm->material->texture->shadeConst ();
        }
      }

//      zoom *= scale;
      glPushMatrix ();
      glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
      float explodefac = (float) explode / 10 / timestep;

      if (showCollision)
      {
        glPushMatrix ();
        glScalef (model.cube.x, model.cube.y, model.cube.z);
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

      glRotatef (rot.phi+90, 0, -1, 0);
      glRotatef (-rot.gamma+90, 0, 0, 1);
      glRotatef (rot.theta+180, 1, 0, 0);
      glScalef (zoom, zoom, zoom);

/*      bool listgen = false;
      if (list2 == -1 && explode <= 0 && displaylist)
      {
        listgen = true;
        list2 = gl.createList ();
      }
      if (listgen || explode > 0 || !displaylist)
      { */

        if (shading == 1)
          glShadeModel (GL_FLAT);
        else
          glShadeModel (GL_SMOOTH);

        if (model.alpha)
        { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.01); }

        for (i = 0; i < model.numObjects; i++)
        {
          if (model.numObjects <= 0) break;
          cm = model.object [i];
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

/*        if (listgen) glEndList ();
      }
      else glCallList (list2); */

      glPopMatrix ();
    }

    void drawNoTexture (const Model3d &model, const Transformation &trafo,
                        float lum, int explode)
    {
      int i, j;
      Object3d *cm;
      Vector3 tl2;
      Vector3 tl(trafo.translation);
      Rotation rot(trafo.rotation);
      float zoom = trafo.scaling.x;

      VertexArray va1;
      VertexArray *va = &va1;
      int alpha = 1;
      int showCollision = 0;
//      int nolight = 0;

//      zoom *= scale;
      glPushMatrix ();
      glTranslatef (tl.x + tl2.x, tl.y + tl2.y - 0.002 * explode * explode / timestep / timestep, tl.z + tl2.z);
      float explodefac = (float) explode / 10 / timestep;

      if (showCollision)
      {
        glPushMatrix ();
        glScalef (model.cube.x, model.cube.y, model.cube.z);
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

      glRotatef (rot.phi+90, 0, -1, 0);
      glRotatef (-rot.gamma+90, 0, 0, 1);
      glRotatef (rot.theta+180, 1, 0, 0);
      glScalef (zoom, zoom, zoom);

      if (alpha)
      { glEnable (GL_BLEND); glEnable (GL_ALPHA_TEST); glAlphaFunc (GL_GEQUAL, 0.2); }

      for (i = 0; i < model.numObjects; i++)
      {
        if (model.numObjects <= 0) break;
        cm = model.object [i];
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

};


class OpenGlRenderer : public Renderer
{
  public:
  
    OpenGlRenderer() : Renderer()
    {
    }
    
    virtual void init(const Space &space, const Landscape &landscape)
    {
    }
    
    virtual void draw(const Space &space, const Landscape &landscape) const
    {
      int i;
      frustum.extractFrustum ();

      for (i = 0; i < space.o.size (); i ++)
      {
        SpaceObj *spaceobj = space.o [i];
        Model3d *model = spaceobj->o;
        if (model != NULL && spaceobj->draw)
        {
          Vector3 tl1(spaceobj->trafo.translation);
          if (frustum.isSphereInFrustum (tl1.x, tl1.y, tl1.z, space.o [i]->trafo.scaling.x * 1.5))
          {
            if (spaceobj->drawLight)
            {
              glEnable (GL_LIGHTING);
              Model3dRealizer mr;
              mr.draw (*model, spaceobj->trafo, 1.0, 0);
              if (!spaceobj->ref.empty ())
              {
                glPushMatrix ();
                glTranslatef (tl1.x, tl1.y - 0.001 * spaceobj->explode * spaceobj->explode / timestep / timestep, tl1.z);
                glRotatef (spaceobj->trafo.rotation.phi + 90, 0, -1, 0);
                glRotatef (-spaceobj->trafo.rotation.gamma + 90, 0, 0, 1);
                glRotatef (spaceobj->trafo.rotation.theta + 180, 1, 0, 0);
                glScalef (spaceobj->trafo.scaling.x, spaceobj->trafo.scaling.y, spaceobj->trafo.scaling.z);
                if (model->refpoint)
                  for (unsigned i2 = 0; i2 < spaceobj->ref.size (); i2 ++)
                    if (spaceobj->ref [i2].trafo.scaling.x > 0.001)
                    {
                      Vector3 tl(model->refpoint [i2 / 3]);
                      tl.add(spaceobj->ref [i2].trafo.translation);
                      mr.draw (*spaceobj->ref [i2].o, Transformation(tl, spaceobj->ref [i2].trafo.rotation, spaceobj->ref [i2].trafo.scaling), spaceobj->lum, spaceobj->explode);
//                      spaceobj->ref [i].o->draw (model->refpoint [i / 3], spaceobj->ref [i].trafo.translation, spaceobj->ref [i].trafo.rotation, ref [i].trafo.scaling.x, spaceobj->lum, spaceobj->explode);
                    }
                glPopMatrix ();
              }
            }
            else
            {
              glDisable (GL_LIGHTING);
              Model3dRealizer mr;
              mr.drawNoLight (*model, spaceobj->trafo, 0);
            }
          }
        }
      }
    }
};


#endif // IS_RENDER_H
