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

TEST_SUBJECT="assemble, 6502"
. test_prelude

cat > example.text << 'fubar'
; applestuff reconstruction
; Copyright (C) 1982, 2012 Peter Miller
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

        .PROC   TTLOUT,2
;
; PROCEDURE TTLOUT(SELECT:INTEGER;DATA:BOOLEAN)
;
RETURN  .EQU    0
PORTS   .EQU    0C058

        POP     RETURN

        PLA             ;POP DATA
        LSR     A       ;CARRY BIT CONTAINS DATA
        PLA             ;DISCARD M.S.BYTE DATA

        PLA             ;POP SELECT
        AND     #3      ;MOD 4
        ROL     A       ;CALC PORT INDEX
        TAY
        LDA     PORTS,Y ;SET DATA INTO PORT
        PLA             ;DISCARD M.S.BYTE SELECT

        PUSH    RETURN  ;RETURN TO CALLER
        RTS

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

        .FUNC   RANDOM
;
; FUNCTION RANDOM:INTEGER
;
; N.B. ALWAYS RETURNS A POSITIVE INTEGER (0..32767)
;      USES A P.R.B.S. ALGORITHM
;
        .DEF    SHIFTREG
RETURN  .EQU    0

        POP     RETURN
        POP     BIAS

        LDY     #7      ;SHIFT ON 7 POSITIONS
LOOP:
        ASL     SHIFTREG ;BIT0 = 0
        PHP             ;SAVE CARRY BIT
        ROL     SHIFTREG+1
        ROL     SHIFTREG+2
        ROL     SHIFTREG+3
        BMI     $1

        PLP
        BPL     FALSE
        BMI     TRUE    ;ALWAYS TAKEN

$1      PLP
        BMI     FALSE

TRUE    INC     SHIFTREG ;SET BIT0

FALSE   DEY             ;LOOPED 7 TIMES ?
        BNE     LOOP    ;BRANCH IF NOT

        LDA     SHIFTREG ;PUSH M.S.BYTE FUNCTION RETURN
        LSR     A       ;ALWAYS POSITIVE
        PHA
        LDA     SHIFTREG+2 ;PUSH L.S.BYTE FUNCTION RETURN
        PHA

        PUSH    RETURN  ;RETURN TO CALLER
        RTS

SHIFTREG:
        .BYTE   5A,0B2,0F6,93

        .PROC   RANDOMIZE
;
; PROCEDURE RANDOMIZE
;
        .REF    SHIFTREG
SEED    .EQU    0BF13

        LDA     SEED    ;SEED THE SHIFT REGISTER
        STA     SHIFTREG
        STA     SHIFTREG+1
        LDA     SEED+1
        STA     SHIFTREG+2
        STA     SHIFTREG+3

        ORA     SHIFTREG ;TEST IF SEED=0
        BNE     OK      ;BRANCH IF <>0

        LDA     #55     ;COMPENSATE IF SEED=0
        STA     SHIFTREG+3

OK      RTS

        .PROC   NOTE,2
;
; PROCEDURE NOTE(PITCH,DURATION:INTEGER)
;
; PITCH=0 IS A REST
;
RETURN  .EQU    0
TEMP    .EQU    2
PHASE   .EQU    3
LENGTH  .EQU    5
SPEAKER .EQU    0C030

        POP     RETURN
        HPOP    DURAT
        HPOP    PITCH
;       HPOP    VOICE
        PUSH    RETURN
        JMP     DEMUX

PLAY:
;
; PLAY A NOTE
;
        LDY     PHASE+1
        LDA     SPEAKER ;TOGGLE THE SPEAKER
$1      INC     LENGTH
        BNE     $2
        INC     LENGTH+1
        BNE     $3
        RTS             ;NOTE FINISHED

$2      NOP             ;TIME COMPENSATION
        JMP     $3
$3      DEY             ;THIS PHASE DONE ?
        BEQ     $5      ;BRANCH IF DONE
        JMP     $4      ;TIME COMPENSATION
