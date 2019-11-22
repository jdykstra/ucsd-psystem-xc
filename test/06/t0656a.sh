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

TEST_SUBJECT="assemble 65C02 opcodes"
. test_prelude

cat > example.text << 'fubar'
        .arch   "65c02s-wdc"
        .radix  10.
        .proc   example
p:      TSB     0       ; 04
        RMB     0, 0    ; 07
        TSB     256     ; 0C
        BBR     0, 0, p ; 0F
        ORA     0       ; 12
        TRB     0       ; 14
        RMB     1, 0    ; 17
        INC     A       ; 1A
        INA             ; 1A aka
        TRB     256     ; 1C
        BBR     0, 0, p ; 1F
        RMB     2, 0    ; 27
        BBR     2, 0, p ; 2F
        AND     (0)     ; 32
        BIT     0, X    ; 34
        RMB     3, 0    ; 37
        DEC     A       ; 3A
        DEA             ; 3A aka
        BIT     256, X  ; 3C
        BBR     3, 0, p ; 3F
q:      RMB     4, 0    ; 47
        BBR     4, 0, q ; 4F
        EOR     (0)     ; 52
        RMB     5, 0    ; 57
        PHY             ; 5A
        BBR     5, 0, q ; 5F
        STZ     0       ; 64
        RMB     6, 0    ; 67
        BBR     6, 0, q ; 6F
        ADC     (0)     ; 72
        STZ     0, x    ; 74
        RMB     7, 0    ; 77
        PLY             ; 7A
        JMP    (256, X) ; 7C
        BBR    7, 0, q  ; 7F
r:      BRA    r        ; 80
        SMB    0, 0     ; 87
        BIT    #0       ; 89
        BBS    0, 0, r  ; 8F
        STA    (0)      ; 92
        SMB    1, 0     ; 97
        STZ    256.     ; 9C
        STZ    256, X   ; 9E
        BBS    1, 0, r  ; 9F
        SMB    2, 0     ; A7
        BBS    2, 0, r  ; AF
        LDA    (0)      ; B2
        SMB    3, 0     ; B7
        BBS    3, 0, r  ; BF
s:      SMB    4, 0     ; C7
        WAI             ; CB
        BBS    4, 0, s  ; CF
        CMP    (0)      ; D2
        SMB    5, 0     ; D7
        PHX             ; DA
        STP             ; DB
        BBS    5, 0, s  ; DF
        SMB    6, 0     ; E7
        BBS    6, 0, s  ; EF
        SBC    (0)      ; F2
        SMB    7, 0     ; F7
        PLX             ; FA
        BBS    7, 0, s  ; FF
        .end
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   154  EXAMPLE   SEPRTSEG   1 6502      3.0
;              EXAMPLE   SEPPROC          1         0
;              EXAMPLE   GLOBDEF          1     $0000
;                        EOFMARK          1
;
; Procedure 1:                                          ; native 6502 code
                                .arch   "6502"
                                .radix  10.
****: 04 00             p1_0:   TSB     0
****: 07 00                     RMB     0, 0
****: 0C 00 01                  TSB     256
****: 0F 00 F6                  BBR     0, 0, p1_0
****: 05 00                     ORA     0
****: 14 00                     TRB     0
****: 17 00                     RMB     1, 0
****: 1A                        INC     A
****: 1A                        INC     A
****: 1C 00 01                  TRB     256
****: 0F 00 E8                  BBR     0, 0, p1_0
****: 27 00                     RMB     2, 0
****: 2F 00 E3                  BBR     2, 0, p1_0
****: 32 00                     AND     (0)
****: 34 00                     BIT     0, X
****: 37 00                     RMB     3, 0
****: D2 3A                     CMP     (58)
****: 3C 00 01                  BIT     256, X
****: 3F 00 D5                  BBR     3, 0, p1_0
****: 47 00             p1_1:   RMB     0, 0
****: 4F 00 FB                  BBR     0, 0, p1_1
****: 52 00                     EOR     (0)
****: 57 00                     RMB     1, 0
****: 5A                        PHY
****: 5F 00 F3                  BBR     1, 0, p1_1
****: 64 00                     STZ     0
****: 67 00                     RMB     2, 0
****: 6F 00 EC                  BBR     2, 0, p1_1
****: 72 00                     ADC     (0)
****: 74 00                     STZ     0, X
****: 77 00                     RMB     3, 0
****: 7A                        PLY
****: 7C 00 01                  JMP     (256, X)
****: 7F 00 DF                  BBR     3, 0, p1_1
****: 80 FE             p1_2:   BRA     p1_2
****: 87 00                     SMB     0, 0
****: 89 00                     BIT     #0
****: 8F 00 F7                  BBS     0, 0, p1_2
****: 92 00                     STA     (0)
****: 97 00                     SMB     1, 0
****: 9C 00 01                  STZ     256
****: 9E 00 01                  STZ     256, X
****: 9F 00 EA                  BBS     1, 0, p1_2
****: A7 00                     SMB     2, 0
****: AF 00 E5                  BBS     2, 0, p1_2
****: B2 00                     LDA     (0)
****: B7 00                     SMB     3, 0
****: BF 00 DE                  BBS     3, 0, p1_2
****: C7 00             p1_3:   SMB     0, 0
****: CB                        WAI
****: CF 00 FA                  BBS     0, 0, p1_3
****: D2 00                     CMP     (0)
****: D7 00                     SMB     1, 0
****: DA                        PHX
****: DB                        STP
****: DF 00 F1                  BBS     1, 0, p1_3
****: E7 00                     SMB     2, 0
****: EF 00 EC                  BBS     2, 0, p1_3
****: F2 00                     SBC     (0)
****: F7 00                     SMB     3, 0
****: FA                        PLX
****: FF 00 E4                  BBS     3, 0, p1_3
; Procedure 1 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 1 Attribute Table:
****: 92 00                     .word   $ - p1_0        ; enter ic
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
