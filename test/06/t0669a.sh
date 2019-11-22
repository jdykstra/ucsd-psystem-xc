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

TEST_SUBJECT="ucsdpsys_librarian -X"
. test_prelude

cat > example.text << 'fubar'
(*$U-*)
program dummy0;

  segment procedure userprog(i, j: integer);
  begin
  end;

  segment procedure dummy2;
  begin
  end;

  segment procedure dummy3;
  begin
  end;

  segment procedure dummy4;
  begin
  end;

  segment procedure dummy5;
  begin
  end;

  segment procedure dummy6;
  begin
  end;

  segment procedure this_is_ok;
  begin
  end;

begin
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
     Library map for "example.code"

Code  Code   Segment   Segment  Seg Machine   System
Addr  Leng   Name      Kind     Num Type      Version
----- -----  --------  -------- --- --------- --------
    1    16  USERPROG  LINKED     1 P-Code LE 3.0
    2    16  THISISOK  LINKED     7 P-Code LE 3.0
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || no_result

ucsdpsys_librarian --file=example.code --remove-system-segments
test $? -eq 0 || fail

ucsdpsys_libmap -o test.out example.code
test $? -eq 0 || no_result

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