$4      BNE     $1      ;ALWAYS TAKEN

$5      LDY     PHASE
        LDA     SPEAKER ;TOGGLE SPEAKER
$6      INC     LENGTH
        BNE     $7
        INC     LENGTH+1
        RTS             ;NOTE FINISHED

$7      NOP             ;TIME COMPENSATION
        JMP     $8
$8      DEY             ;THIS PHASE DONE ?
        BEQ     PLAY    ;BRANCH IF DONE
        JMP     $9      ;TIME COMPENSATION
$9      BNE     $6      ;ALWAYS TAKEN

DEMUX:
;
; EXTRACT A NOTE FROM THE PHASE TABLE
;
        LDA     PITCH   ;CALC INDEX INTI NOTES TABLE
        ASL     A
        TAY
        LDA     NOTES,Y
        STA     TEMP

        LDA     VOICE   ;CALC VOICE BIAS
$1      LSR     A
        BEQ     $2
        LSR     TEMP
        BNE     $1

$2      LDA     NOTES,Y ;DO VOICE BIAS
        SEC
        SBC     TEMP
        STA     PHASE+1
        INY
        LDA     NOTES,Y
        ADC     TEMP
        STA     PHASE

        LDA     #0      ;2'S COMPLEMENT LENGTH
        SEC
        SBC     DURAT
        STA     LENGTH+1
        LDA     #0
        STA     LENGTH

        LDA     PHASE+1
        BNE     PLAY    ;PLAY A NOT IF NOT A REST

REST:
;
; DELAY AS FOR A REST
;
        NOP             ;TIME COMPENSATION
        NOP
        JMP     $1
$1      INC     LENGTH
        BNE     $2
        INC     LENGTH+1
        RTS             ;REST FINISHED

$2      NOP             ;TIME COMPENSATION
        JMP     $3
$3      BNE     REST    ;ALWAYS TAKEN

PITCH   .BYTE   0
DURAT   .BYTE   0
VOICE   .BYTE   0FF     ;8 VOICES: 1,3,7,0F,1F,3F,7F,0FF

NOTES:
;
; PHASE TABLE FOR NOTES
;
        .BYTE   0,0     ;REST
        .BYTE   0F6,0F6
        .BYTE   0E8,0E8
        .BYTE   0DB,0DB
        .BYTE   0CF,0CF
        .BYTE   0C3,0C3
        .BYTE   0B8,0B8
        .BYTE   0AE,0AE
        .BYTE   0A4,0A4
        .BYTE   9B,9B
        .BYTE   92,92
        .BYTE   8A,8A
        .BYTE   82,82
        .BYTE   7B,7B
        .BYTE   74,74
        .BYTE   6D,6E
        .BYTE   67,68
        .BYTE   61,62
        .BYTE   5C,5C
        .BYTE   57,57
        .BYTE   52,52
        .BYTE   4D,4E
        .BYTE   49,49
        .BYTE   45,45
        .BYTE   41,41
        .BYTE   3D,3E
        .BYTE   3A,3A
        .BYTE   36,37
        .BYTE   33,34
        .BYTE   30,31
        .BYTE   2E,2E
        .BYTE   2B,2C
        .BYTE   29,29
        .BYTE   26,27
        .BYTE   24,25
        .BYTE   22,23
        .BYTE   20,21
        .BYTE   1E,1F
        .BYTE   1D,1D
        .BYTE   1B,1C
        .BYTE   1A,1A
        .BYTE   18,19
        .BYTE   17,17
        .BYTE   15,16
        .BYTE   14,15
        .BYTE   13,14
        .BYTE   12,12
        .BYTE   11,11
        .BYTE   10,10
        .BYTE   0F,10
        .BYTE   0E,0F

        .END

fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   660  PADDLE    SEPRTSEG   1 6502      3.0
;              PADDLE    SEPFUNC          1         1
;              PADDLE    GLOBDEF          1     $0000
;              BUTTON    SEPFUNC          2         1
;              BUTTON    GLOBDEF          2     $0000
;              TTLOUT    SEPPROC          3         2
;              TTLOUT    GLOBDEF          3     $0000
;              KEYPRESS  SEPFUNC          4         0
;              KEYPRESS  GLOBDEF          4     $0000
;              RANDOM    SEPFUNC          5         0
;              RANDOM    GLOBDEF          5     $0000
;              SHIFTREG  GLOBDEF          5     $0039
;              RANDOMIZ  SEPPROC          6         0
;              RANDOMIZ  GLOBDEF          6     $0000
;              SHIFTREG  GLOBREF          1
;      WORD reference (6 times) $012E, $0131, $0137, $013A, $013D, $0144
;              NOTE      SEPPROC          7         2
;              NOTE      GLOBDEF          7     $0000
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
; Procedure 3:                                          ; native 6502 code
****: 68                p3_0:   PLA
****: 85 00                     STA     0
****: 68                        PLA
****: 85 01                     STA     1
****: 68                        PLA
****: 4A                        LSR     A
****: 68                        PLA
****: 68                        PLA
****: 29 03                     AND     #3
****: 2A                        ROL     A
****: A8                        TAY
****: B9 58 C0                  LDA     49240, Y
****: 68                        PLA
****: A5 01                     LDA     1
****: 48                        PHA
****: A5 00                     LDA     0
****: 48                        PHA
****: 60                        RTS
****: 00                        BRK
; Procedure 3 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 3 Attribute Table:
****: 22 00                     .word   $ - p3_0        ; enter ic
****: 00                p3_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level
;
; Procedure 4:                                          ; native 6502 code
****: 68                p4_0:   PLA
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
****: D0 06                     BNE     p4_1
****: 20 5C FF                  JSR     65372
****: 4C 1B 00                  JMP     27
****: 20 0A BF          p4_1:   JSR     48906
****: AD 18 BF                  LDA     48920
****: CD 19 BF                  CMP     48921
****: F0 04                     BEQ     p4_2
****: A9 01                     LDA     #1
****: D0 02                     BNE     p4_3
****: A9 00             p4_2:   LDA     #0
****: 48                p4_3:   PHA
****: A5 01                     LDA     1
****: 48                        PHA
****: A5 00                     LDA     0
****: 48                        PHA
****: 60                        RTS
****: 00                        BRK
; Procedure 4 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 1E 00                     .word   30
****: 01 00                     .word   1               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 4 Attribute Table:
****: 3C 00                     .word   $ - p4_0        ; enter ic
****: 00                p4_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level
;
; Procedure 5:                                          ; native 6502 code
****: 68                p5_0:   PLA
****: 85 00                     STA     0
****: 68                        PLA
****: 85 01                     STA     1
****: 68                        PLA
****: 68                        PLA
****: 68                        PLA
****: 68                        PLA
****: A0 07                     LDY     #7
****: 0E 39 00          p5_4:   ASL     57
****: 08                        PHP
****: 2E 3A 00                  ROL     58
****: 2E 3B 00                  ROL     59
****: 2E 3C 00                  ROL     60
****: 30 05                     BMI     p5_1
****: 28                        PLP
****: 10 08                     BPL     p5_2
****: 30 03                     BMI     p5_3
****: 28                p5_1:   PLP
****: 30 03                     BMI     p5_2
****: EE 39 00          p5_3:   INC     57
****: 88                p5_2:   DEY
****: D0 E3                     BNE     p5_4
****: AD 39 00                  LDA     57
****: 4A                        LSR     A
****: 48                        PHA
****: AD 3B 00                  LDA     59
****: 48                        PHA
****: A5 01                     LDA     1
****: 48                        PHA
****: A5 00                     LDA     0
****: 48                        PHA
****: 60                        RTS
****: 5A                        PHY
****: B2 F6                     LDA     (246)
****: 93                        .byte   147
****: 00                        BRK
; Procedure 5 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 33 00                     .word   51
****: 31 00                     .word   49
****: 30 00                     .word   48
****: 2F 00             p5_5:   .word   47
****: 24 00                     .word   36
****: 20 00                     .word   32
****: 1D 00                     .word   29
****: 07 00                     .word   7               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 5 Attribute Table:
****: 54 00                     .word   $ - p5_0        ; enter ic
****: 00                p5_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level
;
; Procedure 6:                                          ; native 6502 code
****: AD 13 BF          p6_0:   LDA     48915
****: 8D 00 00                  STA     0
****: 8D 01 00                  STA     1
****: AD 14 BF                  LDA     48916
****: 8D 02 00                  STA     2
****: 8D 03 00                  STA     3
****: 0D 00 00                  ORA     0
****: D0 05                     BNE     p6_1
****: A9 55                     LDA     #85
****: 8D 03 00                  STA     3
****: 60                p6_1:   RTS
****: 00                        BRK
; Procedure 6 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 1E 00                     .word   30
****: 1D 00                     .word   29
****: 19 00                     .word   25
****: 18 00                     .word   24
****: 17 00                     .word   23
****: 12 00                     .word   18
****: 06 00                     .word   6               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 6 Attribute Table:
****: 32 00                     .word   $ - p6_0        ; enter ic
****: 00                p6_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level
;
; Procedure 7:                                          ; native 6502 code
****: 68                p7_0:   PLA
****: 85 00                     STA     0
****: 68                        PLA
****: 85 01                     STA     1
****: 68                        PLA
****: 8D 92 00                  STA     146
****: 68                        PLA
****: 68                        PLA
****: 8D 91 00                  STA     145
****: 68                        PLA
****: A5 01                     LDA     1
****: 48                        PHA
****: A5 00                     LDA     0
****: 48                        PHA
****: 4C 4B 00                  JMP     75
****: A4 04             p7_6:   LDY     4
****: AD 30 C0                  LDA     49200
****: E6 05             p7_4:   INC     5
****: D0 05                     BNE     p7_1
****: E6 06                     INC     6
****: D0 05                     BNE     p7_2
****: 60                        RTS
****: EA                p7_1:   NOP
****: 4C 2B 00                  JMP     43
****: 88                p7_2:   DEY
****: F0 05                     BEQ     p7_3
****: 4C 31 00                  JMP     49
****: D0 EB                     BNE     p7_4
****: A4 03             p7_3:   LDY     3
****: AD 30 C0                  LDA     49200
****: E6 05             p7_7:   INC     5
****: D0 03                     BNE     p7_5
****: E6 06                     INC     6
****: 60                        RTS
****: EA                p7_5:   NOP
****: 4C 43 00                  JMP     67
****: 88                        DEY
****: F0 D3                     BEQ     p7_6
****: 4C 49 00                  JMP     73
****: D0 ED                     BNE     p7_7
****: AD 91 00                  LDA     145
****: 0A                        ASL     A
****: A8                        TAY
****: B9 94 00                  LDA     148, Y
****: 85 02                     STA     2
****: AD 93 00                  LDA     147
****: 4A                p7_9:   LSR     A
****: F0 04                     BEQ     p7_8
****: 46 02                     LSR     2
****: D0 F9                     BNE     p7_9
****: B9 94 00          p7_8:   LDA     148, Y
****: 38                p7_13:  SEC
****: E5 02                     SBC     2
****: 85 04                     STA     4
****: C8                        INY
****: B9 94 00                  LDA     148, Y
****: 65 02                     ADC     2
****: 85 03                     STA     3
****: A9 00                     LDA     #0
****: 38                        SEC
****: ED 92 00                  SBC     146
****: 85 06                     STA     6
****: A9 00                     LDA     #0
****: 85 05                     STA     5
****: A5 04                     LDA     4
****: D0 9A                     BNE     p7_6
****: EA                p7_11:  NOP
****: EA                        NOP
****: 4C 84 00                  JMP     132
****: E6 05                     INC     5
****: D0 03                     BNE     p7_10
****: E6 06                     INC     6
****: 60                        RTS
****: EA                p7_10:  NOP
****: 4C 8F 00                  JMP     143
****: D0 EE                     BNE     p7_11
****: 00                        BRK
****: 00                        BRK
****: FF 00 00                  BBS     3, 0, p7_12
****: F6 F6             p7_12:  INC     246, X
****: E8                        INX
****: E8                        INX
****: DB                        STP
****: DB                        STP
****: CF CF C3                  BBS     0, 207, p7_13
****: C3                        .byte   195
****: B8                        CLV
****: B8                        CLV
****: AE AE A4                  LDX     42158
****: A4 9B                     LDY     155
****: 9B                        .byte   155
****: 92 92                     STA     (146)
****: 8A                        TXA
****: 8A                        TXA
****: 82                        .byte   130
****: 82                        .byte   130
****: 7B                        .byte   123
****: 7B                        .byte   123
****: 74 74                     STZ     116, X
****: 6D 6E 67                  ADC     26478
****: 68                        PLA
****: 61 62                     ADC     (98, X)
****: 5C                        .byte   92
****: 5C                        .byte   92
****: 57 57                     RMB     1, 87
****: 52 52                     EOR     (82)
****: 4D 4E 49                  EOR     18766
****: 49 45                     EOR     #69
****: 45 41                     EOR     65
****: 41 3D                     EOR     (61, X)
****: 3E 3A 3A                  ROL     14906, X
****: 36 37                     ROL     55, X
****: 33                        .byte   51
****: 34 30                     BIT     48, X
****: 31 2E                     AND     (46), Y
****: 2E 2B 2C                  ROL     11307
****: 29 29                     AND     #41
****: 26 27                     ROL     39
****: 24 25                     BIT     37
****: 22                        .byte   34
****: 23                        .byte   35
****: 20 21 1E                  JSR     7713
****: 1F 1D 1D                  BBR     1, 29, p7_14
****: 1B                        .byte   27
****: 1C 1A 1A                  TRB     6682
****: 18                        CLC
****: 19 17 17                  ORA     5911, Y
****: 15 16                     ORA     22, X
****: 14 15                     TRB     21
****: 13                        .byte   19
****: 14 12                     TRB     18
****: 12 11                     ORA     (17)
****: 11 10                     ORA     (16), Y
****: 10 0F                     BPL     p7_15
****: 10 0E                     BPL     p7_16
****: 0F 00 00                  BBR     0, 0, p7_17
; Procedure 7 Relocation Data:
****: F4 00             p7_17:  .word   244
****: F1 00                     .word   241
****: E9 00                     .word   233
****: D9 00                     .word   217
****: D5 00                     .word   213
****: C5 00             p7_15:  .word   197
****: C1 00                     .word   193
****: BE 00                     .word   190
****: BB 00                     .word   187
****: B8 00                     .word   184
****: B0 00                     .word   176
****: A9 00             p7_18:  .word   169
****: A1 00                     .word   161
****: 94 00                     .word   148
****: 8B 00                     .word   139
****: 0F 00                     .word   15              ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 7 Attribute Table:
****: 20 01                     .word   $ - p7_0        ; enter ic
****: 00                p7_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 02 00                     .word   $ - p7_at       ; procedure 7
****: 28 01                     .word   $ - p6_at       ; procedure 6
****: 60 01                     .word   $ - p5_at       ; procedure 5
****: BA 01                     .word   $ - p4_at       ; procedure 4
****: FC 01                     .word   $ - p3_at       ; procedure 3
****: 24 02                     .word   $ - p2_at       ; procedure 2
****: 5A 02                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 07                        .byte   7               ; number of procedures
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
