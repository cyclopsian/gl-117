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

#ifndef IS_CONFIGURATION_H

#include "Configuration.h"
#include "logging/Logging.h"
#include "util/Util.h"

#include <cassert>

OptionFile::OptionFile (const std::string &filename)
{
  char token [4096];

  optionList.clear ();

  int ret = file.open (filename.c_str ());
  assert (ret);
  if (!ret) return;
  file.setWhitespace (" \t\r\n");
  file.addComment ("#", "\n");
  file.setQuotes ("\"'`");
  
  while (file.nextToken (token, 4096))
  {
    std::string name = token;
    if (name == "") break; // the last token may be empty due to comments!
    if (!file.nextToken (token, 4096) || strcmp (token, "="))
    {
      assert (false);
      DISPLAY_ERROR(FormatString ("Syntax error in file %s line %d. Expected \"=\", but got %s",
                    filename.c_str (), file.getLine (), token));
      return;
    }
    if (!file.nextToken (token, 4096))
    {
      assert (false);
      DISPLAY_ERROR(FormatString ("Syntax error in file %s line %d. Expected a value, but got %s",
                    filename.c_str (), file.getLine (), token));
      return;
    }
    std::string value = token;
    
    optionList.insert (std::pair <std::string, std::string> (name, value));
  }
}

OptionFile::~OptionFile ()
{
  optionList.clear ();
  file.close ();
}

bool OptionFile::getString (const std::string &name, std::string &value)
{
  OptionList::iterator it = optionList.find (name);
  if (it == optionList.end ())
  {
    DISPLAY_DEBUG(FormatString ("Option \"%s\" not found in file \"%s\".",
                  name.c_str (), file.getFilename ()));
    return false;
  }
  value = it->second;
  return true;
}

bool OptionFile::getBoolean (const std::string &name, bool &value)
{
  std::string str;
  if (!getString (name, str)) return false;
  
  if (UpperString (str) == "TRUE") value = true;
  else value = false;
  return true;
}

bool OptionFile::getInteger (const std::string &name, int &value)
{
  std::string str;
  if (!getString (name, str)) return false;
  
  value = atoi (str.c_str ());
  return true;
}

bool OptionFile::getFloat (const std::string &name, float &value)
{
  std::string str;
  if (!getString (name, str)) return false;
  
  char *ptr;
  double d = strtod (str.c_str (), &ptr);
  value = static_cast<float>(d);
  return true;
}

#endif