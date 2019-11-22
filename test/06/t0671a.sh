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

TEST_SUBJECT="ucsdpsys_depends"
. test_prelude

mkdir system
test $? -eq 0 || no_result

cat > system/main.text << 'fubar'
program example;
{$I part1.text}
begin
end.
fubar
test $? -eq 0 || no_result

cat > system/part1.text << 'fubar'
(*$I part2.text*)
fubar
test $? -eq 0 || no_result

cat > system/part2.text << 'fubar'
(*hello*)
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
system/part1.text
system/part2.text
fubar
test $? -eq 0 || no_result

ucsdpsys_depends --one-per-line -J. -Jbl -r -Isystem system/main.text > test.out
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
