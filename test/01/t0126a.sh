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

TEST_SUBJECT="SET assignment"
. test_prelude

#
# SET assignment
#
cat > test.text << 'fubar'
program test;
var
    a: set of char;
    b: set of 'a'..'z';
    c: set of 0..63;
    d: set of 23..127;
begin
    a := b;
    c := d
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    38  TEST      LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: A5 03             p1_0:   LAO     3
****: A5 13                     LAO     19
****: BC 08                     LDM     8
****: 08                        SLDC    8
****: A0 10                     ADJ     16
****: BD 10                     STM     16
****: A5 1B                     LAO     27
****: A5 1F                     LAO     31
****: BC 08                     LDM     8
****: 08                        SLDC    8
****: A0 04                     ADJ     4
****: BD 04                     STM     4
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 48 00                     .word   72              ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 1E 00                     .word   $ - p1_0        ; enter ic
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

$bindir/ucsdpsys_compile test.text
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble -a test.code > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
