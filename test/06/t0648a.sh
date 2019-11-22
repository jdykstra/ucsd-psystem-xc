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

TEST_SUBJECT="ucsdpsys_littoral vs case"
. test_prelude

cat > example.text << 'fubar'
(*$warning otherwise false*)
program example;
var
  x: integer;
begin
  case x of
  -2: writeln('minus two');
  -1: writeln('minus one');
  0: writeln('zero');
  1: writeln('one');
  2: writeln('two');
  otherwise writeln('dunno');
  end
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
// program example
int16_t x;


int
main(int argc, char **argv)
{
    switch (x)
    {
    case -2:
        writeln("minus two");
        break;

    case -1:
        writeln("minus one");
        break;

    case 0:
        writeln("zero");
        break;

    case 1:
        writeln("one");
        break;

    case 2:
        writeln("two");
        break;

    default:
        writeln("dunno");
        break;
    }
}
fubar
test $? -eq 0 || no_result

ucsdpsys_littoral -o test.out example.text
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass


// vim: set ts=8 sw=4 et :
