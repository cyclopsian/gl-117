/* This file includes all global definitions. */

#ifndef IS_COMMON_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

// display log/debug message
void display (char *str, int level)
{
  int len = strlen (str);
  if (!len) return;
  if (level <= debuglevel)
  {
    FILE *stream = stdout;
    if (level == LOG_FATAL || level == LOG_ERROR || level == LOG_WARN)
      stream = stderr;
    if (level == LOG_NONE) fprintf (stream, "%s\n", str);
    else if (level == LOG_FATAL) fprintf (stream, "Fatal: %s\n", str);
    else if (level == LOG_ERROR) fprintf (stream, "Error: %s\n", str);
    else if (level == LOG_WARN) fprintf (stream, "Warning: %s\n", str);
    else if (level == LOG_MOST) fprintf (stream, "Info: %s\n", str);
    else fprintf (stream, "Debug: %s\n", str);
    fflush (stream);
  }
}

// display "out of memory" error and exit
void error_outofmemory ()
{
  display ("Out of memory", LOG_FATAL);
  exit (EXIT_ALLOC);
}

#endif
