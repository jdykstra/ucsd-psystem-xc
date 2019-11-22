#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2006, 2007, 2012 Peter Miller
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or (at
# you option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>
#
tmp=/tmp/$$.doxygen.out
doxygen "$@" > $tmp 2>&1
test $? -eq 0 || exit 1

grep 'Warning:' $tmp > /dev/null
status=$?

sed 's|Warning:|Error:|' < $tmp
rm $tmp

# grep return ssuccess when it finds a Warning,
# but we need to fail.
test $status -eq 1
# vim: set ts=8 sw=4 et :
