"getDescription" <-
function(fn) {
    ret <- .Call("C_getTiffDescription", fn, PACKAGE="rtiff")
    ret
}
