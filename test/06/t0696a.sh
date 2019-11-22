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

TEST_SUBJECT="link intrinsic units"
. test_prelude

cat > example.text << 'fubar'
(*$S+*)
unit example;
  intrinsic code 7;
interface

  procedure nuts;

implementation

  procedure nuts;
  external;

begin
end.
fubar
test $? -eq 0 || no_result

cat > nuts.asm.text << 'fubar'
    .arch   "6502"
    .proc   nuts
    rts
    .end
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "final.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     2    32  EXAMPLE   LINKED_    7 6502      3.0
;                        INTRINS
;        procedure nuts;
;
; Procedure 1:
0000: AD 00             p1_0:   RNP     0
; Procedure 1 Attribute Table:
0002: 00 00                     .word   0               ; data size
0004: 00 00                     .word   0               ; param size
0006: 06 00                     .word   $ - p1_0        ; exit ic
0008: 08 00                     .word   $ - p1_0        ; enter ic
000A: 01                p1_at:  .byte   1               ; procedure number
000B: 01                        .byte   1               ; lex level
;
; Procedure 2:                                          ; native 6502 code
                                .arch   "6502"
                                .radix  10.
000C: 60                p2_0:   RTS
000D: 00                        BRK
; Procedure 2 Relocation Data:
000E: 00 00                     .word   0               ; .interp relative
0010: 00 00                     .word   0               ; pc relative
0012: 00 00                     .word   0               ; .ref relative
0014: 00 00                     .word   0               ; .public relative
; Procedure 2 Attribute Table:
0016: 0A 00                     .word   $ - p2_0        ; enter ic
0018: 00                p2_at:  .byte   0               ; native machine code
0019: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
001A: 02 00                     .word   $ - p2_at       ; procedure 2
001C: 12 00                     .word   $ - p1_at       ; procedure 1
001E: 07                        .byte   7               ; segment number
001F: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || fail

ucsdpsys_assemble nuts.asm.text
test $? -eq 0 || fail

ucsdpsys_link -o final.code example.code nuts.asm.code
test $? -eq 0 || fail

ucsdpsys_disassemble final.code -o test.out
test $? -eq 0 || no_result

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
