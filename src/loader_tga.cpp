/* 
 * loader_tga.c - Loader for Truevision Targa images
 *
 * by Dan Maas <dmaas@dcine.com>   May 15, 2000
 *
 * Modified for GL-117 by Piotr Pawlow <pp@siedziba.pl>
 *
 * based on TGA specifications available at: 
 * http://www.wotsit.org/cgi-bin/search.cgi?TGA
 *
 * header/footer structures courtesy of the GIMP Targa plugin 
 */

#ifndef LOADER_TGA_H

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include "loader_tga.h"
#include "../config.h"

#define DATA8   unsigned char
#define DATA32  unsigned int

#ifndef WORDS_BIGENDIAN

#define A_VAL(p) ((DATA8 *)(p))[3]
#define R_VAL(p) ((DATA8 *)(p))[0]
#define G_VAL(p) ((DATA8 *)(p))[1]
#define B_VAL(p) ((DATA8 *)(p))[2]

#else

#define A_VAL(p) ((DATA8 *)(p))[0]
#define R_VAL(p) ((DATA8 *)(p))[3]
#define G_VAL(p) ((DATA8 *)(p))[2]
#define B_VAL(p) ((DATA8 *)(p))[1]

#endif

#define WRITE_RGBA(p, r, g, b, a) \
   R_VAL(p) = (r); \
   G_VAL(p) = (g); \
   B_VAL(p) = (b); \
   A_VAL(p) = (a);

/* flip an inverted image - see RLE reading below */
static DATA32* flip(DATA32* in, int w, int h);

/* TGA pixel formats */
#define TGA_TYPE_MAPPED      1 
#define TGA_TYPE_COLOR       2 
#define TGA_TYPE_GRAY        3 
#define TGA_TYPE_MAPPED_RLE  9 
#define TGA_TYPE_COLOR_RLE  10 
#define TGA_TYPE_GRAY_RLE   11 

/* TGA header flags */
#define TGA_DESC_ABITS      0x0f 
#define TGA_DESC_HORIZONTAL 0x10 
#define TGA_DESC_VERTICAL   0x20 

#define TGA_SIGNATURE "TRUEVISION-XFILE" 

typedef struct { 
   unsigned char    idLength; 
   unsigned char    colorMapType; 
   unsigned char    imageType; 
   unsigned char    colorMapIndexLo, colorMapIndexHi; 
   unsigned char    colorMapLengthLo, colorMapLengthHi; 
   unsigned char    colorMapSize; 
   unsigned char    xOriginLo, xOriginHi; 
   unsigned char    yOriginLo, yOriginHi; 
   unsigned char    widthLo, widthHi; 
   unsigned char    heightLo, heightHi; 
   unsigned char    bpp; 
   unsigned char    descriptor; 
} tga_header; 

typedef struct { 
   unsigned int extensionAreaOffset; 
   unsigned int developerDirectoryOffset; 
   char signature[16]; 
   char dot; 
   char null; 
} tga_footer; 

/* Load up a TGA file 
 * 
 * As written this function only recognizes the following types of Targas: 
 *		Type 02 - Uncompressed RGB, 24 or 32 bits 
 *		Type 03 - Uncompressed grayscale, 8 bits
 *		Type 10 - RLE-compressed RGB, 24 or 32 bits
 *		Type 11 - RLE-compressed grayscale, 8 bits  
 * There are several other (uncommon) Targa formats which this function can't currently handle
 */

