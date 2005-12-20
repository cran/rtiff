#include <math.h>
#include <tiffio.h>
/*============================================================================ 
 * tiffRead --
 *
 * Copyleft 2001 by Eric J. Kort/Van Andel Research Institute
 * eric.kort@vai.org
 *
 * libtiff copyright info:
 *
 * Copyright (c) 1988-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 *============================================================================
 */

void TiffGetHeight (char** fileName, int* h) {
  TIFF* tif;

  tif = TIFFOpen(*fileName, "r");
  if (tif == 0) {
    *h = -1;
    return;
  }
  TIFFGetField(tif, 257, h);
  TIFFClose(tif);


  return;
}


void TiffGetWidth (char** fileName, int* w) {
  TIFF* tif;

  tif = TIFFOpen(*fileName, "r");
  if (tif == 0) {
    *w = -1;
    return;
  }
  TIFFGetField(tif, 256, w);    
  TIFFClose(tif);
  return;
}

void TiffIsTiled(char **fileName, int* yn) {
  TIFF* tif;

  tif = TIFFOpen(*fileName, "r");
  *yn = TIFFIsTiled(tif);
  TIFFClose(tif);
  return;
}

void TiffGetImageType (char** fileName, int *dir, int* spp, int* pm, int* bps, int* tiled) {
  TIFF* tif;

  if((tif = TIFFOpen(*fileName, "r")) == NULL) {
    return;
  }

  if (TIFFSetDirectory(tif, *dir) != 1) {
    printf("Cannot find page %d in this tiff image\n", (*dir+1));
    return;
  }

  TIFFGetField(tif, 262, pm);
  TIFFGetField(tif, 277, spp);        
  TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, bps);
  *tiled = TIFFIsTiled(tif);
  TIFFClose(tif);

  return;
}

void TiffReadTIFFRGBA (char** fileName, int* dir, int* r, int* g, int* b) {
  uint32 w, h;
  int i;
  uint32 *buf;
  TIFF* tif;


  if((tif = TIFFOpen(*fileName, "r")) == NULL) {
    return;
  }

  if (TIFFSetDirectory(tif, *dir) != 1) {
    printf("Cannot find page %d in this tiff image\n", (*dir+1));
    TIFFClose(tif);
    return;
  }

  TIFFGetField(tif, 257, &h);
  TIFFGetField(tif, 256, &w);

  buf = (uint32*) _TIFFmalloc(w * h * sizeof (uint32));
  if (buf != NULL) {
    if (TIFFReadRGBAImageOriented(tif, w, h, buf, 0, 1)) {
      for (i=0; i<(w*h); i++) {
        r[i] = (int)TIFFGetR(buf[i]);
        g[i] = (int)TIFFGetG(buf[i]);
        b[i] = (int)TIFFGetB(buf[i]);
      }                
      fflush(stdout);
    } else {
    }
  } else {
    printf("Error allocating memory...\n");  
  }
  TIFFClose(tif);
  _TIFFfree(buf);
  return;
}

void reduce(int* r, int* nr, int* w, int* h, double* p) 
{
  int nw, nh, x, y, i;
  nw = ceil((1-*p) * *w);
  nh = ceil((1-*p) * *h);
  for(y = 0; y < nh; y++) {
    for(x = 0; x < nw; x++) {
          i = (int)(floor(y / (1-*p)) * *w + floor(x / (1 - *p)));
          nr[y * nw + x] = r[i];
    }
  }
}
