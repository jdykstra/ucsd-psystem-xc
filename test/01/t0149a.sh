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

TEST_SUBJECT="builtin ord function"
. test_prelude

#
# builtin ord function
#
cat > test.text << 'fubar'
program test;
var
    b: boolean;
    c: char;
    e: (zero, one, two, three);
    i: integer;
    j: integer;
begin
    i := ord(b);
    j := ord(true);
    i := ord(c);
    j := ord('a');
    i := ord(e);
    j := ord(three);
    i := ord(j);
    j := ord(42)
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    40  TEST      LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: EA                p1_0:   SLDO    3
****: AB 06                     SRO     6
****: 01                        SLDC    1
****: AB 07                     SRO     7
****: EB                        SLDO    4
****: AB 06                     SRO     6
****: 61                        SLDC    97              ; 'a'
****: AB 07                     SRO     7
****: EC                        SLDO    5
****: AB 06                     SRO     6
****: 03                        SLDC    3
****: AB 07                     SRO     7
****: EE                        SLDO    7
****: AB 06                     SRO     6
****: 2A                        SLDC    42              ; '*'
****: AB 07                     SRO     7
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 0A 00                     .word   10              ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 20 00                     .word   $ - p1_0        ; enter ic
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
