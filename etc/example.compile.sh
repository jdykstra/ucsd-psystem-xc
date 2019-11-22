#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2010, 2012 Peter Miller
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

if test -f example.text
then
    : ok
else
    echo "where is the example.text file?" 1>&2
    exit 1
fi

dir=/home/vault/ucsd-p-system/version-2.1-apple/p-interp-0.5
set -x
cp $dir/apple_pascal_0.vol work.vol

ucsdpsys_disk -f work.vol -p example.text

ucsdpsys_vm -b- -w work.vol << 'fubar'
cexample
example
hhhhhhh
fubar

ucsdpsys_disk -f work.vol -g example.code

PATH=./linux-x86_64/bin:${PATH}
export PATH
ucsdpsys_disassemble -a example.code -o test.ok
# vim: set ts=8 sw=4 et :
