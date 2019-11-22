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

TEST_SUBJECT="ucsdpsys_errors"
. test_prelude

cat > test.in << 'fubar'
# this s a comment
1:   undefined label
3:must have procedure name

4: number of parameters expected
86: register 'A' expected
2: operand out of range
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
  1: undefined label
  2: operand out of range
  3: must have procedure name
  4: number of parameters expected
 86: register 'A' expected
fubar
test $? -eq 0 || no_result

ucsdpsys_errors --arch=z80 --encode test.in bin
test $? -eq 0 || fail

ucsdpsys_errors --arch=z80 --decode bin test.out
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
