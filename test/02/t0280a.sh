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

TEST_SUBJECT="MOD"
. test_prelude

cat > example.text << 'fubar'
program example;
  procedure verify(x, y: integer);
  begin
    writeln(x: 5, y: 5, x mod y :5)
  end;
begin
  verify( 7,  3); (*  1 *)
  verify(-7,  3); (* -1 *)
  verify( 7, -3); (*  1 *)
  verify(-7, -3); (* -1 *)
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    90  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 2:
****: B6 02 03          p2_0:   LOD     2, 3
****: D9                        SLDL    2
****: 05                        SLDC    5
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 02 03                  LOD     2, 3
****: D8                        SLDL    1
****: 05                        SLDC    5
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 02 03                  LOD     2, 3
****: D9                        SLDL    2
****: D8                        SLDL    1
****: 8E                        MODI
****: 05                        SLDC    5
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 02 03                  LOD     2, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: AD 00             p2_1:   RNP     0
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p2_1        ; exit ic
****: 30 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: 07                p1_0:   SLDC    7
****: 03                        SLDC    3
****: CE 02                     CLP     2
****: 07                        SLDC    7
****: 91                        NGI
****: 03                        SLDC    3
****: CE 02                     CLP     2
****: 07                        SLDC    7
****: 03                        SLDC    3
****: 91                        NGI
****: CE 02                     CLP     2
****: 07                        SLDC    7
****: 91                        NGI
****: 03                        SLDC    3
****: 91                        NGI
****: CE 02                     CLP     2
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 1C 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 22 00                     .word   $ - p2_at       ; procedure 2
****: 04 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 02                        .byte   2               ; number of procedures
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
