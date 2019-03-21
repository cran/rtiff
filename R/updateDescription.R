"updateDescription" <-
function(fn, description) {
    .Call("C_updateTTag", fn, description, PACKAGE="rtiff")
    return()
}
