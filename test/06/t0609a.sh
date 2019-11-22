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

TEST_SUBJECT="class long_integer: signed subtraction"
. test_prelude

cat > test.in << 'fubar'
-3 - -4
-3 - -3
-3 - -2
-3 -  2

 3 - -2
 3 -  2
 3 -  3
 3 -  4
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
1
0
-1
-5

5
1
0
-1
fubar
test $? -eq 0 || no_result

test_long_integer test.in test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
