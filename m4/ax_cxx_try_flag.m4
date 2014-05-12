dnl ===========================================================================
dnl check compiler flags
AC_DEFUN([AX_CXX_TRY_FLAG], [
  AC_MSG_CHECKING([whether $CXX supports $1])

  ax_save_CXXFLAGS="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS $1"

  AC_COMPILE_IFELSE([AC_LANG_SOURCE([ ])], [ax_cxx_flag=yes], [ax_cxx_flag=no])

  if test "x$ax_cxx_flag" = "xno"; then
    CXXFLAGS="$ax_save_CXXFLAGS"
  fi
  AC_MSG_RESULT([$ax_cxx_flag])
])
