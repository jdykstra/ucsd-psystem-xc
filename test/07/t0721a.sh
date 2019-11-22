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

TEST_SUBJECT="assemble, .proc forgets"
. test_prelude

cat > example.text << 'fubar'
; applestuff reconstruction
; Copyright (C) 1982, 2010, 2012 Peter Miller
;
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

        .FUNC   PADDLE,1
;
; FUNCTION PADDLE(SELECT:INTEGER):INTEGER
;
RETURN  .EQU    0
TRIGGER .EQU    0C070
PORT    .EQU    0C064

        POP     RETURN
        POP     BIAS

        PLA             ;POP SELECT
        AND     #3      ;MOD 4
        TAX
        PLA             ;DISCARD M.S.BYTE

        LDA     TRIGGER ;INITIALIZE CAPACITOR CHARGE
        LDY     #0      ;INITIALIZE COUNT
        NOP
        NOP
LOOP    LDA     PORT,X  ;CAPACITOR CHARGED ?
        BPL     DONE    ;BRANCH IF CHARGED
        INY             ;INCRIMENT COUNT
        BNE     LOOP    ;BRANCH IF NO OVERFLOW
        DEY             ;Y=255

DONE    LDA     #0      ;PUSH M.S.BYTE FUNCTION RETURN
        PHA
        TYA             ;PUSH L.S.BYTE FUNCTION RETURN
        PHA

        PUSH    RETURN  ;RETURN TO CALLER
        RTS

        .FUNC   BUTTON,1
;
; FUNCTION BUTTON(SELECT:INTEGER):BOOLEAN
;
RETURN  .EQU    0
BUTTONS .EQU    0C060

        POP     RETURN
        POP     BIAS

        PLA             ;POP SELECT
        CLC
        ADC     #1      ;+1 MOD 4
        AND     #3
        TAX
        PLA             ;DISCARD M.S.BYTE

        LDA     #0      ;PUSH M.S.BYTE FUNCTION RETURN
        PHA

        LDA     BUTTONS,X ;FETCH DATA
        BPL     FALSE
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
;     1   114  PADDLE    SEPRTSEG   1 6502      3.0
;              PADDLE    SEPFUNC          1         1
;              PADDLE    GLOBDEF          1     $0000
;              BUTTON    SEPFUNC          2         1
;              BUTTON    GLOBDEF          2     $0000
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
****: 68                        PLA
****: 29 03                     AND     #3
****: AA                        TAX
****: 68                        PLA
****: AD 70 C0                  LDA     49264
****: A0 00                     LDY     #0
****: EA                        NOP
****: EA                        NOP
****: BD 64 C0          p1_2:   LDA     49252, X
****: 10 04                     BPL     p1_1
****: C8                        INY
****: D0 F8                     BNE     p1_2
****: 88                        DEY
****: A9 00             p1_1:   LDA     #0
****: 48                        PHA
****: 98                        TYA
****: 48                        PHA
****: A5 01                     LDA     1
****: 48                        PHA
****: A5 00                     LDA     0
****: 48                        PHA
****: 60                        RTS
****: 00                        BRK
; Procedure 1 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 1 Attribute Table:
****: 34 00                     .word   $ - p1_0        ; enter ic
****: 00                p1_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level
;
; Procedure 2:                                          ; native 6502 code
****: 68                p2_0:   PLA
****: 85 00                     STA     0
****: 68                        PLA
****: 85 01                     STA     1
****: 68                        PLA
****: 68                        PLA
****: 68                        PLA
****: 68                        PLA
****: 68                        PLA
****: 18                        CLC
****: 69 01                     ADC     #1
****: 29 03                     AND     #3
****: AA                        TAX
****: 68                        PLA
****: A9 00                     LDA     #0
****: 48                        PHA
****: BD 60 C0                  LDA     49248, X
****: 10 04                     BPL     p2_1
****: A9 01                     LDA     #1
****: D0 02                     BNE     p2_2
****: A9 00             p2_1:   LDA     #0
****: 48                p2_2:   PHA
****: A5 01                     LDA     1
****: 48                        PHA
****: A5 00                     LDA     0
****: 48                        PHA
****: 60                        RTS
; Procedure 2 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 2 Attribute Table:
****: 30 00                     .word   $ - p2_0        ; enter ic
****: 00                p2_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 02 00                     .word   $ - p2_at       ; procedure 2
****: 38 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_assemble --arch=6502 example.text
test $? -eq 0 || fail

ucsdpsys_disassemble -a example.code -o test.out
test $? -eq 0 || no_result

diff -u ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
