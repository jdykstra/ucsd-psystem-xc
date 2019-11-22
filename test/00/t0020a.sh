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

TEST_SUBJECT="compile division"
. test_prelude

#
# compile division
#
cat > test.in << 'fubar'
program division;
var
    i1, i2, i3: integer;
    r1, r2, r3: real;
begin
    i1 := i2 div i3;
    r1 := i2 / i3;
    r1 := i2 / r3;
    r1 := r2 / i3;
    r1 := r2 / r3
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    66  DIVISION  LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: EB                p1_0:   SLDO    4
****: EA                        SLDO    3
****: 86                        DVI
****: AB 05                     SRO     5
****: A5 0A                     LAO     10
****: EB                        SLDO    4
****: 8A                        FLT
****: EA                        SLDO    3
****: 8A                        FLT
****: 87                        DVR
****: BD 02                     STM     2
****: A5 0A                     LAO     10
****: EB                        SLDO    4
****: 8A                        FLT
****: A5 06                     LAO     6
****: BC 02                     LDM     2
****: 87                        DVR
****: BD 02                     STM     2
****: A5 0A                     LAO     10
****: A5 08                     LAO     8
****: BC 02                     LDM     2
****: EA                        SLDO    3
****: 8A                        FLT
****: 87                        DVR
****: BD 02                     STM     2
****: A5 0A                     LAO     10
****: A5 08                     LAO     8
****: BC 02                     LDM     2
****: A5 06                     LAO     6
****: BC 02                     LDM     2
****: 87                        DVR
****: BD 02                     STM     2
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 12 00                     .word   18              ; data size
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

$bindir/ucsdpsys_compile -o test.code test.in
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble -a -o test.out test.code
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
