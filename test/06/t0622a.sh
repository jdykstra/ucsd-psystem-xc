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

TEST_SUBJECT="compile ternary"
. test_prelude

cat > example.text << 'fubar'
program example;
var
  a: integer;
  b: boolean;
  c: integer;
  d: integer;
begin
    a := b ? c : d;
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    26  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
0000: EB                p1_0:   SLDO    4
0001: A1 03                     FJP     p1_3
0003: EC                        SLDO    5
0004: B9 01                     UJP     p1_4
0006: ED                p1_3:   SLDO    6
0007: AB 03             p1_4:   SRO     3
0009: C1 00             p1_1:   RBP     0
000B: D7                        NOP
; Procedure 1 Attribute Table:
000C: 08 00                     .word   8               ; data size
000E: 04 00                     .word   4               ; param size
0010: 07 00                     .word   $ - p1_1        ; exit ic
0012: 12 00                     .word   $ - p1_0        ; enter ic
0014: 01                p1_at:  .byte   1               ; procedure number
0015: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
0016: 02 00                     .word   $ - p1_at       ; procedure 1
0018: 01                        .byte   1               ; segment number
0019: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text > LOG 2>&1
if test $? -ne 0
then
    cat LOG
    fail
fi

ucsdpsys_disassemble example.code -o test.out
test $? -eq 0 || no_result

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
