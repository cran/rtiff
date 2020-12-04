"writeTiff" <-
function(pixmap, fn) {
      if(class(pixmap)[1] == "pixmapRGB"){
        .Call("C_writeTiff", pixmap@red, pixmap@green, pixmap@blue, fn)
      } else if(class(pixmap)[1] == "matrix") {
        pixmap = newPixmapRGB(pixmap, pixmap, pixmap);
        .Call("C_writeTiff", pixmap@red, pixmap@green, pixmap@blue, fn)
      } else {
        stop(paste("writeTiff expects a pixmapRGB or matrix, got ", class(pixmap)))
      }
      gc();
      return();
}
