/*
   ****************************************************************************
   * COPYRIGHT  by Testing Solutions                                          *
   * Reuter, Christian  c.reuter@testingsolutions.de                          *
   * Von-Leoprechting-Str. 14, 86899 Landsberg / Lech Germany                 *
   * Phone: +49 8191 305202, Fax: +49 8191 305244                             *
   * Internet: www.testingsolutions.de                                        *
   ****************************************************************************
   *                                                                          *
   * All Rights Reserved.                                                     *
   *                                                                          *
   * This software is the confidential and proprietary information of the     *
   * author.                                                                  *
   *                                                                          *
   * All brand names and product names used in this software are trademarks,  *
   * registered trademarks, or trade names of their respective holders.       *
   * The authors of this software are not associated with any product or      *
   * vendor mentioned in the code or documentation.                           *
   *                                                                          *
   ****************************************************************************

   DOCUMENTATION:
     This is a low level library to split text files into tokens.
*/

#ifndef IS_FILELOADER_H
#define IS_FILELOADER_H

/**************************************************************************
  This is a low level library to split text files into tokens.
  Customize strings for quotes, whitespace and comments.
  Then parse one token after each other calling file_nextToken()
  or set the file cursor to a new position calling file_find(mystr)

  Documentation notes:
  - all int-functions return 0 on error/EOF, 1 on success
  - all documented functions are public, the rest is private

  Example to parse MEA-files:
    char token [256];
    file_setWhitespace (" \t\r\n-=");
    file_setQuotes ("\'{}");
    file_addComment ("$", "\n");
    file_open ("test.mea");
    while (file_find ("[MEA_"))         // search for string [MEA_
    {
      file_nextToken (token, 256);      // read the [MEA_ token
      printf ("%s\n", token);           // and print it on stdout
    }
    file_rewind ();                     // again from the beginning
    while (file_nextToken (token, 256)) // read every token
    {
      printf ("%s\n", token);           // print out every token
    }
**************************************************************************/

#define _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE64
#define _FILE_OFFSET_BITS 64

// we should use __long here!!!
#define XLONG long

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_MAXCOMMENTS 10     /* maximum number of comment strings */
#define FILE_MAXLEN 4096       /* maximum length of the file buffer */

extern int file_quoteremove;    /* 1: remove quotes from tokens */

extern int file_ptr;     /* file_buffer pointer to current char = "file cursor" */
extern int file_reverse; /* read only!!! */

/**************************************************************************
  Init data structure
  Example: file_init ();
**************************************************************************/
extern void file_init ();

extern void file_initComments ();

/**************************************************************************
  Revert the file read order (forward/backward)
  Example: file_revert ();
**************************************************************************/
extern void file_revert ();

/**************************************************************************
  Get current position of the file pointer
  Example: file_getPosition ();
**************************************************************************/
extern XLONG file_getPosition ();

/**************************************************************************
  Set current position of the file pointer
  Example: file_setPosition ();
**************************************************************************/
extern int file_setPosition ();

/**************************************************************************
  Get current line of the file
  Example: file_getLine ();
**************************************************************************/
extern int file_getLine ();

/**************************************************************************
  Set the whitespace chars for this file
  Example: file_setWhitespace (" \n\t");
**************************************************************************/
extern void file_setWhitespace (char *string);

/**************************************************************************
  Set the separator chars for this file
  Example: file_setSeparator (",;");
**************************************************************************/
extern void file_setSeparator (char *string);

/**************************************************************************
  Add comment strings for this file
  Example language C++:
    file_addComment ("//", "\n");
    file_addComment ("/*", "* /"); <-- without whitespace between * and /
**************************************************************************/
extern void file_addComment (char *startstring, char *endstring);

/**************************************************************************
  Set the quote chars for string in this file
  Example: file_setQuotes ("\'\"");
**************************************************************************/
extern void file_setQuotes (char *string);

/**************************************************************************
  Rewind file to an initial state to restart parsing
**************************************************************************/
extern void file_rewind ();

/**************************************************************************
  Open file for parsing
**************************************************************************/
extern int file_open (char *filename);

/**************************************************************************
  Close file
**************************************************************************/
extern int file_close ();

/**************************************************************************
  Find the first occurance of string from the cursor and set cursor
  to the new position, this is quite fast!
**************************************************************************/
extern int file_find (char *string);

/**************************************************************************
  Find the next token from the current cursor position and fill the
  token string. Use tokenlen to define a maximum length for token.
**************************************************************************/
extern int file_nextToken (char *token, int tokenlen);
extern int file_nextChar ();

#endif
