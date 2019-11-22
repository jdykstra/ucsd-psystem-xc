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

TEST_SUBJECT="ucsdpsys_downcase"
. test_prelude

cat > test.in << 'fubar'
(* Comments are unchanged *)
PROGRAM EXAMPLE;
BEGIN
  WRITELN('Strings are unchanged.');
END.
{ Even Ugly Comments Are Unchanged }
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
(* Comments are unchanged *)
program example;
begin
  writeln('Strings are unchanged.');
end.
{ Even Ugly Comments Are Unchanged }
fubar
test $? -eq 0 || no_result

ucsdpsys_downcase < test.in > test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
