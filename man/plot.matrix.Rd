\name{plot.matrix}
\alias{plot.matrix}
\title{ Overload the plot function for matrices }
\description{
  This is a simple overloader for the plot function which takes a matrix,
  converts it to and RGB pixmap (by replicating it 3 times, once for each
  channel), and then plotting the resulting image.
}
\usage{
plot.matrix(x, ...)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{x}{ A matrix representing an image raster. }
  \item{...}{Additional arguments to send to plot. }
}
\value{
  None.
}
\author{ Eric Kort <eric.kort@vai.org> }
\examples{

library(rtiff)
tif <- readTiff(paste(.path.package("rtiff"), "/tiff/jello.tif", sep=""))

#note the overload...specifying plot.matrix is not necessary.
plot(tif@red) 

}
\keyword{ utilities }
