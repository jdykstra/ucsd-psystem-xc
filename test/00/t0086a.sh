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

TEST_SUBJECT="compile enum"
. test_prelude

#
# compile enum
#
cat > test.in << 'fubar'
program test_enum;
type
    enum_t = (one, two, three, four, five, six, seven, eight);
var
    x, y: enum_t;
    b: boolean;

    procedure p(var arg: enum_t);
    begin
        arg := six
    end;

    function f(arg: enum_t): enum_t;
    begin
        f := arg
    end;

begin
    x := one;
    y := x;
    b := (x = y);
    b := (x <> y);
    b := (x > y);
    b := (x >= y);
    b := (x < y);
    b := (x <= y);
    p(x);
    y := f(x)
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   100  TESTENUM  LINKED     1 P-Code LE 3.0
;
; Procedure 2:
****: D8                p2_0:   SLDL    1
****: 05                        SLDC    5
****: 9A                        STO
****: AD 00             p2_1:   RNP     0
****: D7                        NOP
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 02 00                     .word   2               ; param size
****: 07 00                     .word   $ - p2_1        ; exit ic
****: 0C 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 3:
****: DA                p3_0:   SLDL    3
****: CC 01                     STL     1
****: AD 01             p3_1:   RNP     1
****: D7                        NOP
; Procedure 3 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 06 00                     .word   6               ; param size
****: 07 00                     .word   $ - p3_1        ; exit ic
****: 0C 00                     .word   $ - p3_0        ; enter ic
****: 03                p3_at:  .byte   3               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: 00                p1_0:   SLDC    0
****: AB 04                     SRO     4
****: EB                        SLDO    4
****: AB 03                     SRO     3
****: EB                        SLDO    4
****: EA                        SLDO    3
****: C3                        EQUI
****: AB 05                     SRO     5
****: EB                        SLDO    4
****: EA                        SLDO    3
****: CB                        NEQI
****: AB 05                     SRO     5
****: EB                        SLDO    4
****: EA                        SLDO    3
****: C5                        GTRI
****: AB 05                     SRO     5
****: EB                        SLDO    4
****: EA                        SLDO    3
****: C4                        GEQI
****: AB 05                     SRO     5
****: EB                        SLDO    4
****: EA                        SLDO    3
****: C9                        LESI
****: AB 05                     SRO     5
****: EB                        SLDO    4
****: EA                        SLDO    3
****: C8                        LEQI
****: AB 05                     SRO     5
****: A5 04                     LAO     4
****: CE 02                     CLP     2
****: EB                        SLDO    4
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CE 03                     CLP     3
****: AB 03                     SRO     3
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 06 00                     .word   6               ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 38 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 3E 00                     .word   $ - p3_at       ; procedure 3
****: 50 00                     .word   $ - p2_at       ; procedure 2
****: 06 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 03                        .byte   3               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

$bindir/ucsdpsys_compile test.in -o test.code
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble -a test.code > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
