#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

TEST_SUBJECT="compile while"
. test_prelude

#
# compile while
#
cat > test.in << 'fubar'
program WHILE_DEMO;
const
    PI = 3.14;
var
    XL, Frequency, Inductance: real;
begin
    Inductance := 1.0;
    Frequency := 100.00;
    while  Frequency < 1000.00 do
    begin
        XL := 2 * PI * Frequency * Inductance;
        writeln('XL at ', Frequency, ' hertz = ', XL);
        Frequency := Frequency + 100.00
    end
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   156  WHILEDEM  LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: 1F                p1_0:   SLDC    31
****: 9E 15                     CSP     LOADSEGMENT
****: A5 03                     LAO     3
****: 01                        SLDC    1
****: 8A                        FLT
****: BD 02                     STM     2
****: A5 05                     LAO     5
****: 64                        SLDC    100             ; 'd'
****: 8A                        FLT
****: BD 02                     STM     2
****: A5 05             p1_2:   LAO     5
****: BC 02                     LDM     2
****: C7 E8 03                  LDCI    1000
****: 8A                        FLT
****: B5 02                     LES     REAL
****: A1 6C                     FJP     p1_1
****: A5 07                     LAO     7
****: D7                        NOP
****: B3 02 C8 40 C3            LDC     2, 16584, 62915 ; 6.28
      F5
****: A5 05                     LAO     5
****: BC 02                     LDM     2
****: 90                        MPR
****: A5 03                     LAO     3
****: BC 02                     LDM     2
****: 90                        MPR
****: BD 02                     STM     2
****: B6 01 03                  LOD     1, 3
****: A6 06 58 4C 20            LSA     6, "XL at "
      61 74 20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: A5 05                     LAO     5
****: BC 02                     LDM     2
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CD 1F 04                  CXP     31, 4           ; fwritereal (PASCALIO)
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: D7                        NOP
****: A6 09 20 68 65            LSA     9, " hertz = "
      72 74 7A 20 3D
      20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: A5 07                     LAO     7
****: BC 02                     LDM     2
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CD 1F 04                  CXP     31, 4           ; fwritereal (PASCALIO)
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: A5 05                     LAO     5
****: A5 05                     LAO     5
****: BC 02                     LDM     2
****: 64                        SLDC    100             ; 'd'
****: 8A                        FLT
****: 83                        ADR
****: BD 02                     STM     2
****: B9 F6                     UJP     p1_2
****: 1F                p1_1:   SLDC    31
****: 9E 16                     CSP     UNLOADSEGMENT
****: C1 00                     RBP     0
; Procedure 1 Jump Table:
****: 7D 00                     .word   $ - p1_2
; Procedure 1 Attribute Table:
****: 0C 00                     .word   12              ; data size
****: 04 00                     .word   4               ; param size
****: 0B 00                     .word   $ - p1_1        ; exit ic
****: 94 00                     .word   $ - p1_0        ; enter ic
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

$bindir/ucsdpsys_compile -o test.code test.in
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble -a test.code > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
