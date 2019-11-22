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

TEST_SUBJECT="BLOCKWRITE"
. test_prelude

cat > example.text << 'fubar'
program example;
var
    x: integer;
    y: integer;
    buffer: packed array[0..511] of char;
    f: file;
begin
    x := blockwrite(f, buffer[y], 42, 69)
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    56  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: A5 81 05          p1_0:   LAO     261
****: 9F                        LDCN
****: 01                        SLDC    1
****: 91                        NGI
****: CD 00 03                  CXP     0, 3            ; finit
****: A5 81 05                  LAO     261
****: A5 05                     LAO     5
****: EB                        SLDO    4
****: 00                        SLDC    0
****: C7 FF 01                  LDCI    511
****: 88                        CHK
****: 2A                        SLDC    42              ; '*'
****: 45                        SLDC    69              ; 'E'
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CD 00 1C                  CXP     0, 28           ; fblockio
****: 9E 00                     CSP     IOCHECK
****: AB 03                     SRO     3
****: A5 81 05          p1_1:   LAO     261
****: 00                        SLDC    0
****: CD 00 06                  CXP     0, 6            ; fclose
****: C1 00                     RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 54 02                     .word   596             ; data size
****: 04 00                     .word   4               ; param size
****: 0E 00                     .word   $ - p1_1        ; exit ic
****: 30 00                     .word   $ - p1_0        ; enter ic
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
