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

TEST_SUBJECT='(*$Copyright*)'
. test_prelude

cat > example.text << 'fubar'
(*$Copyright (C) 1812 Tchaikovsky. *)
program example;
begin
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "example.code"
; Copyright (C) 1812 Tchaikovsky.
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    16  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
0000: C1 00             p1_0:   RBP     0
; Procedure 1 Attribute Table:
0002: 00 00                     .word   0               ; data size
0004: 04 00                     .word   4               ; param size
0006: 06 00                     .word   $ - p1_0        ; exit ic
0008: 08 00                     .word   $ - p1_0        ; enter ic
000A: 01                p1_at:  .byte   1               ; procedure number
000B: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
000C: 02 00                     .word   $ - p1_at       ; procedure 1
000E: 01                        .byte   1               ; segment number
000F: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || fail

ucsdpsys_disassemble example.code -o test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
