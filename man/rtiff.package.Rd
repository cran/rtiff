\name{rtiff-package}
\alias{rtiff-package}
\alias{rtiff}
\docType{package}
\title{
A tiff reader for R.
}
\description{

This package will read TIFF format images and return them as a pixmap object.
Because the resulting object can be very large for even modestly sized TIFF images, images
can be reduced as they are read for improved performance.  This package is a wrapper around
libtiff (www.libtiff.org), on which it depends.  By using libtiff's highlevel TIFFReadRGBAImage 
function, this package inherently support a wide range of image formats and compression 
schemes.  This package also provides an implementation of the Ridler Autothresholding algorithm
for easy generation of binary masks.

}
\details{
\tabular{ll}{
Package: \tab rtiff\cr
Type: \tab Package\cr
Version: \tab 1.0\cr
Date: \tab 2005-12-20\cr
License: \tab GPL\cr
}

A tiff image can be loaded using readTiff, and then plotted, or processed.  Regardless of the 
colorspace of the original image, this function returns an RGB pixmap.  The image rasters 
can be accessed through the red, green, and blue slots of the pixmap object returned.  These
rasters may be further processed using any of the widely described image analysis alorithms.
As a first step, and autoThreshold function is provided by this package to create binary 
masks of pixmap channels of interest.

}
\author{
Eric Kort <eric.kort@vai.org>
Maintainer: Eric Kort <eric.kort@vai.org>
}
\references{
  www.libtiff.org
}

\examples{
library(rtiff)
tif <- readTiff(system.file("tiff", "jello.tif", package="rtiff"))
plot(tif)


plot(tif@blue)

plot(tif@blue > autoThreshold(tif@blue)[3])

}
\keyword{IO}