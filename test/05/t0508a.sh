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

TEST_SUBJECT="expression location"
. test_prelude

cat > example.text << 'fubar'
program example;
type
  eg_rec =
    record
      s: string;
    end;
var
  p: ^eg_rec;
begin
  with p^ do
    if (length(s) > 5) and (copy(s, length(s) - 4, 5) = '.code') then
      s := copy(s, 1, length(s) - 5);
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    66  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: EA                p1_0:   SLDO    3
****: 00                        SLDC    0
****: BE                        LDB
****: 05                        SLDC    5
****: C5                        GTRI
****: A1 2A                     FJP     p1_1
****: EA                        SLDO    3
****: A5 04                     LAO     4
****: EA                        SLDO    3
****: 00                        SLDC    0
****: BE                        LDB
****: 04                        SLDC    4
****: 95                        SBI
****: 05                        SLDC    5
****: CD 00 19                  CXP     0, 25           ; scopy
****: A5 04                     LAO     4
****: A6 05 2E 63 6F            LSA     5, ".code"
      64 65
****: AF 04                     EQU     STR
****: A1 11                     FJP     p1_1
****: EA                        SLDO    3
****: EA                        SLDO    3
****: A5 07                     LAO     7
****: 01                        SLDC    1
****: EA                        SLDO    3
****: 00                        SLDC    0
****: BE                        LDB
****: 05                        SLDC    5
****: 95                        SBI
****: CD 00 19                  CXP     0, 25           ; scopy
****: A5 07                     LAO     7
****: AA 50                     SAS     80
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 5A 00                     .word   90              ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 3A 00                     .word   $ - p1_0        ; enter ic
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
