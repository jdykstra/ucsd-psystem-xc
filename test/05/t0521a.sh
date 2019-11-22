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

TEST_SUBJECT="array bug"
. test_prelude

cat > example.text << 'fubar'
program example;
type
    COMMANDS =
        (ILLEGAL, ADJUSTC, COPYC, DELETEC, FINDC, INSERTC, JUMPC, LISTC,
        MACRODEFC, PARAC, QUITC, REPLACEC, SETC, VERIFYC, XECUTEC, ZAPC,
        REVERSEC, FORWARDC, UP, DOWN, LEFT, RIGHT, TAB, DIGIT, DUMPC, ADVANCE,
        SPACE, EQUALC, SLASHC);
var
    TRANSLATE: ARRAY [CHAR] OF COMMANDS;

    PROCEDURE MAP(CH: CHAR; C: COMMANDS);
    BEGIN
        TRANSLATE[CH] := C;
        IF CH IN ['A'..'Z'] THEN
            TRANSLATE[CHR(32 + ORD(CH))] := C;
    END;

begin
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    76  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 2:
****: A5 03             p2_0:   LAO     3
****: D9                        SLDL    2
****: 00                        SLDC    0
****: C7 FF 00                  LDCI    255
****: 88                        CHK
****: A4 01                     IXA     1
****: D8                        SLDL    1
****: 9A                        STO
****: D9                        SLDL    2
****: D7                        NOP
****: B3 06 FF 07 FE            LDC     6, 2047, 65534, 0, 0, 0, 0
      FF 00 00 00 00
      00 00 00 00
****: 06                        SLDC    6
****: 8B                        INN
****: A1 0E                     FJP     p2_1
****: A5 03                     LAO     3
****: 20                        SLDC    32              ; ' '
****: D9                        SLDL    2
****: 82                        ADI
****: 00                        SLDC    0
****: C7 FF 00                  LDCI    255
****: 88                        CHK
****: A4 01                     IXA     1
****: D8                        SLDL    1
****: 9A                        STO
****: AD 00             p2_1:   RNP     0
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p2_1        ; exit ic
****: 36 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: C1 00             p1_0:   RBP     0
; Procedure 1 Attribute Table:
****: 00 02                     .word   512             ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_0        ; exit ic
****: 08 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 0E 00                     .word   $ - p2_at       ; procedure 2
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
