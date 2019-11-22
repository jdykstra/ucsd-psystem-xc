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

TEST_SUBJECT="adaptive temp var size"
. test_prelude

cat > example.text << 'fubar'
program example;
var
  s: string;
  t: string[20];
begin
  s := concat('One ', t, ' two.');
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    54  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: A5 03             p1_0:   LAO     3
****: A5 37                     LAO     55
****: D7                        NOP
****: A6 04 4F 6E 65            LSA     4, "One "
      20
****: AA 1D                     SAS     29
****: A5 37                     LAO     55
****: A5 2C                     LAO     44
****: 1D                        SLDC    29
****: CD 00 17                  CXP     0, 23           ; sconcat
****: A5 37                     LAO     55
****: A6 05 20 74 77            LSA     5, " two."
      6F 2E
****: 1D                        SLDC    29
****: CD 00 17                  CXP     0, 23           ; sconcat
****: A5 37                     LAO     55
****: AA 50                     SAS     80
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 86 00                     .word   134             ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 2E 00                     .word   $ - p1_0        ; enter ic
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
