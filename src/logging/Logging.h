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
#define IS_LOGGING_H

#include <iostream>

#define DISPLAY(msg,loglevel) logging.display(msg,loglevel,__FILE__,__LINE__);
#define DISPLAY_FATAL(msg) logging.display(msg,LOG_FATAL,__FILE__,__LINE__);
#define DISPLAY_ERROR(msg) logging.display(msg,LOG_ERROR,__FILE__,__LINE__);
#define DISPLAY_WARN(msg) logging.display(msg,LOG_WARN,__FILE__,__LINE__);
#define DISPLAY_INFO(msg) logging.display(msg,LOG_INFO,__FILE__,__LINE__);
#define DISPLAY_DEBUG(msg) logging.display(msg,LOG_DEBUG,__FILE__,__LINE__);

const int LOG_NONE = 0;  ///< do not log anything
const int LOG_FATAL = 1; ///< only show fatal (GL-117 will exit) errors (stderr)
const int LOG_ERROR = 2; ///< show errors too (stderr)
const int LOG_WARN = 3;  ///< show warnings too (stdout)
const int LOG_INFO = 4;  ///< show important messages too (stdout)
const int LOG_DEBUG = 5;   ///< log everything (stdout)

// exit codes
const int EXIT_NORMAL = 0;
const int EXIT_LOADFILE = 1;
const int EXIT_INIT = 2;
const int EXIT_COMMAND = 3;
const int EXIT_ALLOC = 4;
const int EXIT_CONTEXT = 5;

/**
* This class does all logging activities to stdout/stderr and logfile.
* A message is only logged if its loglevel is lower (has higher priority)
* than the overall loglevel.
*/
class Logging
{
  public:
    FILE *displayOut;
    std::string filename;

    int loglevel; ///< current log/debug level (set to constants LOG_*)

    Logging ();
    ~Logging ();

    void setLevel (int loglevel);
    void setFile (const std::string &filename);
    void display (const std::string &str, int level, char *file, int line);

  protected:

    void displayStream (FILE *stream, const std::string &str, int level,
                        char *filename, int line);
};

extern Logging logging;

#endif
