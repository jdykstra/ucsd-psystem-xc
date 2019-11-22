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

TEST_SUBJECT="string parameters"
. test_prelude

cat > example.text << 'fubar'
program example;
var f: text;      (* 602 *)
    name: string; (*  82 *)
begin
                  (* 256 *)
    openold(f, concat(name, '.text'))
end.
      (* data size = 940 *)
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
****: A5 03             p1_0:   LAO     3
****: A5 81 2F                  LAO     303
****: 02                        SLDC    2
****: 91                        NGI
****: CD 00 03                  CXP     0, 3            ; finit
****: A5 03                     LAO     3
****: A5 81 59                  LAO     345
****: A5 81 30                  LAO     304
****: AA 55                     SAS     85
****: A5 81 59                  LAO     345
****: A6 05 2E 74 65            LSA     5, ".text"
      78 74
****: 55                        SLDC    85              ; 'U'
****: CD 00 17                  CXP     0, 23           ; sconcat
****: A5 81 59                  LAO     345
****: 01                        SLDC    1
****: 9F                        LDCN
****: CD 00 05                  CXP     0, 5            ; fopen
****: 9E 00                     CSP     IOCHECK
****: A5 03             p1_1:   LAO     3
****: 00                        SLDC    0
****: CD 00 06                  CXP     0, 6            ; fclose
****: C1 00                     RBP     0
; Procedure 1 Attribute Table:
****: 02 03                     .word   770             ; data size
****: 04 00                     .word   4               ; param size
****: 0C 00                     .word   $ - p1_1        ; exit ic
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
