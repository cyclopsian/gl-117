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

#endif // IS_RENDER_H
