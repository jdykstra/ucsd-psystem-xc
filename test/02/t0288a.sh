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

TEST_SUBJECT="compile listing"
. test_prelude

cat > example.text << 'fubar'
program example;
var x: integer;

  function fubar(q: integer): integer;
  begin
    fubar := q * 2
  end;

begin
  for x := 1 to
  10 do
    begin
      writeln
      (
        x:
        5,
        x*
        x:
        5
      )
    end
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; example.text: 4:         2    1  parameter   q         integer
; Procedure 2:
; example.text: 1:      program example;
; example.text: 2:      var x: integer;
; example.text: 3:
; example.text: 4:        function fubar(q: integer): integer;
; example.text: 5:        begin
; example.text: 6:          fubar := q * 2
0000: DA                p2_0:   SLDL    3
0001: 02                        SLDC    2
0002: 8F                        MPI
0003: CC 01                     STL     1
; example.text: 7:        end;
0005: AD 01             p2_1:   RNP     1
0007: D7                        NOP
; Procedure 2 Attribute Table:
0008: 00 00                     .word   0               ; data size
000A: 06 00                     .word   6               ; param size
000C: 07 00                     .word   $ - p2_1        ; exit ic
000E: 0E 00                     .word   $ - p2_0        ; enter ic
0010: 02                p2_at:  .byte   2               ; procedure number
0011: 01                        .byte   1               ; lex level
; example.text: 1:         0    1  parameter   bogus:out text
; example.text: 1:         1    1  parameter   bogus:in  text
; example.text: 2:         2    1  variable    x         integer
; example.text: 1:                 program     example   program
; example.text: 4:                 function    fubar     integer
; Procedure 1:
; example.text: 8:
; example.text: 9:      begin
; example.text: 10:       for x := 1 to
0012: 01                p1_0:   SLDC    1
0013: AB 03                     SRO     3
0015: EA                p1_2:   SLDO    3
; example.text: 11:       10 do
0016: 0A                        SLDC    10
0017: C8                        LEQI
0018: A1 25                     FJP     p1_1
; example.text: 12:         begin
; example.text: 13:           writeln
; example.text: 14:           (
; example.text: 15:             x:
001A: B6 01 03                  LOD     1, 3
001D: EA                        SLDO    3
; example.text: 16:             5,
001E: 05                        SLDC    5
001F: CD 00 0D                  CXP     0, 13           ; fwriteint
0022: 9E 00                     CSP     IOCHECK
0024: B6 01 03                  LOD     1, 3
; example.text: 17:             x*
0027: EA                        SLDO    3
; example.text: 18:             x:
0028: EA                        SLDO    3
0029: 8F                        MPI
; example.text: 19:             5
002A: 05                        SLDC    5
002B: CD 00 0D                  CXP     0, 13           ; fwriteint
002E: 9E 00                     CSP     IOCHECK
0030: B6 01 03                  LOD     1, 3
0033: CD 00 16                  CXP     0, 22           ; fwriteln
0036: 9E 00                     CSP     IOCHECK
0038: EA                        SLDO    3
0039: 01                        SLDC    1
003A: 82                        ADI
003B: AB 03                     SRO     3
003D: B9 F6                     UJP     p1_2
; example.text: 20:           )
; example.text: 21:         end
; example.text: 22:     end.
003F: C1 00             p1_1:   RBP     0
0041: D7                        NOP
; Procedure 1 Jump Table:
0042: 2D 00                     .word   $ - p1_2
; Procedure 1 Attribute Table:
0044: 02 00                     .word   2               ; data size
0046: 04 00                     .word   4               ; param size
0048: 09 00                     .word   $ - p1_1        ; exit ic
004A: 38 00                     .word   $ - p1_0        ; enter ic
004C: 01                p1_at:  .byte   1               ; procedure number
004D: 00                        .byte   0               ; lex level
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
