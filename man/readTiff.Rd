\name{readTiff}
\alias{readTiff}
\title{ A function to load TIFF images into a pixmap. }
\description{
  Loads a TIFF image from a file and returns the image as a pixmap object, with optional scaling.
}
\usage{
readTiff(fn, page = 0, reduce = 0, pixmap = TRUE)
}

\arguments{
  \item{fn}{ Filename (the tiff image to load) }
  \item{page}{ In the case of multi-page tiffs, which page do you want? }
  \item{reduce}{ Optional scaling factor to improve performance with large images, 
                 should be a value between 0 and 1 (i.e. a decimal representation of 
                 a percentage).  See details. }
  \item{pixmap}{ By default, readTiff returns a pixmap object, ideal for plotting.  However pixmap 
                 rescales the values to [0..1].  If you want the raw values from the file, set 
                 pixmap=FALSE to get a list of r, g, and b values, unscaled. }
}
\details{


This package is a wrapper around libtiff (www.libtiff.org), on which it depends.  By using libtiff's 
highlevel TIFFReadRGBAImage function, this package inherently support a wide range of image formats 
and compression schemes (interestingly, thanks to libtiff, this package can load a number of TIFF formats 
that tools like Window's Paint or the open source Gimp application will not load).

High resolution images (by which I mean images that contain a "large" number of pixels) will occupy 
quite a bit of memory and will also plot very slowly.  If you do not need all the resolution for your
purposes, you scan specify a scaling factor (reduce=x) to downsample the image.  The factor is the 
amount you want the image scaled BY, not TO.  In otherwords, reduce=.90 will reduce the image by 90%, 
yielding an image 10% the size of the orginal.  The scaling is not sophisticated...a nearest neighbor 
approach is used.  Since we are reducing and not enlarging, I hope this will be suitable for your 
applications.

For simplicity, an RGB pixmap is generated regardless of the colorspace of the original image (including
grayscale images, in which case the R, G, and B rasters are identical).  The pixmap object requires 
pixel intensities to be between 0 and 1, so the intensities in the orginal image file are scaled 
accordingly.  Alternatively, specify pixmap=FALSE to return a list of unscale r, g, and b values instead of a 
pixmap object.

}
\value{
   A pixmap object containing the image rasters.
}

\author{ Eric Kort <eric.kort@vai.org> }


\examples{
tif <- readTiff(system.file("tiff", "jello.tif", package="rtiff"))
plot(tif)
}

\keyword{ utilities }
\keyword{ IO }
\keyword{ file }
