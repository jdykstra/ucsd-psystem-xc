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

TEST_SUBJECT="function call locations"
. test_prelude

cat > example.text << 'fubar'
program example;

  procedure fetid;
  var
    f: text;
  begin
    openold(f, 'noxious.text');
  end;

begin
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; example.text: 5:         0  301  variable    f         text
; Procedure 2:
; example.text: 1:      program example;
; example.text: 2:
; example.text: 3:        procedure fetid;
; example.text: 4:        var
; example.text: 5:          f: text;
0000: C6 01             p2_0:   LLA     1
0002: C6 81 2D                  LLA     301
0005: 02                        SLDC    2
0006: 91                        NGI
0007: CD 00 03                  CXP     0, 3            ; finit
; example.text: 6:        begin
; example.text: 7:          openold(f, 'noxious.text');
000A: C6 01                     LLA     1
000C: D7                        NOP
000D: A6 0C 6E 6F 78            LSA     12, "noxious.text"
      69 6F 75 73 2E
      74 65 78 74
001B: 01                        SLDC    1
001C: 9F                        LDCN
001D: CD 00 05                  CXP     0, 5            ; fopen
0020: 9E 00                     CSP     IOCHECK
; example.text: 8:        end;
0022: C6 01             p2_1:   LLA     1
0024: 00                        SLDC    0
0025: CD 00 06                  CXP     0, 6            ; fclose
0028: AD 00                     RNP     0
; Procedure 2 Attribute Table:
002A: 5A 02                     .word   602             ; data size
002C: 00 00                     .word   0               ; param size
002E: 0C 00                     .word   $ - p2_1        ; exit ic
0030: 30 00                     .word   $ - p2_0        ; enter ic
0032: 02                p2_at:  .byte   2               ; procedure number
0033: 01                        .byte   1               ; lex level
; example.text: 1:         0    1  parameter   bogus:out text
; example.text: 3:                 procedure   fetid
; example.text: 1:         1    1  parameter   bogus:in  text
; example.text: 1:                 program     example   program
; Procedure 1:
; example.text: 9:
; example.text: 10:     begin
; example.text: 11:     end.
0034: C1 00             p1_0:   RBP     0
; Procedure 1 Attribute Table:
0036: 00 00                     .word   0               ; data size
0038: 04 00                     .word   4               ; param size
003A: 06 00                     .word   $ - p1_0        ; exit ic
003C: 08 00                     .word   $ - p1_0        ; enter ic
003E: 01                p1_at:  .byte   1               ; procedure number
003F: 00                        .byte   0               ; lex level
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text --listing=test.out
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
