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

TEST_SUBJECT="VAR statement"
. test_prelude

cat > example.text << 'fubar'
program example;

  function sqrt(x: real): real;
  var
    result, diff: real;
  begin
    (* In reality, a maximum of 5 iterations is all that is required
     * for 32-bit floats.  *)
    result := x / 2;
    repeat
      begin
        var approx := result - (sqr(result) - x) / (2 * result);
        diff := abs(approx - result);
        result := approx
      end
    until
      diff < 1e-6;
    sqrt := result
  end;

begin
  writeln(sqrt(2))
end.
fubar
test $? -eq 0 || no_result

sed 's| *;;.*||' > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   140  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 2:
****: C6 07             p2_0:   LLA     7       ;; result := x / 2
****: C6 03                     LLA     3
****: BC 02                     LDM     2
****: 02                        SLDC    2
****: 8A                        FLT
****: 87                        DVR
****: BD 02                     STM     2
****: C6 09             p2_2:   LLA     9       ;; repeat approx :=
****: C6 07                     LLA     7       ;;   result -
****: BC 02                     LDM     2
****: C6 07                     LLA     7       ;;   (sqr(result) - x) /
****: BC 02                     LDM     2
****: 99                        SQR
****: C6 03                     LLA     3
****: BC 02                     LDM     2       ;;   (2 * result)
****: 96                        SBR
****: 02                        SLDC    2
****: 8A                        FLT
****: C6 07                     LLA     7
****: BC 02                     LDM     2
****: 90                        MPR
****: 87                        DVR
****: 96                        SBR
****: BD 02                     STM     2
****: C6 05                     LLA     5       ;; diff := abs(approx - result)
****: C6 09                     LLA     9
****: BC 02                     LDM     2
****: C6 07                     LLA     7
****: BC 02                     LDM     2
****: 96                        SBR
****: 81                        ABR
****: BD 02                     STM     2
****: C6 07                     LLA     7       ;; result := approx
****: C6 09                     LLA     9
****: A8 02                     MOV     2
****: C6 05                     LLA     5       ;; until diff < 1e-6
****: BC 02                     LDM     2
****: B3 02 86 35 BD            LDC     2, 13702, 14269 ; 1e-06
      37
****: B5 02                     LES     REAL
****: A1 F6                     FJP     p2_2
****: C6 01                     LLA     1       ;; sqrt := result
****: C6 07                     LLA     7
****: A8 02                     MOV     2
****: AD 02             p2_1:   RNP     2
; Procedure 2 Jump Table:
****: 45 00                     .word   $ - p2_2
; Procedure 2 Attribute Table:
****: 0C 00                     .word   12              ; data size
****: 08 00                     .word   8               ; param size
****: 08 00                     .word   $ - p2_1        ; exit ic
****: 58 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: 1F                p1_0:   SLDC    31
****: 9E 15                     CSP     LOADSEGMENT
****: B6 01 03                  LOD     1, 3
****: 02                        SLDC    2
****: 8A                        FLT
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CE 02                     CLP     2
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CD 1F 04                  CXP     31, 4           ; fwritereal (PASCALIO)
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: 1F                p1_1:   SLDC    31
****: 9E 16                     CSP     UNLOADSEGMENT
****: C1 00                     RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 09 00                     .word   $ - p1_1        ; exit ic
****: 26 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 2C 00                     .word   $ - p2_at       ; procedure 2
****: 04 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
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