unsigned char* tga_load (char* im_file,int* w,int* h)
{ 
   FILE        *fp; 
   int         bpp, vinverted = 0;
   int         rle = 0, footer_present = 0; 
   DATA32      *im_data;   
   int         im_w,im_h;

   tga_header header; 
   tga_footer footer; 
   
   fp = fopen(im_file, "rb"); 
   if(!fp)
      return 0;
   
   /* read the footer first */
   fseek (fp, 0L - (sizeof (tga_footer)), SEEK_END); 
   if (fread (&footer, sizeof (tga_footer), 1, fp) != 1) 
     { 
	fclose(fp);
	return 0; 
     } 
   
   /* check the footer to see if we have a v2.0 TGA file */
   if (memcmp(footer.signature, TGA_SIGNATURE, sizeof (footer.signature)) == 0) 
      footer_present = 1; 
   
   /* now read the header */
   if (fseek (fp, 0, SEEK_SET) || fread (&header, sizeof (header), 1, fp) != 1)  
     { 
	fclose(fp);
	return 0; 
     }
   
   /* skip over alphanumeric ID field */
   if (header.idLength && fseek (fp, header.idLength, SEEK_CUR)) 
     { 
	fclose(fp);
	return 0; 
     } 
   
   /* now parse the header */
   
   /* this flag indicated bottom-up pixel storage */
   vinverted = header.descriptor ^ TGA_DESC_VERTICAL; 
   
   switch (header.imageType) 
     { 
     case TGA_TYPE_COLOR_RLE:
     case TGA_TYPE_GRAY_RLE:
	rle = 1; 
	break; 
	
     case TGA_TYPE_COLOR: 
     case TGA_TYPE_GRAY:
	rle = 0; 
	break; 
	
     default: 
	fclose(fp);
	return 0; 
     } 
   
   /* bits per pixel */
   bpp = header.bpp; 
   
   if( ! ((bpp == 32) || (bpp == 24) || (bpp == 8)) ) 
     { 
	fclose(fp);
	return 0;
     } 
   
   /* endian-safe loading of 16-bit sizes */
   im_w = (header.widthHi << 8) | header.widthLo; 
   im_h = (header.heightHi << 8) | header.heightLo;    
   
   if ((im_w > 32767) || (im_w < 1) || (im_h > 32767) || (im_h < 1))
     {
	im_w = 0;
	fclose(fp);
	return 0;
     }
   
     {
	unsigned long datasize;
	struct stat ss;
	unsigned char *buf, *bufptr;
	DATA32 *dataptr; 
	
	int   y;
	
	/* allocate the destination buffer */
	im_data = (DATA32*)malloc(im_w * im_h * sizeof(DATA32));
	if(!im_data)
	  {
	     im_w = 0;
	     fclose(fp);
	     return 0;
	  }
	
	/* first we read the file data into a buffer for parsing */
	/* then we decode from RAM */
	
	/* find out how much data must be read from the file */
	/* (this is NOT simply width*height*4, due to compression) */
	
	stat(im_file, &ss);
	datasize = ss.st_size - sizeof(tga_header) - header.idLength -
	   (footer_present ? sizeof(tga_footer) : 0);
	
	buf = (unsigned char*)malloc(datasize); 
	if(!buf) 
	  {
	     im_w = 0;
	     fclose(fp);
	     return 0;
	  }
	
	/* read in the pixel data */
	if( fread(buf, 1, datasize, fp) != datasize) 
	  {
	     fclose(fp);
	     return 0;
	  }

	/* buffer is ready for parsing */
	
	/* bufptr is the next byte to be read from the buffer */
	bufptr = buf; 
	
	/* dataptr is the next 32-bit pixel to be filled in */
	dataptr = im_data;
	
	/* decode uncompressed BGRA data */
	if(!rle) 
	  { 
	     for(y = 0; y < im_h; y++) /* for each row */
	       { 
		  int x;
		  
		  /* point dataptr at the beginning of the row */
		  if(!vinverted)
		     /* some TGA's are stored upside-down! */
		     dataptr = im_data + (im_h - (y+1)) * im_w;
		  else
		     dataptr = im_data + y * im_w;
		  
		  
		  for(x = 0; x < im_w; x++) /* for each pixel in the row */
		    {
		       switch(bpp) {
			  
			  /* 32-bit BGRA pixels */
		       case 32: 
			  WRITE_RGBA(dataptr, 
				     *(bufptr + 2),  /* R */
				     *(bufptr + 1),  /* G */
				     *(bufptr + 0),  /* B */
				     *(bufptr + 3)   /* A */
				     );
			  dataptr++;
			  bufptr += 4;
			  break;
			  
			  /* 24-bit BGR pixels */
		       case 24: 
			  WRITE_RGBA(dataptr,
				     *(bufptr + 2),  /* R */
				     *(bufptr + 1),  /* G */
				     *(bufptr + 0),  /* B */
				     (char) 0xff     /* A */
				     );
			  dataptr++;
			  bufptr += 3;
			  break;
			  
			  /* 8-bit grayscale */
		       case 8:
			  WRITE_RGBA(dataptr,
				     *bufptr,      /* grayscale */
				     *bufptr,
				     *bufptr,
				     (char) 0xff
				     );
			  dataptr++;
			  bufptr += 1;
			  break;
		       }  
		       
		    } /* end for (each pixel) */
		  
	       } /* end for (each row) */
	     
	  } /* end if (RLE) */
	
	/* decode RLE compressed data */
	else 
	  { 
	     unsigned char curbyte, red, green, blue, alpha; 
	     DATA32 *final_pixel = dataptr + im_w * im_h; 
	     
	     /* loop until we've got all the pixels */
	     while(dataptr < final_pixel)	
	       { 
		  int count; 
		  
		  curbyte = *bufptr++; 
		  count = (curbyte & 0x7F) + 1; 
		  
		  if(curbyte & 0x80)  /* RLE packet */
		    { 
		       int i;
		       
		       switch(bpp) {
		       case 32:
			  blue = *bufptr++; green = *bufptr++; red = *bufptr++; 
			  alpha = *bufptr++;
			  for(i = 0; i < count; i++) {
			     WRITE_RGBA(dataptr, red, green, blue, alpha);
			     dataptr++;
			  }
			  break;
			  
		       case 24:
			  blue = *bufptr++; green = *bufptr++; red = *bufptr++; 
			  for(i = 0; i < count; i++) {
			     WRITE_RGBA(dataptr, red, green, blue, (char) 0xff);
			     dataptr++;
			  }
			  break;
			  
		       case 8:
			  alpha = *bufptr++;
			  for(i = 0; i < count; i++) {
			     WRITE_RGBA(dataptr, alpha, alpha, alpha, (char) 0xff);
			     dataptr++;
			  }
			  break;
		       }
		       
		    } /* end if (RLE packet) */
		  
		  else  /* raw packet */
		    { 
		       int i;
		       
		       for(i = 0; i < count; i++) 
			 { 
			    switch(bpp) {
			       
			       /* 32-bit BGRA pixels */
			    case 32: 
			       WRITE_RGBA(dataptr, 
					  *(bufptr + 2),  /* R */
					  *(bufptr + 1),  /* G */
					  *(bufptr + 0),  /* B */
					  *(bufptr + 3)   /* A */
					  );
			       dataptr++;
			       bufptr += 4;
			       break;
			       
			       /* 24-bit BGR pixels */
			    case 24: 
			       WRITE_RGBA(dataptr,
					  *(bufptr + 2),  /* R */
					  *(bufptr + 1),  /* G */
					  *(bufptr + 0),  /* B */
					  (char) 0xff     /* A */
					  );
			       dataptr++;
			       bufptr += 3;
			       break;
			       
			       /* 8-bit grayscale */
			    case 8:
			       WRITE_RGBA(dataptr,
					  *bufptr,      /* pseudo-grayscale */
					  *bufptr,
					  *bufptr,
					  (char) 0xff
					  );
			       dataptr++;
			       bufptr += 1;
			       break;
			    }  
			 } 
		    } /* end if (raw packet) */ 
		  
	       } /* end for (each packet) */
	     
	     /* must now flip a bottom-up image */
	     
	     /* This is the best of several ugly implementations
	      * I considered. It's not very good since the image
	      * will be upside-down throughout the loading process.
	      * This could be done in-line with the de-RLE code
	      * above, but that would be messy to code. There's
	      * probably a better way... */
	     
	     if(!vinverted) {
		im_data = flip(im_data, im_w, im_h);
		if(!im_data) {
		   fclose(fp);
		   free(buf);
		   return 0;
		}
	     }

	  } /* end if (image is RLE) */ 

	free(buf); 
     }
   
//   fclose(fp);
   *w=im_w; *h=im_h;
   return (unsigned char*)im_data;
}

/**********************/

/* flip a DATA32 image block vertically
 * by allocating a new block, then copying
 * the rows in reverse order
 */

static DATA32*
flip(DATA32* in, int w, int h)
{
   int adv, adv2, i;
   DATA32* out;
   
   out = (DATA32*)malloc(w * h * sizeof(DATA32));
   if(!out)
      return NULL;
   
   adv = 0; adv2 = w * h;
   
   for(i = 0; i < h; i++) {
      adv2 -= w;
      memmove(out + adv, in + adv2, w * sizeof(DATA32));
      adv += w;
   }
   
   free(in);
   return out;
}

#endif
