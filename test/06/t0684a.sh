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

TEST_SUBJECT="ucsdpsys_link"
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

cat > example.code.ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    20  EXAMPLE   HOSTSEG    1 P-Code LE 3.0
;              NOTHING   EXTPROC          2         0
;                        EOFMARK          3
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
; Procedure dictionary:
;
****: 00 00                     .word   0               ; procedure 2 external
****: 04 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || no_result

ucsdpsys_disassemble -a example.code -o example.code.dis
test $? -eq 0 || no_result

diff example.code.ok example.code.dis
test $? -eq 0 || no_result

cat > nothing.text << 'fubar'
        .arch   "6502"
        .proc   nothing
        rts
        .end
fubar
test $? -eq 0 || no_result

cat > nothing.code.ok << 'fubar'
; Disassembly of "nothing.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    18  NOTHING   SEPRTSEG   1 6502      3.0
;              NOTHING   SEPPROC          1         0
;              NOTHING   GLOBDEF          1     $0000
;                        EOFMARK          1
;
; Procedure 1:                                          ; native 6502 code
                                .arch   "6502"
                                .radix  10.
****: 60                p1_0:   RTS
****: 00                        BRK
; Procedure 1 Relocation Data:
****: 00 00                     .word   0               ; .interp relative
****: 00 00                     .word   0               ; pc relative
****: 00 00                     .word   0               ; .ref relative
****: 00 00                     .word   0               ; .public relative
; Procedure 1 Attribute Table:
****: 0A 00                     .word   $ - p1_0        ; enter ic
****: 00                p1_at:  .byte   0               ; native machine code
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

ucsdpsys_assemble nothing.text
test $? -eq 0 || no_result

ucsdpsys_disassemble -a nothing.code -o nothing.code.dis
test $? -eq 0 || no_result

diff nothing.code.ok nothing.code.dis
test $? -eq 0 || no_result

#
# This is the real meat: link the two.
#
cat > complete.code.ok << 'fubar'
; Disassembly of "complete.code"
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

cat > complete.map.ok << 'fubar'
Link map for assembler host

Segment 1, "EXAMPLE":
   Separate procedures:
      "NOTHING"  proc   2, base =    14, leng =   14
fubar
test $? -eq 0 || no_result

ucsdpsys_link --output=complete.code --map=complete.map \
    example.code nothing.code
test $? -eq 0 || fail

ucsdpsys_disassemble -a complete.code -o complete.code.dis
test $? -eq 0 || no_result

diff complete.code.ok complete.code.dis
test $? -eq 0 || fail

diff complete.map.ok complete.map
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
