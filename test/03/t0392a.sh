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

TEST_SUBJECT="WITH vs errors"
. test_prelude

cat > example.text << 'fubar'
program example;

type
  bar =
    record
      CODEVID: string[7];
      CODETID: string[15];
    end;
var
  foo: ^bar;

  procedure thingummy(fred: string[6]);
  begin
  end;

begin
  with foo^ do
    begin
      thingummy
      (
        concat
        (
          codevid, ':', codetid
        )
      )
    end
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
ucsdpsys_compile: example.text: 23: warning: call of procedure "thingummy":
    parameter one: maximum string length of parameter (23) is greater
    than...
ucsdpsys_compile: example.text: 12: warning: ... declared maximum string
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
