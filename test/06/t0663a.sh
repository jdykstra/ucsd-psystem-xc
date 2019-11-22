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

TEST_SUBJECT="detect ord/odd hack"
. test_prelude

cat > example.text << 'fubar'
program example;
var
  a, b, c: integer;
begin
  a := ord(odd(b) and odd(c));
  a := ord(odd(b) or odd(c));
  a := ord(not odd(b));
  a := ord((odd(a) and not odd(b)) or (not odd(a) and odd(b)));
end.
fubar
test $? -eq 0 || no_result

cat > ok1 << 'fubar'
ucsdpsys_compile: example.text: 5: warning: this expression appears to be
    using the ord/odd hack to gain access to the bit-wise opcodes; this
    cross compiler (and the ucsd-psystem-os native compiler) support
    integer parameters to the "and", "or" and "not" operators, allowing
    bit-wise operations without this ugly hack
ucsdpsys_compile: example.text: 6: warning: this expression appears to be
    using the ord/odd hack
ucsdpsys_compile: example.text: 7: warning: this expression appears to be
    using the ord/odd hack
ucsdpsys_compile: example.text: 8: warning: this expression appears to be
    using the ord/odd hack
fubar
test $? -eq 0 || no_result

cat > ok2 << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    42  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: EB                p1_0:   SLDO    4
****: EA                        SLDO    3
****: 84                        LAND
****: AB 05                     SRO     5
****: EB                        SLDO    4
****: EA                        SLDO    3
****: 8D                        LOR
****: AB 05                     SRO     5
****: EB                        SLDO    4
****: 93                        LNOT
****: AB 05                     SRO     5
****: EC                        SLDO    5
****: EB                        SLDO    4
****: 93                        LNOT
****: 84                        LAND
****: EC                        SLDO    5
****: 93                        LNOT
****: EB                        SLDO    4
****: 84                        LAND
****: 8D                        LOR
****: AB 05                     SRO     5
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 06 00                     .word   6               ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 22 00                     .word   $ - p1_0        ; enter ic
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

ucsdpsys_compile example.text > test.out 2>&1
test $? -eq 0 || fail

diff ok1 test.out
test $? -eq 0 || fail

ucsdpsys_disassemble -a example.code -o test.out
test $? -eq 0 || no_result

diff ok2 test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
