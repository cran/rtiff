".onLoad" <-
  function(lib, pkg) {
  library.dynam("rtiff", pkg, lib)
  require(pixmap)
}
