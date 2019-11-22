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

TEST_SUBJECT="assembler .ref"
. test_prelude

cat > example.text << 'fubar'
        .arch   "6502"
        .proc   example
        .ref    fred
        jsr     fred + 46
        jsr     fred
        rts
        .proc   example2
        .ref    fred
        jsr     fred
        rts
        .end
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    48  EXAMPLE   SEPRTSEG   1 6502      3.0
;              EXAMPLE   SEPPROC          1         0
;              EXAMPLE   GLOBDEF          1     $0000
;              FRED      GLOBREF          1
;      WORD reference (2 times) $0001, $0004
;              EXAMPLE2  SEPPROC          2         0
;              EXAMPLE2  GLOBDEF          2     $0000
;              FRED      GLOBREF          1
;      WORD reference (once) $0019
;                        EOFMARK          1
;
; Procedure 1:                                          ; native 6502 code
                                .arch   "6502"
                                .radix  10.
****: 20 46 00          p1_0:   JSR     70
****: 20 00 00                  JSR     0
****: 60                        RTS
****: 00                        BRK
; Procedure 1 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 0B 00                     .word   11
****: 0A 00                     .word   10
****: 02 00                     .word   2               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 1 Attribute Table:
****: 14 00                     .word   $ - p1_0        ; enter ic
****: 00                p1_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level
;
; Procedure 2:                                          ; native 6502 code
****: 20 00 00          p2_0:   JSR     0
****: 60                        RTS
; Procedure 2 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 07 00                     .word   7
****: 01 00                     .word   1               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 2 Attribute Table:
****: 0E 00                     .word   $ - p2_0        ; enter ic
****: 00                p2_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 02 00                     .word   $ - p2_at       ; procedure 2
****: 16 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_assemble example.text
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
