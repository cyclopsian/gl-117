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

#ifndef IS_LOGGING_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "Logging.h"



Logging logging;

Logging::Logging ()
{
  displayOut = NULL;
  loglevel = LOG_MOST;
  filename = "logfile.txt";
}

Logging::~Logging ()
{
  if (displayOut) fclose (displayOut);
  displayOut = NULL;
}

void Logging::setLevel (int loglevel)
{
  this->loglevel = loglevel;
}

void Logging::setFile (const std::string &filename)
{
  this->filename = filename;
}

void Logging::displayStream (FILE *stream, const std::string &str, int level)
{
  if (level == LOG_NONE) fprintf (stream, "%s\n", str.c_str ());
  else if (level == LOG_FATAL) fprintf (stream, "Fatal: %s\n", str.c_str ());
  else if (level == LOG_ERROR) fprintf (stream, "Error: %s\n", str.c_str ());
  else if (level == LOG_WARN) fprintf (stream, "Warning: %s\n", str.c_str ());
  else if (level == LOG_MOST) fprintf (stream, "Info: %s\n", str.c_str ());
  else fprintf (stream, "Debug: %s\n", str.c_str ());
  fflush (stream);
}

// display log/debug message
void Logging::display (const std::string &str, int level)
{
  int len = str.length ();
  if (!len) return;

  if (level <= loglevel)
  {
    FILE *stream = stdout;
    if (level == LOG_FATAL || level == LOG_ERROR || level == LOG_WARN)
      stream = stderr;
    displayStream (stream, str, level);

    if (!displayOut)
    {
      if ((displayOut = fopen (filename.c_str (), "wt")) != NULL)
      {
        displayStream (displayOut, str, level);
      }
    }
    else
    {
      displayStream (displayOut, str, level);
    }
  }
}

// display "out of memory" error and exit
void Logging::errorOutOfMemory ()
{
  logging.display ("Out of memory", LOG_FATAL);
  exit (EXIT_ALLOC);
}

#endif
