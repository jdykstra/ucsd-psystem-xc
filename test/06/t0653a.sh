# !/bin/sh
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
        .arch   "6502"
        .proc   lotsa, 0
p1_0:   BRK
        ORA     (0, X)
        ORA     0
        ASL     0
        PHP
        ORA     #0
        ASL     A
        ORA     256.
        ASL     256.
p1_1:
        BPL     p1_1
        ORA     (0), Y
        ORA     0, X
        ASL     0, X
        CLC
        ORA     256., Y
        ORA     256., X
        ASL     256., X
        JSR     256.
        AND     (0, X)
        BIT     0
        AND     0
        ROL     0
        PLP
        AND     #0
        ROL     A
        BIT     256.
        AND     256.
        ROL     256.
p1_2:
        BMI     p1_2
        AND     (0), Y
        AND     0, X
        ROL     0, X
        SEC
        AND     256., Y
        AND     256., X
        ROL     256., X
        RTI
        EOR     (0, X)
        EOR     0
        LSR     0
        PHA
        EOR     #0
        LSR     A
        JMP     256.
        EOR     256.
        LSR     256.
p1_3:
        BVC     p1_3
        EOR     (0), Y
        EOR     0, X
        LSR     0, X
        CLI
        EOR     256., Y
        EOR     256., X
        LSR     256., X
        RTS
        ADC     (0, X)
        ADC     0
        ROR     0
        PLA
        ADC     #0
        ROR     A
        JMP     (256.)
        ADC     256.
        ROR     256.
p1_4:
        BVS     p1_4
        ADC     (0), Y
        ADC     0, X
        ROR     0, X
        SEI
        ADC     256., Y
        ADC     256., X
        ROR     256., X
        STA     (0, X)
        STY     0
        STA     0
        STX     0
        DEY
        TXA
        STY     256.
        STA     256.
        STX     256.
p1_5:
        BCC     p1_5
        STA     (0), Y
        STY     0, X
        STA     0, X
        STX     0, Y
        TYA
        STA     256., Y
        TXS
        STA     256., X
        LDY     #0
        LDA     (0, X)
        LDX     #0
        LDY     0
        LDA     0
        LDX     0
        TAY
        LDA     #0
        TAX
        LDY     256.
        LDA     256.
        LDX     256.
p1_6:
        BCS     p1_6
        LDA     (0), Y
        LDY     0, X
        LDA     0, X
        LDX     0, Y
        CLV
        LDA     256., Y
        TXS
        LDY     256., X
        LDA     256., X
        LDX     256., Y
        CPY     #0
        CMP     (0, X)
        CPY     0
        CMP     0
        DEC     0
        INY
        CMP     #0
        DEX
        CPY     256.
        CMP     256.
        DEC     256.
p1_7:
        BNE     p1_7
        CMP     (0), Y
        CMP     0, X
        DEC     0, X
        CLD
        CMP     256., Y
        CMP     256., X
        DEC     256., X
        CPX     #0
        SBC     (0, X)
        CPX     0
        SBC     0
        INC     0
        INX
        SBC     #0
        NOP
        CPX     256.
        SBC     256.
        INC     256.
p1_8:
        BEQ     p1_8
        SBC     (0), Y
        SBC     0, X
        INC     0, X
        SED
        SBC     256., Y
        SBC     256., X
        INC     256., X
        .end
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   338  LOTSA     SEPRTSEG   1 6502      3.0
;              LOTSA     SEPPROC          1         0
;              LOTSA     GLOBDEF          1     $0000
;                        EOFMARK          1
;
; Procedure 1:                                          ; native 6502 code
                                .arch   "6502"
                                .radix  10.
