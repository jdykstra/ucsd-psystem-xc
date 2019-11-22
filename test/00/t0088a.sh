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

TEST_SUBJECT="compile subranges"
. test_prelude

#
# test subranges
#
cat > test.in << 'fubar'
program test_subrange;
type
    enum_t = ( one, two, three, four, five);
    sub1 = two..four;
    sub2 = 0..127;
var
    v1: sub1;
    v2: sub2;
    c3: enum_t;
    c4: integer;
begin
    v1 := three;
    v2 := 64;
    c3 := v1;
    c4 := v2;
    v1 := c3;
    v2 := c4
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    40  TESTSUBR  LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: 02                p1_0:   SLDC    2
****: AB 03                     SRO     3
****: 40                        SLDC    64              ; '@'
****: AB 04                     SRO     4
****: EA                        SLDO    3
****: AB 05                     SRO     5
****: EB                        SLDO    4
****: AB 06                     SRO     6
****: EC                        SLDO    5
****: 01                        SLDC    1
****: 03                        SLDC    3
****: 88                        CHK
****: AB 03                     SRO     3
****: ED                        SLDO    6
****: 00                        SLDC    0
****: 7F                        SLDC    127
****: 88                        CHK
****: AB 04                     SRO     4
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 08 00                     .word   8               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 20 00                     .word   $ - p1_0        ; enter ic
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
