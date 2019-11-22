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

TEST_SUBJECT="check project name shadowing"
. test_prelude

#
# check project name shadowing
#
cat > test.text << 'fubar'
(*$warning error true*)
program chr;
begin
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
ucsdpsys_compile: test.text: 2: program name "chr" shadows the "chr"
    function
ucsdpsys_compile: found one fatal error
fubar
test $? -eq 0 || no_result

$bindir/ucsdpsys_compile test.text > test.out 2>&1
if test $? -ne 1
then
    cat test.out
    echo "The compile is supposed to fail."
    fail
fi

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
