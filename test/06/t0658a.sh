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

TEST_SUBJECT="assemble .list"
. test_prelude

cat > example.text << 'fubar'
.list
.arch "6502"
.proc example
nop
nop
nop
nop
nop
nop
nop
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
;     1    24  EXAMPLE   SEPRTSEG   1 6502      3.0
;              EXAMPLE   SEPPROC          1         0
;              EXAMPLE   GLOBDEF          1     $0000
;                        EOFMARK          1
;
; Procedure 1:                                          ; native 6502 code
                                .arch   "6502"
                                .radix  10.
****: EA                p1_0:   NOP
****: EA                        NOP
****: EA                        NOP
****: EA                        NOP
****: EA                        NOP
****: EA                        NOP
****: EA                        NOP
****: 60                        RTS
; Procedure 1 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 1 Attribute Table:
****: 10 00                     .word   $ - p1_0        ; enter ic
****: 00                p1_at:  .byte   0               ; native machine code
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
