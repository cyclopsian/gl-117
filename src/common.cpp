/* This file includes all global definitions. */

#ifndef IS_COMMON_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

// Display text on stdout/stderr
void display (char *str, int level)
{
  int len = strlen (str);
  if (!len) return;
  if (level <= debuglevel)
  {
    if (level == LOG_NONE) fprintf (stdout, "%s\n", str);
    else if (level == LOG_FATAL) fprintf (stderr, "Fatal: %s\n", str);
    else if (level == LOG_ERROR) fprintf (stderr, "Error: %s\n", str);
    else if (level == LOG_WARN) fprintf (stderr, "Warning: %s\n", str);
    else if (level == LOG_MOST) fprintf (stdout, "Info: %s\n", str);
    else fprintf (stdout, "Debug: %s\n", str);
  }
}

#endif
