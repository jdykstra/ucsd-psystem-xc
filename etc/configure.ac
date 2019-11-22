dnl
dnl     UCSD p-System cross compiler
dnl     Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
dnl
dnl     This program is free software; you can redistribute it and/or modify
dnl     it under the terms of the GNU General Public License as published by
dnl     the Free Software Foundation; either version 3 of the License, or
dnl     (at your option) any later version.
dnl
dnl     This program is distributed in the hope that it will be useful,
dnl     but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl     GNU General Public License for more details.
dnl
dnl     You should have received a copy of the GNU General Public License
dnl     along with this program. If not, see
dnl     <http://www.gnu.org/licenses/>.
dnl
AC_INIT(install-sh)
AC_CONFIG_HEADER(lib/config.h)
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_YACC
AC_PROG_RANLIB
AC_TYPE_SIGNAL
AC_SYS_LARGEFILE

AC_CHECK_PROGS(SOELIM, gsoelim soelim)
AC_CHECK_PROGS(AR, ar)
AC_CHECK_LIB(m, main)
AC_CHECK_LIB(explain, explain_open)
AC_CHECK_LIB(png, png_create_write_struct)

AC_CHECK_FUNCS(         \
    backtrace           \
    fnmatch             \
)

AC_CHECK_HEADERS([      \
    fnmatch.h           \
    getopt.h            \
    stdint.h            \
])

dnl
dnl     If the soelim program exists, and understands the -I option,
dnl     arrange for the Makefile to install the .ps files from the
dnl     documentation source files.
dnl
if test -n "$SOELIM"
then
  if $SOELIM -I. /dev/null > /dev/null 2>&1
  then
    : nothing
  else
    AC_MSG_RESULT([
        The $SOELIM program does not understand the -I option.
        GNU Groff 1.15 or later works correctly.
        See the BUILDING file for details.])
    GROFF=
  fi
fi

AC_CHECK_PROGS(GROFF, groff roff)
if test -n "$GROFF"
then
  HAVE_GROFF=yes
else
  HAVE_GROFF=no
  AC_MSG_RESULT([
        The Aegis documentation set and manual pages will
        be formatted and installed if you first install
        GNU Groff 1.15 or later and then re-run this ./configure script.
        Remember to use \`\`rm config.cache'' before you do.
])
fi
AC_SUBST(HAVE_GROFF)

dnl
dnl     Test to find a Bourne shell which understands functions
dnl
AC_MSG_CHECKING([for a Bourne shell which understands functions])
if test "z$SH" = "z"; then
    if test -f /bin/sh5; then
        SH=/bin/sh5
    else
        SH=/bin/sh
    fi
fi
AC_SUBST(SH)
AC_DEFINE_UNQUOTED(CONF_SHELL, ["$SH"],
[Set this to be the absolute path of a Bourne shell
which understands functions.])
AC_MSG_RESULT($SH)

dnl
dnl Evaluate some of the variables, to remove ${prefix} references.
dnl This way, they can be used in C programs and Roff input.
dnl Make sure that ucsd-psystem-xc is mentioned in the libdir and datadir paths;
dnl add it if it is not already there.
dnl
test "x$prefix" = xNONE && prefix="${ac_default_prefix-/usr/local}"
test "x$exec_prefix" = xNONE && exec_prefix="$prefix"
eval "bindir=$bindir"
eval "datarootdir=$datarootdir"
eval "datadir=$datadir"
eval "libdir=$libdir"
eval "mandir=$mandir"
eval "sysconfdir=$sysconfdir"

AH_TOP([/*
 * UCSD p-System cross compiler
 * Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 3 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program. If not, see
 *      <http://www.gnu.org/licenses/>.
 */

#ifndef LIB_CONFIG_H
#define LIB_CONFIG_H
])

AH_BOTTOM([
/*
 * There is more to do, but we need to insulate it from config.status,
 * because it screws up the #undef lines.  They are all implications of
 * the above information, so there is no need for you to edit the file,
 * if you are configuring Aegis manually.
 */
#include <lib/config.messy.h>

#endif /* LIB_CONFIG_H */])

AC_OUTPUT(Makefile)
dnl! vim: set ts=8 sw=4 et :
