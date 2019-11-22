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

TEST_SUBJECT="assembler"
. test_prelude

cat > example.text << 'fubar'
; This isn't intended to be executable code,
; it is just to make sure all the opcodes work.
;
; .arch "p-code-le"
.proc eg
abi
abr
adi
adj 1
adr
bpt 42
cbp 1
cgp 1
chk
cip 1
clp 1
csp 0
cxp 31, 31
dif
dvi
dvr
equ 2
equi
flo
flt
geq 2
geqi
gtr 2
gtri
inc 1
ind 1
inn
int
ixa 1
ixp 1, 2
ixs
lae 1, 2
land
lao 1
lda 1, 2
ldb
ldc 1, 2
ldci 1234H
ldcn
lde 1, 2
ldl 1
ldm 5
ldo 1
ldp
leq 2
leqi
les 2
lesi
lla 1
lnot
lod 29, 31
lor
lpa 5, 80., 40H, 0x30, 16#20, 11H
lsa 5, "Hello"
modi
mov 7
mpi
mpr
neq 4
neqi
ngi
ngr
nop
rnp 0
sas 80
sbi
sbr
sgs
sind 2
sldl 3
sldo 4
sqi
sqr
sro 3210
srs
stb
ste 30, 456
stl 567
stm 4
sto
stp
str 1, 2
uni
exit_ic: rbp 0
.end
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   162  EG        HOSTSEG    1 P-Code LE 3.0
;              EG        SEPPROC          1         0
;              EG        GLOBDEF          1     $0000
;                        EOFMARK          1
;
; Procedure 1:
****: 80                p1_0:   ABI
****: 81                        ABR
****: 82                        ADI
****: A0 01                     ADJ     1
****: 83                        ADR
****: D5 2A                     BPT     42
****: C2 01                     CBP     1
****: CF 01                     CGP     1
****: 88                        CHK
****: AE 01                     CIP     1
****: CE 01                     CLP     1
****: 9E 00                     CSP     IOCHECK
****: CD 1F 1F                  CXP     31, 31
****: 85                        DIF
****: 86                        DVI
****: 87                        DVR
****: AF 02                     EQU     REAL
****: C3                        EQUI
****: 89                        FLO
****: 8A                        FLT
****: B0 02                     GEQ     REAL
****: C4                        GEQI
****: B1 02                     GTR     REAL
****: C5                        GTRI
****: A2 01                     INC     1
****: A3 01                     IND     1
****: 8B                        INN
****: 8C                        INT
****: A4 01                     IXA     1
****: C0 01 02                  IXP     1, 2
****: 9B                        IXS
****: A7 01 02                  LAE     1, 2
****: 84                        LAND
****: A5 01                     LAO     1
****: B2 01 02                  LDA     1, 2
****: BE                        LDB
****: B3 01 02 00               LDC     1, 2
****: C7 34 12                  LDCI    4660
****: 9F                        LDCN
****: 9D 01 02                  LDE     1, 2
****: CA 01                     LDL     1
****: BC 05                     LDM     5
****: A9 01                     LDO     1
****: BA                        LDP
****: B4 02                     LEQ     REAL
****: C8                        LEQI
****: B5 02                     LES     REAL
****: C9                        LESI
****: C6 01                     LLA     1
****: 93                        LNOT
****: B6 1D 1F                  LOD     29, 31
****: 8D                        LOR
****: D0 05 50 40 30            LPA     5, 80, 64, 48, 32, 17
      20 11
****: A6 05 48 65 6C            LSA     5, "Hello"
      6C 6F
****: 8E                        MODI
****: A8 07                     MOV     7
****: 8F                        MPI
****: 90                        MPR
****: B7 04                     NEQ     STR
****: CB                        NEQI
****: 91                        NGI
****: 92                        NGR
****: D7                        NOP
****: AD 00                     RNP     0
****: AA 50                     SAS     80
****: 95                        SBI
****: 96                        SBR
****: 97                        SGS
****: FA                        SIND    2
****: DA                        SLDL    3
****: EB                        SLDO    4
****: 98                        SQI
****: 99                        SQR
****: AB 8C 8A                  SRO     3210
****: 94                        SRS
****: BF                        STB
****: D1 1E 81 C8               STE     30, 456
****: CC 82 37                  STL     567
****: BD 04                     STM     4
****: 9A                        STO
****: BB                        STP
****: B8 01 02                  STR     1, 2
****: 9C                        UNI
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 9A 00                     .word   $ - p1_0        ; enter ic
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

ucsdpsys_assemble --arch=p-code-le example.text
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
