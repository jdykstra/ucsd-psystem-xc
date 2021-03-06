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

TEST_SUBJECT="librarian renumber segment"
. test_prelude

cat > example.text << 'fubar'
program example;
begin
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
     Library map for "test.code"

Code  Code   Segment   Segment  Seg Machine   System
Addr  Leng   Name      Kind     Num Type      Version
----- -----  --------  -------- --- --------- --------
    1    16  EXAMPLE   LINKED    42 P-Code LE 3.0
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || no_result

ucsdpsys_librarian --create test.code --copy example.code --segment example=42
test $? -eq 0 || fail

ucsdpsys_libmap -o test.out test.code
test $? -eq 0 || no_result

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
