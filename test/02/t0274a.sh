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

TEST_SUBJECT="UNITWRITE"
. test_prelude

cat > example.text << 'fubar'
program example;
var
    un: integer;
    a: packed array [0..511] of char;
    lngth: integer;
begin
    lngth := sizeof(a) - 29;
    unitwrite(un, a[29], lngth, 13, 17)
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    34  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: C7 E3 01          p1_0:   LDCI    483
****: AB 81 04                  SRO     260
****: EA                        SLDO    3
****: A5 04                     LAO     4
****: 1D                        SLDC    29
****: A9 81 04                  LDO     260
****: 0D                        SLDC    13
****: 11                        SLDC    17
****: 9E 06                     CSP     UNITWRITE
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 04 02                     .word   516             ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 1A 00                     .word   $ - p1_0        ; enter ic
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
