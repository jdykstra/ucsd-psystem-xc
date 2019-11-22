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

TEST_SUBJECT="builtin pred function"
. test_prelude

#
# builtin pred function
#
cat > test.text << 'fubar'
program test;
var
    b: boolean;
    c: char;
    e: (zero, one, two, three);
    i: integer;
begin
    b := pred(b, i);
    b := pred(false);
    c := pred(c, i);
    c := pred('a');
    e := pred(e, i);
    e := pred(three);
    i := pred(i, 2);
    i := pred(42)
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    50  TEST      LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: EA                p1_0:   SLDO    3
****: ED                        SLDO    6
****: 95                        SBI
****: AB 03                     SRO     3
****: 01                        SLDC    1
****: 91                        NGI
****: AB 03                     SRO     3
****: EB                        SLDO    4
****: ED                        SLDO    6
****: 95                        SBI
****: AB 04                     SRO     4
****: 60                        SLDC    96              ; '`'
****: AB 04                     SRO     4
****: EC                        SLDO    5
****: ED                        SLDO    6
****: 95                        SBI
****: AB 05                     SRO     5
****: 02                        SLDC    2
****: AB 05                     SRO     5
****: ED                        SLDO    6
****: 02                        SLDC    2
****: 95                        SBI
****: AB 06                     SRO     6
****: 29                        SLDC    41              ; ')'
****: AB 06                     SRO     6
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 08 00                     .word   8               ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 2A 00                     .word   $ - p1_0        ; enter ic
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
