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

TEST_SUBJECT="CONST vs pretty"
. test_prelude

cat > example.text << 'fubar'
(*comment*)
PROGRAM EXAMPLE;

const
    MAXSEG = 15;        { max code seg # in code files }
    MAXSEG1 = 16;       { MAXSEG+1, useful for loop vars }
    MAXLC = MAXINT;     { max compiler assigned address }
    MAXIC = 2400;       { max number bytes of code per proc }
    MAXPROC = 160;      { max legal procedure number }

BEGIN
  WRITELN(MAXLC)
END.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
program EXAMPLE;
const
    MAXSEG = 15;
    MAXSEG1 = 16;
    MAXLC = maxint;
    MAXIC = 2400;
    MAXPROC = 160;
begin
    writeln(MAXLC)
end.
fubar
test $? -eq 0 || no_result

ucsdpsys_pretty example.text -o test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
