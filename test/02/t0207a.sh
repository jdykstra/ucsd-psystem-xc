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

TEST_SUBJECT="IN operator"
. test_prelude

cat > example.text << 'fubar'
program example;
var
  x: integer;
begin
  x := 5;
  if x in [2, 7, 8, 10, 13, 17, 18, 19, 20, 32, 34, 35, 40, 42, 43]
  then
    writeln('yes')
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
****: 05                p1_0:   SLDC    5
****: AB 03                     SRO     3
****: EA                        SLDO    3
****: B3 03 0D 0D 1E            LDC     3, 3341, 30, 9604
      00 84 25
****: 03                        SLDC    3
****: 8B                        INN
****: A1 16                     FJP     p1_1
****: B6 01 03                  LOD     1, 3
****: A6 03 79 65 73            LSA     3, "yes"
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 02 00                     .word   2               ; data size
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
