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

TEST_SUBJECT="record case vs semicolons"
. test_prelude

cat > example.text << 'fubar'
program example;
var
  temp1,
  temp2: packed record
           case boolean of
           TRUE:  (val: integer);
           FALSE: (lowbyte: 0..255;
                   highbyte:0..255);
         end;
begin
end.
fubar
test $? -eq 0 || no_result

# This test is for the semicolon following highbyte, above.
# It was once throwing a syntax error at that point.
ucsdpsys_compile example.text
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
