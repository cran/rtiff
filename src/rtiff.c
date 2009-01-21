#include <math.h>
#include "tiffio.h"
#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

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

  uint16  count;
  char *data;

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

SEXP getTiffDescription(SEXP fn) {
  char* fileName; 
  TIFF *tiff;
  char* data;
  SEXP ans;
  fileName = (char*)CHAR(STRING_ELT(fn, 0)) ;
  if((tiff = TIFFOpen(fileName, "r")) == NULL){
    fprintf(stderr, "Could not open image file\n");
    return;
  }

  TIFFGetField(tiff, 270, &data);
  TIFFClose(tiff);  

  PROTECT(ans=NEW_CHARACTER(1));
  SET_STRING_ELT(ans, 0, mkChar(data));
  UNPROTECT(1);
  return(ans);
}

void writeTiff(SEXP mr, SEXP mg, SEXP mb, SEXP fn)
{
  TIFF *output;
  char *raster;
  int x, y;
  int h = INTEGER(GET_DIM(mr))[0];
  int w = INTEGER(GET_DIM(mr))[1];
  double *r = REAL(mr);
  double *g = REAL(mg);
  double *b = REAL(mb);
  char* fileName;
  fileName  = (char*)CHAR(STRING_ELT(fn, 0));

  // Open the output image
  if((output = TIFFOpen(fileName, "w")) == NULL){
    fprintf(stderr, "Could not open outgoing image\n");
    exit(42);
  }

  if((raster = (char*) malloc(sizeof(char*) * w * h * 3)) == NULL){
    fprintf(stderr, "Could not allocate enough memory\n");
    exit(42);
  }

  for (x=0; x<w; x++)
  {
    for (y=0; y<h; y++)
    {
      int index = 3 * (y * w + x);
      raster[index] = (char)(255.0 * r[y + x*h]);
      raster[index + 1] = (char)(255.0 * g[y + x*h]);
      raster[index + 2] = (char)(255.0 * b[y + x*h]);
    }
  }

  // Write the tiff tags to the file
  TIFFSetField(output, TIFFTAG_IMAGEWIDTH, w);
  TIFFSetField(output, TIFFTAG_IMAGELENGTH, h);
  TIFFSetField(output, TIFFTAG_COMPRESSION, 1);
  TIFFSetField(output, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  TIFFSetField(output, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
  TIFFSetField(output, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(output, TIFFTAG_SAMPLESPERPIXEL, 3);

  // Actually write the image
  if(TIFFWriteEncodedStrip(output, 0, raster, w * h * 3) == 0){
    fprintf(stderr, "Could not write image\n");
    exit(42);
  }

  TIFFClose(output);
  free(raster);
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


void updateTTag (SEXP fn, SEXP desc)
{
    TIFF *tiff;
    char* fileName;
    char* description;
    // Open the image
    fileName = (char*)CHAR(STRING_ELT(fn, 0)) ;
    description = (char*)CHAR(STRING_ELT(desc, 0)) ;
	
    if((tiff = TIFFOpen(fileName, "r+")) == NULL){
      fprintf(stderr, "Could not open outgoing image\n");
      exit(42);
    }

    const TIFFFieldInfo *fip;
    fip = TIFFFieldWithTag(tiff, 270);
    if (!fip) {
      fprintf(stderr, "Could not get field information\n");
      exit(42);
    }
    if (fip->field_type == TIFF_ASCII) {
        if (TIFFSetField(tiff, fip->field_tag, description) != 1)
            fprintf( stderr, "Failed to set field.");
    } else {
      fprintf(stderr, "Description field is not ascii\n");
      exit(42);
    }            
    TIFFRewriteDirectory(tiff);
    TIFFClose(tiff);
    return;
}

