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

TEST_SUBJECT="string call size mismatch"
. test_prelude

cat > example.text << 'fubar'
program example;
var s: string;

  procedure check_me(fred: string[6]);
  begin
    writeln(fred)
  end;

begin
  check_me(s);
  check_me('1234567')
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
ucsdpsys_compile: example.text: 10: warning: call of procedure "check_me":
    parameter one: maximum string length of parameter (80) is greater
    than...
ucsdpsys_compile: example.text: 4: warning: ... declared maximum string
    length (6)
ucsdpsys_compile: example.text: 11: warning: call of procedure "check_me":
    parameter one: maximum string length of parameter (7) is greater
    than...
ucsdpsys_compile: example.text: 4: warning: ... declared maximum string
    length (6)
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text > test.out 2>&1
if test $? -ne 0
then
    cat test.out
    fail
fi

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
