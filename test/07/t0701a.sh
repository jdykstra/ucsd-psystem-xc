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

TEST_SUBJECT="ucsdpsys_link --copyright"
. test_prelude

cat > example.text << 'fubar'
program example;

  procedure nothing;
  external;

begin
  nothing
end.
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || no_result

cat > nothing.text << 'fubar'
        .arch   "6502"
        .proc   nothing
        rts
        .end
fubar
test $? -eq 0 || no_result

ucsdpsys_assemble nothing.text
test $? -eq 0 || no_result

#
# link the two.
#
cat > complete.code.ok << 'fubar'
; Disassembly of "complete.code"
; notice
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    34  EXAMPLE   LINKED     1 6502      3.0
;
; Procedure 1:
****: CE 02             p1_0:   CLP     2
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 0A 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level
;
; Procedure 2:                                          ; native 6502 code
                                .arch   "6502"
                                .radix  10.
****: 60                p2_0:   RTS
****: 00                        BRK
; Procedure 2 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 2 Attribute Table:
****: 0A 00                     .word   $ - p2_0        ; enter ic
****: 00                p2_at:  .byte   0               ; native machine code
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 02 00                     .word   $ - p2_at       ; procedure 2
****: 12 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_link --output=complete.code --copyright=notice \
    example.code nothing.code
test $? -eq 0 || fail

ucsdpsys_disassemble -a complete.code -o complete.code.dis
test $? -eq 0 || no_result

diff complete.code.ok complete.code.dis
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
