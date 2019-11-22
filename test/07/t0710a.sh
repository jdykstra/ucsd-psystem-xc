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

TEST_SUBJECT="assemble 6502"
. test_prelude

cat > example.text << 'fubar'
; applestuff reconstruction
; Copyright (C) 1982, 2012 Peter Miller
        .MACRO  POP
        PLA
        .IF     "%1"="BIAS"
        PLA
        PLA             ;POP FUNCTION RETURN STACK BIAS
        PLA
        .ELSE
        STA     %1
        PLA             ;POP 16-BIT VALUE
        STA     %1+1
        .ENDC
        .ENDM

        .MACRO  HPOP
        PLA
        STA     %1      ;POP 8-BIT VALUE
        PLA
        .ENDM

        .MACRO  PUSH
        LDA     %1+1
        PHA             ;PUSH 16-BIT VALUE
        LDA     %1
        PHA
        .ENDM

        .FUNC   KEYPRESS
;
; FUNCTION KEYPRESS:BOOLEAN
;
RETURN  .EQU    0
VERSION .EQU    0BF21
WPTR    .EQU    0BF18
RPTR    .EQU    0BF19
CHECK10 .EQU    0FF5C
CHECK11 .EQU    0BF0A

        POP     RETURN
        POP     BIAS

        LDA     #0      ;PUSH M.S.BYTE FUNCTION RETURN
        PHA

        LDA     VERSION
        BNE     $11
        JSR     CHECK10
        JMP     $10

$11     JSR     CHECK11

$10     LDA     WPTR    ;SEE IF ANYTHING IN INPUT BUFFER
        CMP     RPTR
        BEQ     FALSE   ;BRANCH IF EMPTY
        LDA     #1
        BNE     RET     ;ALWAYS TAKEN

FALSE   LDA     #0

RET     PHA             ;PUSH L.S.BYTE FUNCTION RETURN

        PUSH    RETURN  ;RETURN TO CALLER
        RTS

        .END
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    68  KEYPRESS  SEPRTSEG   1 6502      3.0
;              KEYPRESS  SEPFUNC          1         0
;              KEYPRESS  GLOBDEF          1     $0000
;                        EOFMARK          1
;
; Procedure 1:                                          ; native 6502 code
                                .arch   "6502"
                                .radix  10.
****: 68                p1_0:   PLA
****: 85 00                     STA     0
****: 68                        PLA
****: 85 01                     STA     1
****: 68                        PLA
****: 68                        PLA
****: 68                        PLA
****: 68                        PLA
****: A9 00                     LDA     #0
****: 48                        PHA
****: AD 21 BF                  LDA     48929
****: D0 06                     BNE     p1_1
****: 20 5C FF                  JSR     65372
****: 4C 1B 00                  JMP     27
****: 20 0A BF          p1_1:   JSR     48906
****: AD 18 BF                  LDA     48920
****: CD 19 BF                  CMP     48921
****: F0 04                     BEQ     p1_2
****: A9 01                     LDA     #1
****: D0 02                     BNE     p1_3
****: A9 00             p1_2:   LDA     #0
****: 48                p1_3:   PHA
****: A5 01                     LDA     1
****: 48                        PHA
****: A5 00                     LDA     0
****: 48                        PHA
****: 60                        RTS
****: 00                        BRK
; Procedure 1 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 1E 00                     .word   30
****: 01 00                     .word   1               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 1 Attribute Table:
****: 3C 00                     .word   $ - p1_0        ; enter ic
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

ucsdpsys_assemble --arch=6502 example.text
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
