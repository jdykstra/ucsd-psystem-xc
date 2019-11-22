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

TEST_SUBJECT="compile addition"
. test_prelude

#
# compile addition
#
cat > test.in << 'fubar'
program test;
var
    i: integer;
    r: real;
begin
    i := 40 + 2;
    i := i + 7;
    r := 4;
    r := 4.0;
    r := r + 38;
    r := r + 38.0
end.
fubar
test $? -eq 0 || fail

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    58  TEST      LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: 2A                p1_0:   SLDC    42              ; '*'
****: AB 03                     SRO     3
****: EA                        SLDO    3
****: 07                        SLDC    7
****: 82                        ADI
****: AB 03                     SRO     3
****: A5 04                     LAO     4
****: 04                        SLDC    4
****: 8A                        FLT
****: BD 02                     STM     2
****: A5 04                     LAO     4
****: 04                        SLDC    4
****: 8A                        FLT
****: BD 02                     STM     2
****: A5 04                     LAO     4
****: A5 04                     LAO     4
****: BC 02                     LDM     2
****: 26                        SLDC    38              ; '&'
****: 8A                        FLT
****: 83                        ADR
****: BD 02                     STM     2
****: A5 04                     LAO     4
****: A5 04                     LAO     4
****: BC 02                     LDM     2
****: 26                        SLDC    38              ; '&'
****: 8A                        FLT
****: 83                        ADR
****: BD 02                     STM     2
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 06 00                     .word   6               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 32 00                     .word   $ - p1_0        ; enter ic
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
test $? -eq 0 || fail

$bindir/ucsdpsys_compile -o test.code test.in
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble -a -o test.out test.code
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