****: 00                p1_0:   BRK
****: 01 00                     ORA     (0, X)
****: 05 00                     ORA     0
****: 06 00                     ASL     0
****: 08                        PHP
****: 09 00                     ORA     #0
****: 0A                        ASL     A
****: 0D 00 01                  ORA     256
****: 0E 00 01                  ASL     256
****: 10 FE             p1_1:   BPL     p1_1
****: 11 00                     ORA     (0), Y
****: 15 00                     ORA     0, X
****: 16 00                     ASL     0, X
****: 18                        CLC
****: 19 00 01                  ORA     256, Y
****: 1D 00 01                  ORA     256, X
****: 1E 00 01                  ASL     256, X
****: 20 00 01                  JSR     256
****: 21 00                     AND     (0, X)
****: 24 00                     BIT     0
****: 25 00                     AND     0
****: 26 00                     ROL     0
****: 28                        PLP
****: 29 00                     AND     #0
****: 2A                        ROL     A
****: 2C 00 01                  BIT     256
****: 2D 00 01                  AND     256
****: 2E 00 01                  ROL     256
****: 30 FE             p1_2:   BMI     p1_2
****: 31 00                     AND     (0), Y
****: 35 00                     AND     0, X
****: 36 00                     ROL     0, X
****: 38                        SEC
****: 39 00 01                  AND     256, Y
****: 3D 00 01                  AND     256, X
****: 3E 00 01                  ROL     256, X
****: 40                        RTI
****: 41 00                     EOR     (0, X)
****: 45 00                     EOR     0
****: 46 00                     LSR     0
****: 48                        PHA
****: 49 00                     EOR     #0
****: 4A                        LSR     A
****: 4C 00 01                  JMP     256
****: 4D 00 01                  EOR     256
****: 4E 00 01                  LSR     256
****: 50 FE             p1_3:   BVC     p1_3
****: 51 00                     EOR     (0), Y
****: 55 00                     EOR     0, X
****: 56 00                     LSR     0, X
****: 58                        CLI
****: 59 00 01                  EOR     256, Y
****: 5D 00 01                  EOR     256, X
****: 5E 00 01                  LSR     256, X
****: 60                        RTS
****: 61 00                     ADC     (0, X)
****: 65 00                     ADC     0
****: 66 00                     ROR     0
****: 68                        PLA
****: 69 00                     ADC     #0
****: 6A                        ROR     A
****: 6C 00 01                  JMP     (256)
****: 6D 00 01                  ADC     256
****: 6E 00 01                  ROR     256
****: 70 FE             p1_4:   BVS     p1_4
****: 71 00                     ADC     (0), Y
****: 75 00                     ADC     0, X
****: 76 00                     ROR     0, X
****: 78                        SEI
****: 79 00 01                  ADC     256, Y
****: 7D 00 01                  ADC     256, X
****: 7E 00 01                  ROR     256, X
****: 81 00                     STA     (0, X)
****: 84 00                     STY     0
****: 85 00                     STA     0
****: 86 00                     STX     0
****: 88                        DEY
****: 8A                        TXA
****: 8C 00 01                  STY     256
****: 8D 00 01                  STA     256
****: 8E 00 01                  STX     256
****: 90 FE             p1_5:   BCC     p1_5
****: 91 00                     STA     (0), Y
****: 94 00                     STY     0, X
****: 95 00                     STA     0, X
****: 96 00                     STX     0, Y
****: 98                        TYA
****: 99 00 01                  STA     256, Y
****: 9A                        TXS
****: 9D 00 01                  STA     256, X
****: A0 00                     LDY     #0
****: A1 00                     LDA     (0, X)
****: A2 00                     LDX     #0
****: A4 00                     LDY     0
****: A5 00                     LDA     0
****: A6 00                     LDX     0
****: A8                        TAY
****: A9 00                     LDA     #0
****: AA                        TAX
****: AC 00 01                  LDY     256
****: AD 00 01                  LDA     256
****: AE 00 01                  LDX     256
****: B0 FE             p1_6:   BCS     p1_6
****: B1 00                     LDA     (0), Y
****: B4 00                     LDY     0, X
****: B5 00                     LDA     0, X
****: B6 00                     LDX     0, Y
****: B8                        CLV
****: B9 00 01                  LDA     256, Y
****: 9A                        TXS
****: BC 00 01                  LDY     256, X
****: BD 00 01                  LDA     256, X
****: BE 00 01                  LDX     256, Y
****: C0 00                     CPY     #0
****: C1 00                     CMP     (0, X)
****: C4 00                     CPY     0
****: C5 00                     CMP     0
****: C6 00                     DEC     0
****: C8                        INY
****: C9 00                     CMP     #0
****: CA                        DEX
****: CC 00 01                  CPY     256
****: CD 00 01                  CMP     256
****: CE 00 01                  DEC     256
****: D0 FE             p1_7:   BNE     p1_7
****: D1 00                     CMP     (0), Y
****: D5 00                     CMP     0, X
****: D6 00                     DEC     0, X
****: D8                        CLD
****: D9 00 01                  CMP     256, Y
****: DD 00 01                  CMP     256, X
****: DE 00 01                  DEC     256, X
****: E0 00                     CPX     #0
****: E1 00                     SBC     (0, X)
****: E4 00                     CPX     0
****: E5 00                     SBC     0
****: E6 00                     INC     0
****: E8                        INX
****: E9 00                     SBC     #0
****: EA                        NOP
****: EC 00 01                  CPX     256
****: ED 00 01                  SBC     256
****: EE 00 01                  INC     256
****: F0 FE             p1_8:   BEQ     p1_8
****: F1 00                     SBC     (0), Y
****: F5 00                     SBC     0, X
****: F6 00                     INC     0, X
****: F8                        SED
****: F9 00 01                  SBC     256, Y
****: FD 00 01                  SBC     256, X
****: FE 00 01                  INC     256, X
****: 00                        BRK
; Procedure 1 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 1 Attribute Table:
****: 4A 01                     .word   $ - p1_0        ; enter ic
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
