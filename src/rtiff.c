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

void TiffGetHeight (char** filename, int* h) 
{
    TIFF* tif;

    tif = TIFFOpen(*filename, "r");
    if (tif == 0) {
	*h = -1;
	return;
    }
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, h);
    TIFFClose(tif);


    return;
}


void TiffGetWidth (char** filename, int* w)
{
    TIFF* tif;

    tif = TIFFOpen(*filename, "r");
    if (tif == 0) {
	*w = -1;
	return;
    }
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, w);    
    TIFFClose(tif);
    return;
}

void TiffIsTiled(char **filename, int* yn)
{
    TIFF* tif;

    tif = TIFFOpen(*filename, "r");
    *yn = TIFFIsTiled(tif);
    TIFFClose(tif);
    return;
}

void TiffGetImageType (char** filename, int *dir, int* spp, int* pm, int* bps, int* tiled) 
{
    TIFF* tif;

    if((tif = TIFFOpen(*filename, "r")) == NULL)  return;

    if (TIFFSetDirectory(tif, *dir) != 1) {
	warning("Cannot find page %d in this tiff image\n", (*dir+1));
	return;
    }

    TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, pm);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, spp);        
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, bps);
    *tiled = TIFFIsTiled(tif);
    TIFFClose(tif);

    return;
}

void TiffReadTIFFRGBA (char** filename, int* dir, int* r, int* g, int* b)
{
    uint32 w, h;
    int i;
    uint32 *buf;
    TIFF* tif;

    if((tif = TIFFOpen(*filename, "r")) == NULL)  return;

    if (TIFFSetDirectory(tif, *dir) != 1) {
	warning("Cannot find page %d in this tiff image\n", (*dir+1));
	TIFFClose(tif);
	return;
    }

    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);

    buf = (uint32*) _TIFFmalloc(w * h * sizeof (uint32));
    if (buf != NULL) {
	if (TIFFReadRGBAImageOriented(tif, w, h, buf, 0, 1)) {
	    for (i=0; i<(w*h); i++) {
		r[i] = (int)TIFFGetR(buf[i]);
		g[i] = (int)TIFFGetG(buf[i]);
		b[i] = (int)TIFFGetB(buf[i]);
	    }                
	}
    } else
	error("Error allocating memory in TIFFmalloc");  
    TIFFClose(tif);
    _TIFFfree(buf);
}

SEXP getTiffDescription(SEXP fn)
{
    const char* filename = CHAR(STRING_ELT(fn, 0)) ;
    TIFF *tiff;
    char *data = NULL;
    int res;
    SEXP ans;

    if((tiff = TIFFOpen(filename, "r")) == NULL)
	error("Could not open image file '%s'", filename);

    res = TIFFGetField(tiff, TIFFTAG_IMAGEDESCRIPTION, &data);
    ans = res ? mkString(data) : ScalarString(NA_STRING);
    TIFFClose(tiff);
    return ans;
}

void writeTiff(SEXP mr, SEXP mg, SEXP mb, SEXP fn)
{
    TIFF *output;
    unsigned char *raster;
    int x, y;
    int h = INTEGER(GET_DIM(mr))[0];
    int w = INTEGER(GET_DIM(mr))[1];
    double *r = REAL(mr);
    double *g = REAL(mg);
    double *b = REAL(mb);
    const char* filename = CHAR(STRING_ELT(fn, 0));

    // Open the output image
    if((output = TIFFOpen(filename, "w")) == NULL)
	error("Could not open image file '%s'", filename);

    if((raster = (char*) malloc(sizeof(char*) * w * h * 3)) == NULL)
	error("Could not allocate enough memory");

    for (x=0; x<w; x++)
    {
	for (y=0; y<h; y++)
	{
	    int index = 3 * (y * w + x);
	    raster[index] = (unsigned char)(255.0 * r[y + x*h]);
	    raster[index + 1] = (unsigned char)(255.0 * g[y + x*h]);
	    raster[index + 2] = (unsigned char)(255.0 * b[y + x*h]);
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
    if(TIFFWriteEncodedStrip(output, 0, raster, w * h * 3) == 0)
	error("Could not write image");

    TIFFClose(output);
    free(raster);
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
#if TIFF_VERSION_CLASSIC >= 40
    error("Interface changed in libtiff 4, so no longer implemented");
#else
    TIFF *tiff;
    const char* filename = CHAR(STRING_ELT(fn, 0)) ;
    const char* description = CHAR(STRING_ELT(desc, 0)) ;	
    if((tiff = TIFFOpen(filename , "r+")) == NULL)
	error("Could not open image file '%s'", filename);
    const TIFFFieldInfo *fip;
    fip = TIFFFieldWithTag(tiff, 270);
    if (!fip) error("Could not get field information");
    if (fip->field_type == TIFF_ASCII) {
        if (TIFFSetField(tiff, fip->field_tag, description) != 1)
            error("Failed to set field.");
    } else error("Description field is not ascii");
    TIFFRewriteDirectory(tiff);
    TIFFClose(tiff);
#endif
}

