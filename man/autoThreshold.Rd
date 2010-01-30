\name{autoThreshold}
\alias{autoThreshold}
\title{ Suggests threshold levels to use in binarizing an image channel. }
\description{
  This is an implementation of the Ridler method for binarization (see references).  NOTE: The order of the values returned did
  not match what is described in the "value" section below.  This is fixed in this version.
}
\usage{
autoThreshold(d.m, est = 0.5)
}
\arguments{
  \item{d.m}{ A data matrix representing the pixel intensities for a single image channel (e.g. readTiff("image.tif")@red). }
  \item{est}{ The initial thresholding estimate to work from.  The default only works if the pixel intensities are between 0 and 1,
              otherwise the mean intensity of the entire channel is likely a reasonable starting point.  The selection of the 
              estimate should not influence the final threshold selected. }
}
\value{
  A vector (v) of estimates, the 3rd element of which is the true Ridler estimate.  However, experience demonstrates
  that sometimes a lower (elements 1 or 2) or higher (elements 4 or 5) estimate performs better for a given application.
  The Ridler estimate is the mean between the average intensity of bright regions in the raster and the average intensity
  of dim regions.  This is v[3].  v[1] is the average of dim regions, v[5] is the average of dim regions, and v[2] and v[4]
  are the mean between the Ridler estimate and v[1] and v[2], respectively.  
}
\references{ Ridler T, Calvard S. Picture thresholding using an iterative selection method. IEEE Trans on Systems Man and Cybernetics, SMC 8;630-2. }
\author{ Eric Kort <eric.kort@vai.org> }
\examples{
tif <- readTiff(system.file("tiff", "jello.tif", package="rtiff"))
threshold <- autoThreshold(tif@red)
plot(tif)
plot(tif@red > threshold[3])
}
\keyword{ utilities }
\keyword{ IO }
\keyword{ file }
