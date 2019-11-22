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

TEST_SUBJECT="USES transcendental"
. test_prelude

cat > example.text << 'fubar'
program example;
uses transcend;
begin
writeln(sqrt(2))
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    56  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: 1F                p1_0:   SLDC    31
****: 9E 15                     CSP     LOADSEGMENT
****: 1D                        SLDC    29
****: 9E 15                     CSP     LOADSEGMENT
****: CD 1D 01                  CXP     29, 1
****: B6 01 03                  LOD     1, 3
****: 02                        SLDC    2
****: 8A                        FLT
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CD 1D 08                  CXP     29, 8           ; sqrt (TRANSCEN)
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CD 1F 04                  CXP     31, 4           ; fwritereal (PASCALIO)
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: 1D                p1_1:   SLDC    29
****: 9E 16                     CSP     UNLOADSEGMENT
****: 1F                        SLDC    31
****: 9E 16                     CSP     UNLOADSEGMENT
****: C1 00                     RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 0C 00                     .word   $ - p1_1        ; exit ic
****: 30 00                     .word   $ - p1_0        ; enter ic
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

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
