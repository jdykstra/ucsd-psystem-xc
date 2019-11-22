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

TEST_SUBJECT="string param copies"
. test_prelude

cat > example.text << 'fubar'
program example;

  procedure fred(a: integer; b: string; c: integer; d: string);
  const sym2 = 42;
  type sym1 = packed array [boolean] of char;
  begin
    a := 1;
    b := '22';
    c := 3;
    d := '44'
  end;

begin
  fred(1, '22', 3, '44')
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    78  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 2:
****: C6 05             p2_0:   LLA     5
****: DA                        SLDL    3
****: AA 50                     SAS     80
****: C6 2E                     LLA     46
****: D8                        SLDL    1
****: AA 50                     SAS     80
****: 01                        SLDC    1
****: CC 04                     STL     4
****: C6 05                     LLA     5
****: A6 02 32 32               LSA     2, "22"
****: AA 50                     SAS     80
****: 03                        SLDC    3
****: CC 02                     STL     2
****: C6 2E                     LLA     46
****: D7                        NOP
****: A6 02 34 34               LSA     2, "44"
****: AA 50                     SAS     80
****: AD 00             p2_1:   RNP     0
****: D7                        NOP
; Procedure 2 Attribute Table:
****: A4 00                     .word   164             ; data size
****: 08 00                     .word   8               ; param size
****: 07 00                     .word   $ - p2_1        ; exit ic
****: 2A 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: 01                p1_0:   SLDC    1
****: A6 02 32 32               LSA     2, "22"
****: 03                        SLDC    3
****: D7                        NOP
****: A6 02 34 34               LSA     2, "44"
****: CE 02                     CLP     2
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 16 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 1C 00                     .word   $ - p2_at       ; procedure 2
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
