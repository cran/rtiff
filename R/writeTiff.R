"writeTiff" <-
function(pixmap, fn) {
      if(inherits(pixmap, "pixmapRGB")) {
        .Call("C_writeTiff", pixmap@red, pixmap@green, pixmap@blue, fn)
      } else if(inherits(pixmap, "matrix")) {
        pixmap = newPixmapRGB(pixmap, pixmap, pixmap);
        .Call("C_writeTiff", pixmap@red, pixmap@green, pixmap@blue, fn)
      } else {
        stop(paste("writeTiff expects a pixmapRGB or matrix, got ", class(pixmap)))
      }
      gc()
      invisible(NULL)
}
