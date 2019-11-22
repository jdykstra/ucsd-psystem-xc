#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

TEST_SUBJECT="function returns"
. test_prelude

#
# Functions returns
#
cat > test.in << 'fubar'
program test;

    function factorial(x: integer): integer;
    begin
        if x <= 1
        then
            factorial := 1
        else
            factorial := x * factorial(x - 1)
    end;

begin
    writeln(factorial(6))
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    74  TEST      LINKED     1 P-Code LE 3.0
;
; Procedure 2:
****: DA                p2_0:   SLDL    3
****: 01                        SLDC    1
****: C8                        LEQI
****: A1 05                     FJP     p2_3
****: 01                        SLDC    1
****: CC 01                     STL     1
****: B9 0B                     UJP     p2_1
****: DA                p2_3:   SLDL    3
****: DA                        SLDL    3
****: 01                        SLDC    1
****: 95                        SBI
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CF 02                     CGP     2
****: 8F                        MPI
****: CC 01                     STL     1
****: AD 01             p2_1:   RNP     1
****: D7                        NOP
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 06 00                     .word   6               ; param size
****: 07 00                     .word   $ - p2_1        ; exit ic
****: 1E 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: B6 01 03          p1_0:   LOD     1, 3
****: 06                        SLDC    6
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CE 02                     CLP     2
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 1E 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 24 00                     .word   $ - p2_at       ; procedure 2
****: 04 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

$bindir/ucsdpsys_compile -o test.code test.in
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble -a -o test.out test.code
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
