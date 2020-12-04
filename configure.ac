: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  echo "could not determine R_HOME"
  exit 1
fi
CC=`"${R_HOME}/bin/R" CMD config CC`
CFLAGS=`"${R_HOME}/bin/R" CMD config CFLAGS`
CPPFLAGS=`"${R_HOME}/bin/R" CMD config CPPFLAGS`

AC_PREREQ(2.1)
AC_INIT([rtiff], [1.1], [eric.kort@vai.org])
AC_CONFIG_SRCDIR([src/rtiff.c])

# Checks for programs.
AC_PROG_CC

# Checks for libtiff.
AC_CHECK_LIB([tiff], [TIFFOpen], [have_tiff=yes], [have_tiff=no])
if test "x${have_tiff}" = xyes; then
PKG_LIBS="-ltiff"
else
  ## version for Mac OS X, static libtiff 4.0.x
  unset ac_cv_lib_tiff_TIFFOpen
  echo 'Try static libs needed on OS X'
  AC_CHECK_LIB([tiff], [TIFFOpen], [have_tiff=yes], [have_tiff=no],
               [-llzma -ljpeg -lz])
  if test "x${have_tiff}" = xyes; then
    PKG_LIBS="-ltiff -llzma -ljpeg -lz"
  else
                echo Error: Could not find libtiff.
                exit -1
  fi
fi

AC_SUBST(PKG_LIBS)
AC_OUTPUT([src/Makevars])
