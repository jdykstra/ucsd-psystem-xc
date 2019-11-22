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

TEST_SUBJECT="littoral vs with array[idx]"
. test_prelude

cat > example.text << 'fubar'
(*$warning shadow false*)
program example;
const
  endmark = 2048;
type
  filentry =
    record
      statusword: integer;
      first: integer;
      second: integer;
      extension: integer;
      lgth: integer;
      filler: integer;
      date: integer
    end;
  direc =
    record case boolean of
      false: (
        buf: packed array [0..1023] of char);
      true: (
        segsavail: integer;
        nextseg: integer;
        highseg: integer;
        filler: integer;
        beginseg: integer;
        entry: array [0..71] of filentry)
    end;
var
  i: integer;
  rt11: direc;
begin
  with rt11 do
    while entry[i].statusword <> endmark do
      with entry[i] do
        begin
          writeln(statusword);
        end
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
// program example
const int16_t endmark = 2048;
typedef filentry filentry;
typedef direc direc;
int16_t i;
direc rt11;


int
main(int argc, char **argv)
{
    while ((rt11.entry[i]).statusword != endmark)
    {
        writeln((rt11.entry[i]).statusword);
    }
}
fubar
test $? -eq 0 || no_result

ucsdpsys_littoral example.text -o test.out
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass


// vim: set ts=8 sw=4 et :
