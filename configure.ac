AC_INIT([adalyah], [0], [kilobyte@angband.pl])
AM_INIT_AUTOMAKE([-Wall dist-xz no-dist-gzip foreign])
AC_CONFIG_SRCDIR([adalyah.h])
AC_CONFIG_HEADERS([config.h])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CXX
AC_PROG_INSTALL

AX_CXX_COMPILE_STDCXX_11(noext, mandatory)

AX_CXX_TRY_FLAG([-Wmissing-declarations])
AX_CXX_TRY_FLAG([-Wredundant-decls])
AX_CXX_TRY_FLAG([-Wno-parentheses])

# Check if anyone reads build logs.
AC_SCO

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
