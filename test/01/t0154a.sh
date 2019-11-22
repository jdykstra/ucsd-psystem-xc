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

TEST_SUBJECT="segment zero"
. test_prelude

#
# insert your test here...
#
cat > test.text << 'fubar'
{$S+}
{$U-}
program test;
begin
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    16  TEST      LINKED     0 Undefined Pre-II.1
;
; Procedure 1:
0000: D6                p1_0:   XIT
0001: D7                        NOP
; Procedure 1 Attribute Table:
0002: 00 00                     .word   0               ; data size
0004: 00 00                     .word   0               ; param size
0006: 06 00                     .word   $ - p1_0        ; exit ic
0008: 08 00                     .word   $ - p1_0        ; enter ic
000A: 01                p1_at:  .byte   1               ; procedure number
000B: FF                        .byte   -1              ; lex level

;
; Procedure dictionary:
;
000C: 02 00                     .word   $ - p1_at       ; procedure 1
000E: 00                        .byte   0               ; segment number
000F: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

$bindir/ucsdpsys_compile test.text
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble test.code > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
