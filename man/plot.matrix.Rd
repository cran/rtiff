\name{plot.matrix}
\alias{plot.matrix}
\title{ Overload the plot function for matrices }
\description{
  This is a simple S3 method for the plot function which takes a matrix,
  converts it to an RGB pixmap (by replicating it 3 times, once for each
  channel), and then plotting the resulting image.
}
\usage{
\S3method{plot}{matrix}(x, ...)
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
tif <- readTiff(system.file("tiff", "jello.tif", package="rtiff"))
plot(tif@red) 
}
\keyword{ utilities }
