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

TEST_SUBJECT="builtin succ function"
. test_prelude

#
# builtin succ function
#
cat > test.text << 'fubar'
program test;
var
    b: boolean;
    c: char;
    e: (zero, one, two, three);
    i: integer;
begin
    b := succ(b, i);
    b := succ(false);
    c := succ(c, i);
    c := succ('a');
    e := succ(e, i);
    e := succ(three);
    i := succ(i, 2);
    i := succ(42)
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    48  TEST      LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: EA                p1_0:   SLDO    3
****: ED                        SLDO    6
****: 82                        ADI
****: AB 03                     SRO     3
****: 01                        SLDC    1
****: AB 03                     SRO     3
****: EB                        SLDO    4
****: ED                        SLDO    6
****: 82                        ADI
****: AB 04                     SRO     4
****: 62                        SLDC    98              ; 'b'
****: AB 04                     SRO     4
****: EC                        SLDO    5
****: ED                        SLDO    6
****: 82                        ADI
****: AB 05                     SRO     5
****: 04                        SLDC    4
****: AB 05                     SRO     5
****: ED                        SLDO    6
****: 02                        SLDC    2
****: 82                        ADI
****: AB 06                     SRO     6
****: 2B                        SLDC    43              ; '+'
****: AB 06                     SRO     6
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 08 00                     .word   8               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 28 00                     .word   $ - p1_0        ; enter ic
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

$bindir/ucsdpsys_compile test.text
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
