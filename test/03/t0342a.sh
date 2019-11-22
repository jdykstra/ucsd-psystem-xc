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

TEST_SUBJECT="char case labels"
. test_prelude

cat > example.text << 'fubar'
program example;
var c: char;
begin
  case c of
  'y', 'Y': writeln('yes');
  'n', 'N': writeln('no');
  end
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   164  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: EA                p1_0:   SLDO    3
****: B9 32                     UJP     p1_2
****: B6 01 03          p1_3:   LOD     1, 3
****: D7                        NOP
****: A6 02 6E 6F               LSA     2, "no"
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: B9 79                     UJP     p1_1
****: B6 01 03          p1_5:   LOD     1, 3
****: D7                        NOP
****: A6 03 79 65 73            LSA     3, "yes"
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: B9 60                     UJP     p1_1
****: D7                        NOP
****: AC 4E 00 79 00    p1_2:   XJP     78, 121         ; switch
      B9 58             p1_4:   UJP     p1_1            ; otherwise
      39 00                     .word   $ - p1_3        ; case 78 'N'
      04 00                     .word   $ - p1_4        ; case 79 'O'
      06 00                     .word   $ - p1_4        ; case 80 'P'
      08 00                     .word   $ - p1_4        ; case 81 'Q'
      0A 00                     .word   $ - p1_4        ; case 82 'R'
      0C 00                     .word   $ - p1_4        ; case 83 'S'
      0E 00                     .word   $ - p1_4        ; case 84 'T'
      10 00                     .word   $ - p1_4        ; case 85 'U'
      12 00                     .word   $ - p1_4        ; case 86 'V'
      14 00                     .word   $ - p1_4        ; case 87 'W'
      16 00                     .word   $ - p1_4        ; case 88 'X'
      37 00                     .word   $ - p1_5        ; case 89 'Y'
      1A 00                     .word   $ - p1_4        ; case 90 'Z'
      1C 00                     .word   $ - p1_4        ; case 91 '['
      1E 00                     .word   $ - p1_4        ; case 92 '\'
      20 00                     .word   $ - p1_4        ; case 93 ']'
      22 00                     .word   $ - p1_4        ; case 94 '^'
      24 00                     .word   $ - p1_4        ; case 95 '_'
      26 00                     .word   $ - p1_4        ; case 96 '`'
      28 00                     .word   $ - p1_4        ; case 97 'a'
      2A 00                     .word   $ - p1_4        ; case 98 'b'
      2C 00                     .word   $ - p1_4        ; case 99 'c'
      2E 00                     .word   $ - p1_4        ; case 100 'd'
      30 00                     .word   $ - p1_4        ; case 101 'e'
      32 00                     .word   $ - p1_4        ; case 102 'f'
      34 00                     .word   $ - p1_4        ; case 103 'g'
      36 00                     .word   $ - p1_4        ; case 104 'h'
      38 00                     .word   $ - p1_4        ; case 105 'i'
      3A 00                     .word   $ - p1_4        ; case 106 'j'
      3C 00                     .word   $ - p1_4        ; case 107 'k'
      3E 00                     .word   $ - p1_4        ; case 108 'l'
      40 00                     .word   $ - p1_4        ; case 109 'm'
      79 00                     .word   $ - p1_3        ; case 110 'n'
      44 00                     .word   $ - p1_4        ; case 111 'o'
      46 00                     .word   $ - p1_4        ; case 112 'p'
      48 00                     .word   $ - p1_4        ; case 113 'q'
      4A 00                     .word   $ - p1_4        ; case 114 'r'
      4C 00                     .word   $ - p1_4        ; case 115 's'
      4E 00                     .word   $ - p1_4        ; case 116 't'
      50 00                     .word   $ - p1_4        ; case 117 'u'
      52 00                     .word   $ - p1_4        ; case 118 'v'
      54 00                     .word   $ - p1_4        ; case 119 'w'
      56 00                     .word   $ - p1_4        ; case 120 'x'
      77 00                     .word   $ - p1_5        ; case 121 'y'
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 02 00                     .word   2               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 9C 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 02 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || fail

ucsdpsys_disassemble -a example.code -o test.out
test $? -eq 0 || no_result

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
