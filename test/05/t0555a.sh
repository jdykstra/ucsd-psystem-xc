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

TEST_SUBJECT="CASE negative"
. test_prelude

cat > example.text << 'fubar'
program example;
var x: integer;
begin
  case x of
  -2: write('abstruse');
  -1: write('arcane');
  0: write('enigmatic');
  1: write('inscrutable');
  2: write('obscure');
  end;
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   146  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: EA                p1_0:   SLDO    3
****: B9 6E                     UJP     p1_2
****: B6 01 03          p1_3:   LOD     1, 3
****: D7                        NOP
****: A6 08 61 62 73            LSA     8, "abstruse"
      74 72 75 73 65
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B9 69                     UJP     p1_1
****: B6 01 03          p1_4:   LOD     1, 3
****: D7                        NOP
****: A6 06 61 72 63            LSA     6, "arcane"
      61 6E 65
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B9 55                     UJP     p1_1
****: B6 01 03          p1_5:   LOD     1, 3
****: D7                        NOP
****: A6 09 65 6E 69            LSA     9, "enigmatic"
      67 6D 61 74 69
      63
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B9 3E                     UJP     p1_1
****: B6 01 03          p1_6:   LOD     1, 3
****: A6 0B 69 6E 73            LSA     11, "inscrutable"
      63 72 75 74 61
      62 6C 65
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B9 26                     UJP     p1_1
****: B6 01 03          p1_7:   LOD     1, 3
****: A6 07 6F 62 73            LSA     7, "obscure"
      63 75 72 65
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B9 12                     UJP     p1_1
****: D7                        NOP
****: AC FE FF 02 00    p1_2:   XJP     -2, 2           ; switch
      B9 0A                     UJP     p1_1            ; otherwise
      75 00                     .word   $ - p1_3        ; case -2
      61 00                     .word   $ - p1_4        ; case -1
      4F 00                     .word   $ - p1_5        ; case 0
      3A 00                     .word   $ - p1_6        ; case 1
      24 00                     .word   $ - p1_7        ; case 2
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 02 00                     .word   2               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 8A 00                     .word   $ - p1_0        ; enter ic
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
