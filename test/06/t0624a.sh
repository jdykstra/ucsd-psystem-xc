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

TEST_SUBJECT="long integers, trunc"
. test_prelude

cat > example.text << 'fubar'
program example;
var
  x: integer;
  y: integer[8];
begin
  x := trunc(y);
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    34  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: 1E                p1_0:   SLDC    30
****: 9E 15                     CSP     LOADSEGMENT
****: A5 04                     LAO     4
****: BC 03                     LDM     3
****: 03                        SLDC    3
****: 14                        SLDC    20
****: CD 1E 04                  CXP     30, 4           ; decops trunc
****: AB 03                     SRO     3
****: 1E                p1_1:   SLDC    30
****: 9E 16                     CSP     UNLOADSEGMENT
****: C1 00                     RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 08 00                     .word   8               ; data size
****: 04 00                     .word   4               ; param size
****: 0A 00                     .word   $ - p1_1        ; exit ic
****: 1A 00                     .word   $ - p1_0        ; enter ic
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

ucsdpsys_compile example.text
test $? -eq 0 || fail

ucsdpsys_disassemble -a example.code -o test.out
test $? -eq 0 || no_result

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
