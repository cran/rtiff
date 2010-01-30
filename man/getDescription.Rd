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
  A string with the description field from the TIFF file, or \code{NA}
  if there is no such field.
}
\author{ Eric Kort <eric.kort@vai.org> }
\examples{
# This has no description
getDescription(system.file("tiff", "jello.tif", package="rtiff"))
# but this has
getDescription(system.file("tiff", "t1.tif", package="rtiff"))
}
\keyword{ utilities }
\keyword{ IO }
\keyword{ file }
