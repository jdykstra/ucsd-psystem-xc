#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program. If not, see
#       <http://www.gnu.org/licenses/>.
#

#
# where to get the distribution tarball from
#
case "$1" in
/*)
    src_tgz=$1
    ;;
*)
    src_tgz=`pwd`/$1
    ;;
esac

set -e
set -x
rm -rf check-tarball-tmp
mkdir check-tarball-tmp
cd check-tarball-tmp

tar xzf $src_tgz

cd ucsd-psystem-xc-*
./configure --prefix=/usr --sysconfdir=/etc

#
# Build the executables.
#
make

#
# Run all of the tests.
#
make sure

du -s -h

exit 0
# vim: set ts=8 sw=4 et :
