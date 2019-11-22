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

TEST_SUBJECT="exit vs seg 0"
. test_prelude

cat > example.text << 'fubar'
(*$U-*)
program example;
  procedure getcmd;
    procedure fred;
    begin
      exit(getcmd)
    end;
  begin
  end;
begin
  getcmd
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    50  EXAMPLE   LINKED     0 Undefined Pre-II.1
;
; Procedure 3:
****: 00                p3_0:   SLDC    0
****: 02                        SLDC    2
****: 9E 04                     CSP     EXIT
****: AD 00             p3_1:   RNP     0
; Procedure 3 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p3_1        ; exit ic
****: 0C 00                     .word   $ - p3_0        ; enter ic
****: 03                p3_at:  .byte   3               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 2:
****: C1 00             p2_0:   RBP     0
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p2_0        ; exit ic
****: 08 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 00                        .byte   0               ; lex level
;
; Procedure 1:
****: C2 02             p1_0:   CBP     2
****: D6                p1_1:   XIT
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 0A 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: FF                        .byte   -1              ; lex level

;
; Procedure dictionary:
;
****: 1C 00                     .word   $ - p3_at       ; procedure 3
****: 12 00                     .word   $ - p2_at       ; procedure 2
****: 06 00                     .word   $ - p1_at       ; procedure 1
****: 00                        .byte   0               ; segment number
****: 03                        .byte   3               ; number of procedures
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
