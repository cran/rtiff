\name{getDescription}
\alias{getDescription}
\title{ Get the description on a TIFF file. }
\description{
  Gets the TIFF description field (tag 270) for a TIFF file.
}
\usage{
getDescription(fn)
}
\arguments{
  \item{fn}{ The name of the tiff file. }
}
\value{
  A string with the description field from the TIFF file.
}
\author{ Eric Kort <eric.kort@vai.org> }
\examples{

library(rtiff)
tif <- readTiff(paste(.path.package("rtiff"), "/tiff/jello.tif", sep=""))
getDescription(paste(.path.package("rtiff"), "/tiff/jello.tif", sep=""))
}
\keyword{ utilities }
\keyword{ IO }
\keyword{ file }
