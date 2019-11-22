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

TEST_SUBJECT="assembler .error"
. test_prelude

cat > example.text << 'fubar'
    .error "testing, testing", 1, 2, 3
    .arch "6502"
    .proc example
    rts
    .end
fubar
test $? -eq 0 || no_result

# Yes, this is the intended behaviour.
# If you want more spaces, use constant strings.
cat > ok << 'fubar'
ucsdpsys_assemble: example.text: 1: testing, testing123
ucsdpsys_assemble: example.text: found one fatal error
fubar
test $? -eq 0 || no_result

ucsdpsys_assemble example.text > test.out 2>&1
if test $? -ne 1
then
    echo "This was supposed to fail"
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
