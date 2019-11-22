#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

TEST_SUBJECT="assignment error messages"
. test_prelude

#
# test assignment error messages
#
cat > test.text << 'fubar'
program a;
var
    x: record
        y: real
    end;
    b: boolean;
begin
    x.y := true;
    b := 0
end.
fubar
test $? -eq 0 || fail

cat > test.ok << 'fubar'
ucsdpsys_compile: test.text: 8: assignment type mismatch, unable to assign
    a boolean value...
ucsdpsys_compile: test.text: 8: ... to a real variable "y"
ucsdpsys_compile: test.text: 9: assignment type mismatch, unable to assign
    an integer value...
ucsdpsys_compile: test.text: 9: ... to a boolean variable "b"
ucsdpsys_compile: found two fatal errors
fubar
test $? -eq 0 || fail

$bindir/ucsdpsys_compile test.text > test.out 2>&1
test $? -eq 1 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
