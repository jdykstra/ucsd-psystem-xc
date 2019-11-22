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

TEST_SUBJECT="posns"
. test_prelude

cat > example.text << 'fubar'
program example;
var a, b, c, d: ^^integer;
begin
    a^^ := 1;
    b^^ := 2;
    c^^ := 3;
    d^^ := 4
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; example.text: 2:         5    1  variable    a         ^^integer
; example.text: 2:         4    1  variable    b         ^^integer
; example.text: 2:         3    1  variable    c         ^^integer
; example.text: 2:         2    1  variable    d         ^^integer
; example.text: 1:         0    1  parameter   bogus:out text
; example.text: 1:         1    1  parameter   bogus:in  text
; example.text: 1:                 program     example   program
; Procedure 1:
; example.text: 1:      program example;
; example.text: 2:      var a, b, c, d: ^^integer;
; example.text: 3:      begin
; example.text: 4:          a^^ := 1;
0000: ED                p1_0:   SLDO    6
0001: F8                        SIND    0
0002: 01                        SLDC    1
0003: 9A                        STO
; example.text: 5:          b^^ := 2;
0004: EC                        SLDO    5
0005: F8                        SIND    0
0006: 02                        SLDC    2
0007: 9A                        STO
; example.text: 6:          c^^ := 3;
0008: EB                        SLDO    4
0009: F8                        SIND    0
000A: 03                        SLDC    3
000B: 9A                        STO
; example.text: 7:          d^^ := 4
000C: EA                        SLDO    3
000D: F8                        SIND    0
000E: 04                        SLDC    4
000F: 9A                        STO
; example.text: 8:      end.
0010: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
0012: 08 00                     .word   8               ; data size
0014: 04 00                     .word   4               ; param size
0016: 06 00                     .word   $ - p1_1        ; exit ic
0018: 18 00                     .word   $ - p1_0        ; enter ic
001A: 01                p1_at:  .byte   1               ; procedure number
001B: 00                        .byte   0               ; lex level
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text --listing=test.out
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
