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
#define IS_FILELOADER_H

//#define _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE64
#define _FILE_OFFSET_BITS 64

// we should use __long here!!!
#define XLONG long

#define MAXCOMMENTS 10    ///< maximum number of comment strings
#define MAXLEN 4096       ///< maximum length of the file buffer

/**
* This is a low level library to split text files into tokens.
* Customize strings for quotes, whitespace and comments.
* Then parse one token after each other calling nextToken()
* or set the file cursor to a new position calling find(mystr)
*
* Documentation notes:
* - all int-functions return 0 on error/EOF, 1 on success
* - all documented functions are public, the rest is private
*
* Example to parse MEA-files:
*   char token [256];
*   setWhitespace (" \t\r\n-=");
*   setQuotes ("\'{}");
*   addComment ("$", "\n");
*   open ("test.mea");
*   while (find ("[MEA_"))         // search for string [MEA_
*   {
*     nextToken (token, 256);      // read the [MEA_ token
*     printf ("%s\n", token);           // and print it on stdout
*   }
*   rewind ();                     // again from the beginning
*   while (nextToken (token, 256)) // read every token
*   {
*     printf ("%s\n", token);           // print out every token
*   }
*/
class FileTokenizer
{
  public:

    FileTokenizer ();

    /**
    * Init data structure
    * Example: init ();
    */
    void init ();

    void initComments ();

    /**
    * Revert the file read order (forward/backward)
    * Example: revert ();
    */
    void revert ();

    /**
    * Get current position of the file pointer
    * Example: getPosition ();
    */
    XLONG getPosition ();

    /**
    * Set current position of the file pointer
    * Example: setPosition ();
    */
    int setPosition ();

    /**
    * Get current line of the file
    * Example: getLine ();
    */
    int getLine ();

    /**
    * Set the whitespace chars for this file
    * Example: setWhitespace (" \n\t");
    */
    void setWhitespace (char *string);

    /**
    * Set the separator chars for this file
    * Example: setSeparator (",;");
    */
    void setSeparator (char *string);

    /**
    * Add comment strings for this file
    * Example language C++:
    *   addComment ("//", "\n");
    */
    void addComment (char *startstring, char *endstring);

    /**
    * Set the quote chars for string in this file
    * Example: setQuotes ("\'\"");
    */
    void setQuotes (char *string);

    /**
    * Rewind file to an initial state to restart parsing
    */
    void rewind ();

    /**
    * Open file for parsing
    */
    int open (char *filename);

    /**
    * Close file
    */
    int close ();

    /**
    * Find the first occurance of string from the cursor and set cursor
    * to the new position, this is quite fast!
    */
    int find (char *string);

    /**
    * Find the next token from the current cursor position and fill the
    * token string. Use tokenlen to define a maximum length for token.
    */
    int nextToken (char *token, int tokenlen);

    /**
    * Return the next character.
    */
    int nextChar ();

  protected:

    int reverse;
    int quoteremove;             ///< 1: remove quotes from tokens
    int ptr;                     ///< buffer pointer to current char = "file cursor"
    FILE *file;                  ///< FILE pointer
    char buffer [MAXLEN+1];      ///< file buffer in RAM
    char whitespace [256];       ///< string containing the whitespace chars
    char separator [256];        ///< string containing the separator chars
    int commentnum;              ///< number of comment strings
    char quotes [256];           ///< string containing the quote chars
    char commentstart [MAXCOMMENTS] [256]; ///< strings for comment beginnings
    char commentend [MAXCOMMENTS] [256];   ///< strings for corresponding comment ends
    int bufferlen;               ///< current length of the file buffer
    int line;
    char currentquotes;
    char currentcomment;
    XLONG bufferstart;
    int bufferread;
    char lastchar;
    int tokenpos;

    void revertBuffer (int start, int end);
    int fillBuffer ();
    int setPosition (XLONG pointer, int whence);
    int isChar (char *string);
    int isCommentStart ();
    int isCommentEnd ();
    int fillToken (char *token, int tokenlen);

};

#endif
