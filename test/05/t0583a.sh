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

TEST_SUBJECT="ucsdpsys_setup"
. test_prelude

dd if=/dev/zero of=test.in bs=512 count=1 2>/dev/null
test $? -eq 0 || no_result

cat > ok << 'fubar'
Has clock :=                          false
Has 8510a :=                          false
Has lower case :=                     false
Has random cursor addressing :=       false
Has slow terminal :=                  false
Student :=                            false
Has byte flipped machine :=           false
Has word oriented machine :=          false
Lead in to screen :=                  NUL
Move cursor home :=                   NUL
Erase to end of screen :=             NUL
Erase to end of line :=               NUL
Move cursor right :=                  NUL
Move cursor up :=                     NUL
Vertical move delay :=                0
Backspace :=                          NUL
Erase line :=                         NUL
Erase screen :=                       NUL
Prefixed[Move cursor up] :=           false
Prefixed[Move cursor right] :=        false
Prefixed[Erase to end of line] :=     false
Prefixed[Erase to end of screen] :=   false
Prefixed[Move cursor home] :=         false
Prefixed[Delete character] :=         false
Prefixed[Erase screen] :=             false
Prefixed[Erase line] :=               false
Screen height :=                      0
Screen width :=                       0
Key to move cursor up :=              NUL
Key to move cursor down :=            NUL
Key to move cursor left :=            NUL
Key to move cursor right :=           NUL
Key to end file :=                    NUL
Key for flush :=                      NUL
Key for break :=                      NUL
Key for stop :=                       NUL
Key to delete character :=            NUL
Non-printing character :=             NUL
Key to delete line :=                 NUL
Editor escape key :=                  NUL
Lead in from keyboard :=              NUL
Editor accept key :=                  NUL
Prefixed[Key to move cursor right] := false
Prefixed[Key to move cursor left] :=  false
Prefixed[Key to move cursor up] :=    false
Prefixed[Key to move cursor down] :=  false
Prefixed[Non-printing character] :=   false
Prefixed[Key for stop] :=             false
Prefixed[Key for break] :=            false
Prefixed[Key for flush] :=            false
Prefixed[Key to end file] :=          false
Prefixed[Editor escape key] :=        false
Prefixed[Key to delete line] :=       false
Prefixed[Key to delete character] :=  false
Prefixed[Editor accept key] :=        false
fubar
test $? -eq 0 || no_result

ucsdpsys_setup -d test.in test.out
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
