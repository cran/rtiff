\name{newPixmapRGB}
\alias{newPixmapRGB}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{ Create an RGB pixmap }
\description{
  A utility function to create an RGB pixmap object out of 3 rasters.
}
\usage{
newPixmapRGB(red = NULL, green = NULL, blue = NULL)
}
\arguments{
  \item{red}{ The red raster (a matrix)  }
  \item{green}{ The green raster (a matrix) }
  \item{blue}{ The blue raster (a matrix) }
}
\value{
  An object of type pixmap representing the resulting RGB image.
}
\author{ Eric Kort <eric.kort@vai.org> }

\examples{

#this is tautological, but demonstrates the syntax...
library(rtiff)
tif <- readTiff(paste(.path.package("rtiff"), "/tiff/jello.tif", sep=""))
myPixmap <- newPixmapRGB(tif@red, tif@green, tif@blue)
plot(myPixmap)

#let's scramble the color channels just for fun

get(getOption("device"))()
myPixmap <- newPixmapRGB(tif@blue, tif@red, tif@green)
plot(myPixmap)

}

\keyword{ utilities }% at least one, from doc/KEYWORDS
