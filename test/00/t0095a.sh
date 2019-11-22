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

TEST_SUBJECT="missing comma"
. test_prelude

#
# missing comma
#
cat > test.pas << 'fubar'
program missing_comma;
    procedure fred(a b: integer);
    begin
    end;
begin
    fred(1 2)
end.
fubar
test $? -eq 0 || fail

cat > test.ok << 'fubar'
ucsdpsys_compile: test.pas: 2: missing comma
ucsdpsys_compile: test.pas: 6: missing comma
ucsdpsys_compile: found two fatal errors
fubar
test $? -eq 0 || fail

$bindir/ucsdpsys_compile test.pas -o test.code 2> test.err
test $? -eq 1 || fail

diff test.ok test.err
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
