/*
    GL-117
    Copyright 2001-2004 Thomas A. Drexl aka heptargon

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

/* This file includes all AI objects instancing models. */

#ifndef IS_AIOBJECT_H

#include "AiObj.h"

#include <cassert>

UnitDescriptorList UnitDescriptorRegistry::unitDescriptorList;

UnitDescriptorRegistry::UnitDescriptorRegistry ()
{
}

UnitDescriptorRegistry::~UnitDescriptorRegistry ()
{
  unitDescriptorList.clear ();
}

void UnitDescriptorRegistry::add (const UnitDescriptor &desc)
{
  unitDescriptorList.insert (std::pair<int, const UnitDescriptor> (desc.id, desc));
}

const UnitDescriptor &UnitDescriptorRegistry::get (int id)
{
  UnitDescriptorList::iterator it = unitDescriptorList.find (id);
  if (it == unitDescriptorList.end ())
  {
    assert (false);
    return *(new UnitDescriptor);
  }
  else
  {
    return it->second;
  }
}

#endif
