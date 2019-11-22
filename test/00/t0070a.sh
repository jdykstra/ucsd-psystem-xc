#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

TEST_SUBJECT="dot not appropriate"
. test_prelude

#
# dot not appropriate
#
cat > test.pas << 'fubar'
program test;
var
    a, b: integer;
begin
    a := b.c
end.
fubar
test $? -eq 0 || no_result

$bindir/ucsdpsys_compile test.pas > LOG 2>&1
if test $? -ne 1
then
    cat LOG
    echo "Compile succeeded, it must fail" 1>&2
    fail
fi

grep 'the left hand side of the dot' LOG > /dev/null
if test $? -ne 0
then
    cat LOG
    echo "Expected error message not present" 1>&2
    fail
fi

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
