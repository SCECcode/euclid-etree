dnl GNU Autoconf config
dnl
dnl 2022/01/03 Brad Aagaard
dnl
dnl   Adapted from euclid3-1.3 from http://www.cs.cmu.edu/~euclid/ with initial
dnl   autotools build added by David Gill (SCEC).
dnl
dnl   * Remove checks for headers and functions, because the results were not
dnl     used and configure does not fail if they are not present.
dnl   * Limit build to libsrc.

AC_PREREQ(2.59)
AC_INIT([euclid], [3.1.4], [baagaard@usgs.gov])
AC_CONFIG_AUX_DIR([./aux-config])
AM_INIT_AUTOMAKE(tar-pax)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_INSTALL

AC_CONFIG_FILES([Makefile
                 libsrc/Makefile
		 ])
AC_OUTPUT
