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

TEST_SUBJECT="packed array index and store"
. test_prelude

#
# packed array index and load
#
cat > test.text << 'fubar'
program test;
type
    sub = 0..31;
    arr = packed array [0..27] of sub;
var
    x: integer;
    y: arr;
begin
    y[x] := x
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    30  TEST      LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: A5 04             p1_0:   LAO     4
****: EA                        SLDO    3
****: 00                        SLDC    0
****: 1B                        SLDC    27
****: 88                        CHK
****: C0 03 05                  IXP     3, 5
****: EA                        SLDO    3
****: 00                        SLDC    0
****: 1F                        SLDC    31
****: 88                        CHK
****: BB                        STP
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 16 00                     .word   22              ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 16 00                     .word   $ - p1_0        ; enter ic
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

ucsdpsys_compile test.text
test $? -eq 0 || no_result

ucsdpsys_disassemble -a test.code > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || no_result

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
