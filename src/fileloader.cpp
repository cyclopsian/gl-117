/*
    GL-117
    Copyright 2001, 2002 Thomas A. Drexl aka heptargon

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

#ifndef IS_FILELOADER_H

#include "fileloader.h"

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

#define FILE_MAXCOMMENTS 10     /* maximum number of comment strings */

FILE *file_file = NULL;         /* FILE pointer */
char file_buffer [FILE_MAXLEN]; /* file buffer in RAM */
int file_ptr = FILE_MAXLEN;     /* file_buffer pointer to current char = "file cursor" */
char file_whitespace [256];     /* string containing the whitespace/separator chars */
int file_commentnum = 0;        /* number of comment strings */
char file_quotes [256];         /* string containing the quote chars */
char file_commentstart [FILE_MAXCOMMENTS] [256]; /* strings for comment beginnings */
char file_commentend [FILE_MAXCOMMENTS] [256];   /* strings for corresponding comment ends */
int file_bufferlen = FILE_MAXLEN;                /* current length of the file buffer */
int file_line = 1;

/**************************************************************************
  Get current line of the file
  Example: file_getLine ();
**************************************************************************/
int file_getLine ()
{
  return file_line;
}

/**************************************************************************
  Set the whitespace chars for this file
  Example: file_setWhitespace (" \n\t");
**************************************************************************/
void file_setWhitespace (char *string)
{
  strncpy (file_whitespace, string, 256);
  file_whitespace [255] = 0;
}

/**************************************************************************
  Add comment strings for this file
  Example language C++:
    file_addComment ("//", "\n");
    file_addComment ("/*", "* /"); <-- without whitespace between * and /
**************************************************************************/
void file_addComment (char *startstring, char *endstring)
{
  if (file_commentnum >= FILE_MAXCOMMENTS) return;
  strncpy (file_commentstart [file_commentnum], startstring, 256);
  file_commentstart [file_commentnum] [255] = 0;
  strncpy (file_commentend [file_commentnum], endstring, 256);
  file_commentend [file_commentnum] [255] = 0;
  file_commentnum ++;
}

/**************************************************************************
  Set the quote chars for string in this file
  Example: file_setQuotes ("\'\"");
**************************************************************************/
void file_setQuotes (char *string)
{
  strncpy (file_quotes, string, 256);
  file_quotes [255] = 0;
}

int file_fillBuffer ()
{
  int bytesread = 0;
  int n = FILE_MAXLEN - file_ptr;

  if (file_ptr <= FILE_MAXLEN / 2 && file_ptr < file_bufferlen) return 1;
  if (file_buffer [file_ptr] == 0)
    return 0;
  memcpy (file_buffer, &file_buffer [file_ptr], n);
  bytesread = fread (&file_buffer [n], 1, file_ptr, file_file);
  file_bufferlen = file_bufferlen + bytesread - file_ptr;
  if (bytesread != file_ptr)
  {
    if (file_bufferlen < 0)
      return 0;
    file_buffer [file_bufferlen] = 0;
  }
  file_ptr = 0;
  return 1;
}

/**************************************************************************
  Rewind file to an initial state to restart parsing
**************************************************************************/
void file_rewind ()
{
  fseek (file_file, 0, SEEK_SET);
  file_ptr = FILE_MAXLEN;
  file_bufferlen = FILE_MAXLEN;
  file_fillBuffer ();
  file_line = 1;
}

/**************************************************************************
  Open file for parsing
**************************************************************************/
int file_open (char *filename)
{
  file_file = fopen (filename, "r");
  if (!file_file) return 0;
  file_rewind ();
  return 1;
}

/**************************************************************************
  Close file
**************************************************************************/
int file_close ()
{
  if (file_file)
    return !fclose (file_file);
  else
    return 1;
}

int file_isChar (char *string)
{
  int i = 0;
  while (string [i] != 0 && i < 256)
  {
    if (file_buffer [file_ptr] == string [i])
      return 1;
    i ++;
  }
  return 0;
}

int file_isCommentStart ()
{
  int i;
  for (i = 0; i < file_commentnum; i ++)
  {
    if (file_isChar (file_commentstart [i]))
      return 1;
  }
  return 0;
}

int file_isCommentEnd ()
{
  int i;
  for (i = 0; i < file_commentnum; i ++)
  {
    if (file_isChar (file_commentend [i]))
      return 1;
  }
  return 0;
}

/**************************************************************************
  Find the first occurance of string from the cursor and set cursor
  to the new position, this is quite fast!
**************************************************************************/
int file_find (char *string)
{
  char *result = NULL;
  int stringlen = strlen (string);

  if (stringlen >= FILE_MAXLEN / 2)
    return 0;

  while (result == NULL)
  {
    result = strstr (&file_buffer [file_ptr], string);
    if (result == NULL)
    {
      file_ptr = file_bufferlen - stringlen;
      if (file_bufferlen < FILE_MAXLEN)
        return 0;
      if (!file_fillBuffer ())
        return 0;
    }
    else
    {
      file_ptr += (int) (result - (&file_buffer [file_ptr]));
      file_fillBuffer ();
      return 1;
    }
  }
  return 1;
}

int file_nextChar ()
{
  if (file_isChar ("\n"))
    file_line ++;
  file_ptr ++;
  return file_fillBuffer ();
}

int file_tokenpos = 0;

int file_fillToken (char *token, int tokenlen)
{
  if (tokenlen == 0)
    return 0;
  if (file_tokenpos >= tokenlen - 1)
  {
    token [tokenlen - 1] = 0;
    return 0;
  }
  token [file_tokenpos ++] = file_buffer [file_ptr];
  token [file_tokenpos] = 0;
  return 1;
}

/**************************************************************************
  Find the next token from the current cursor position and fill the
  token string. Use tokenlen to define a maximum length for token.
**************************************************************************/
int file_nextToken (char *token, int tokenlen)
{
  int ptr = 0;
  file_tokenpos = 0;
  if (tokenlen <= 0) return 1;
  token [0] = '\0';

  while (1)
  {
    if (file_isChar (file_whitespace))
    {
      if (!file_nextChar ())
        return 0;
    }
    else if (file_isCommentStart ())
    {
      do
      {
        if (!file_nextChar ())
          return 0;
      } while (!file_isCommentEnd ());
    }
    else if (file_isChar (file_quotes))
    {
      while (1)
      {
        if (!file_nextChar ())
          return 0;
        if (file_isChar (file_quotes))
        {
          if (!file_nextChar ())
            return 0;
          return 1;
        }
        if (!file_fillToken (token, tokenlen))
          return 1;
      }
    }
    else
    {
      while (1)
      {
        if (!file_fillToken (token, tokenlen))
          return 1;
        if (!file_nextChar ())
          return 0;
        if (file_isChar (file_whitespace))
          return 1;
        if (file_isCommentStart ())
          return 1;
        if (file_isChar (file_quotes))
          return 1;
      }
    }
  }
  return 1;
}

/*
void main ()
{
  char token [256];
  file_setWhitespace (" \t\r\n-");
  file_setQuotes ("\'{}");
  file_addComment ("$", "\n");
  file_open ("test.mea");
  while (file_find ("[MEA_"))
  {
    file_nextToken (token, 256);
    printf ("%s\n", token);
  }
  file_rewind ();
  while (file_nextToken (token, 256))
  {
    printf ("%s\n", token);
  }
  file_close ();
}
*/

#endif
