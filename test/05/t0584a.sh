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

cat > test.in << 'fubar'
(*
 * comment
 *)
backspace := bs
editor accept key := etx
editor escape key := esc
erase line := nul
erase screen := ff
erase to end of line := gs
erase to end of screen := vt
has 8510a := false
has byte flipped machine := false
has clock = true
has lower case := false
has random cursor addressing := true
has slow terminal := false
has word oriented machine := false
key for break := nul
key for flush := ack
key for stop := dc3
key to delete character := bs
key to delete line: can
key to end file := etx
key to move cursor down := ff
key to move cursor left := bs
key to move cursor right := nak
key to move cursor up := si
lead in from keyboard := nul
lead in to screen := nul
move cursor home := em
move cursor right := fs
move cursor up := us
non-printing character := 63
prefixed[delete character] := false
prefixed[editor accept key] := false
prefixed[editor escape key] := false
prefixed[erase line] := false
prefixed[erase screen] := false
prefixed[erase to end of line] := false
prefixed[erase to end of screen] := false
prefixed[key for break] := false
prefixed[key for flush] := false
prefixed[key for stop] := false
prefixed[key to delete character] := false
prefixed[key to delete line] := false
prefixed[key to end file] := false
prefixed[key to move cursor down] := false
prefixed[key to move cursor left] := false
prefixed[key to move cursor right] := false
{ another comment }
prefixed[key to move cursor up] := false
prefixed[move cursor home] := false
prefixed[move cursor right] := false
prefixed[move cursor up] := false
prefixed[non-printing character] := false
screen height := 24
screen width := 80
student := false
vertical move delay := 0
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
Has clock :=                          true
Has 8510a :=                          false
Has lower case :=                     false
Has random cursor addressing :=       true
Has slow terminal :=                  false
Student :=                            false
Has byte flipped machine :=           false
Has word oriented machine :=          false
Lead in to screen :=                  NUL
Move cursor home :=                   EM
Erase to end of screen :=             VT
Erase to end of line :=               GS
Move cursor right :=                  FS
Move cursor up :=                     US
Vertical move delay :=                0
Backspace :=                          BS
Erase line :=                         NUL
Erase screen :=                       FF
Prefixed[Move cursor up] :=           false
Prefixed[Move cursor right] :=        false
Prefixed[Erase to end of line] :=     false
Prefixed[Erase to end of screen] :=   false
Prefixed[Move cursor home] :=         false
Prefixed[Delete character] :=         false
Prefixed[Erase screen] :=             false
Prefixed[Erase line] :=               false
Screen height :=                      24
Screen width :=                       80
Key to move cursor up :=              SI
Key to move cursor down :=            FF
Key to move cursor left :=            BS
Key to move cursor right :=           NAK
Key to end file :=                    ETX
Key for flush :=                      ACK
Key for break :=                      NUL
Key for stop :=                       DC3
Key to delete character :=            BS
Non-printing character :=             63
Key to delete line :=                 CAN
Editor escape key :=                  ESC
Lead in from keyboard :=              NUL
Editor accept key :=                  ETX
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

ucsdpsys_setup -e test.in test.bin
test $? -eq 0 || fail

# Now we eat our own dog food.
ucsdpsys_setup -d test.bin test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
