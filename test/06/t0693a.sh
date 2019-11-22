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

TEST_SUBJECT="external data seg"
. test_prelude

cat > example.text << 'fubar'
(*$S+*)
unit example;
  intrinsic code 12 data 11;
interface

  var fred: integer;

  procedure bump;

implementation

  procedure bump;
  begin
    fred := fred + 1
  end;

begin
  fred := 0
end.
fubar
test $? -eq 0 || no_result

cat > ok.1 << 'fubar'
     Library map for "example.code"

Code  Code   Segment   Segment  Seg Machine   System
Addr  Leng   Name      Kind     Num Type      Version
----- -----  --------  -------- --- --------- --------
    0     2  EXAMPLE   DATASEG   11 P-Code LE 3.0
    2    42  EXAMPLE   LINKED_   12 P-Code LE 3.0
                       INTRINS
       var fred: integer;

       procedure bump;
fubar
test $? -eq 0 || no_result

cat > ok.2 << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     2    42  EXAMPLE   LINKED_   12 P-Code LE 3.0
;                        INTRINS
;        var fred: integer;
;
;        procedure bump;
;
; Procedure 2:
****: 9D 0B 01          p2_0:   LDE     11, 1
****: 01                        SLDC    1
****: 82                        ADI
****: D1 0B 01                  STE     11, 1
****: AD 00             p2_1:   RNP     0
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p2_1        ; exit ic
****: 10 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: 00                p1_0:   SLDC    0
****: D1 0B 01                  STE     11, 1
****: AD 00             p1_1:   RNP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 0C 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 01                        .byte   1               ; lex level

;
; Procedure dictionary:
;
****: 12 00                     .word   $ - p2_at       ; procedure 2
****: 04 00                     .word   $ - p1_at       ; procedure 1
****: 0C                        .byte   12              ; segment number
****: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || fail

# first, check that the data segment is present
ucsdpsys_libmap example.code > test.out
test $? -eq 0 || no_result
diff ok.1 test.out
test $? -eq 0 || fail

# second, check that the code segment is correct
ucsdpsys_disassemble -a example.code -o test.out
test $? -eq 0 || no_result
diff ok.2 test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
