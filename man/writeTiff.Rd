\name{writeTiff}
\alias{writeTiff}
\title{ A function to load TIFF images into a pixmap. }
\description{
  Loads a TIFF image from a file and returns the image as a pixmap object, with optional scaling.
}
\usage{
writeTiff(pixmap, fn)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{pixmap}{ Either a pixmapRGB or matrix containing the image to save.  In the case of a matrix,
                 a new pixmapRGB will be created from it.  The resulting TIFF file will be RGB, but
                 will appear grey because each channel will be identical. }
  \item{fn}{ What to call the new tiff file. }
}
\details{

This function saves the given pixmap or matrix raster as an unencrypted TIFF image, utilizing libtiff's 
TIFFWriteEncodedStrip, with the entire raster in a single strip (for simplicity).

}
\value{
   None.  Used for its handy side effect of creating a tiff file.
}

\author{ Eric Kort <eric.kort@vai.org> }


\examples{
tif <- readTiff(system.file("tiff", "jello.tif", package="rtiff"))
writeTiff(tif@red, "atesttif.tif")
}

\keyword{ utilities }
\keyword{ IO }
\keyword{ file }
