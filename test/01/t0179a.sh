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

TEST_SUBJECT="variant records"
. test_prelude

cat > example.text << 'fubar'
program example;
type
  test =
    record
    case integer of
    1: (i: integer);
    2: (r: real);
    3: (c: char);
    4: (s: set of 0..255)
    end;
begin
  writeln(sizeof(test))
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    34  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
0000: B6 01 03          p1_0:   LOD     1, 3
0003: 20                        SLDC    32              ; ' '
0004: 00                        SLDC    0
0005: CD 00 0D                  CXP     0, 13           ; fwriteint
0008: 9E 00                     CSP     IOCHECK
000A: B6 01 03                  LOD     1, 3
000D: CD 00 16                  CXP     0, 22           ; fwriteln
0010: 9E 00                     CSP     IOCHECK
0012: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
0014: 00 00                     .word   0               ; data size
0016: 04 00                     .word   4               ; param size
0018: 06 00                     .word   $ - p1_1        ; exit ic
001A: 1A 00                     .word   $ - p1_0        ; enter ic
001C: 01                p1_at:  .byte   1               ; procedure number
001D: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
001E: 02 00                     .word   $ - p1_at       ; procedure 1
0020: 01                        .byte   1               ; segment number
0021: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || fail

ucsdpsys_disassemble example.code -o test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
