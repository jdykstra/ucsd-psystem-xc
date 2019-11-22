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

TEST_SUBJECT="pretty SCAN"
. test_prelude

cat > example.text << 'fubar'
PROGRAM EXAMPLE;VAR DEM:PACKED ARRAY[0..52]OF CHAR;BEGIN
DEM := '.....THE TERAK IS A MEMBER OF THE PTERODACTYL FAMILY.';
WRITELN(SCAN(-26,=':',DEM[30]));WRITELN(SCAN(100,<>'.',DEM));
WRITELN(SCAN(15,=' ',DEM[0]));END.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
program EXAMPLE;
var
    DEM: packed array [0..52] of char;
begin
    DEM := '.....THE TERAK IS A MEMBER OF THE PTERODACTYL FAMILY.';
    writeln(scan(-26, = ':', DEM[30]));
    writeln(scan(100, <> '.', DEM));
    writeln(scan(15, = ' ', DEM[0]))
end.
fubar
test $? -eq 0 || no_result

ucsdpsys_pretty example.text -o test.out
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
