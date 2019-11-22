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

TEST_SUBJECT="uses plain unit data"
. test_prelude

cat > fredunit.text << 'fubar'
(*$S+*)
unit fredunit;
interface

  procedure fred;

implementation

  procedure fred;
  begin
  end;

begin
end.
fubar
test $? -eq 0 || no_result

# ..............................................................................
#
# The unit defintion is not what is being tested here,
# but we sanity-check just in case.

cat > ok_0 << 'fubar'
; Disassembly of "fredunit.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     2    30  FREDUNIT  UNITSEG    7 P-Code LE 3.0
;        procedure fred;
;                        EOFMARK          1
;
; Procedure 2:
****: AD 00             p2_0:   RNP     0
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p2_0        ; exit ic
****: 08 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: AD 00             p1_0:   RNP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p1_0        ; exit ic
****: 08 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 01                        .byte   1               ; lex level

;
; Procedure dictionary:
;
****: 0E 00                     .word   $ - p2_at       ; procedure 2
****: 04 00                     .word   $ - p1_at       ; procedure 1
****: 07                        .byte   7               ; segment number
****: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile fredunit.text
test $? -eq 0 || no_result

ucsdpsys_disassemble -a fredunit.code -o test.out
test $? -eq 0 || no_result

diff ok_0 test.out
test $? -eq 0 || no_result

# ..............................................................................

cat > example.text << 'fubar'
program example;
(*$U fredunit.code *)
uses fredunit;
begin
  fred
end.
fubar
test $? -eq 0 || no_result

# Note the zero-length FREDUNIT segment.
cat > ok_1 << 'fubar'
     Library map for "example.code"

Code  Code   Segment   Segment  Seg Machine   System
Addr  Leng   Name      Kind     Num Type      Version
----- -----  --------  -------- --- --------- --------
    1    28  EXAMPLE   HOSTSEG    1 P-Code LE 3.0
                       EOFMARK          3
    0     0  FREDUNIT  UNITSEG    7 P-Code LE 3.0
fubar
test $? -eq 0 || no_result

cat > ok_2 << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    28  EXAMPLE   HOSTSEG    1 P-Code LE 3.0
;                        EOFMARK          3
;
; Procedure 1:
****: 07                p1_0:   SLDC    7
****: 9E 15                     CSP     LOADSEGMENT
****: CD 07 01                  CXP     7, 1
****: CD 07 02                  CXP     7, 2
****: 07                p1_1:   SLDC    7
****: 9E 16                     CSP     UNLOADSEGMENT
****: C1 00                     RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 09 00                     .word   $ - p1_1        ; exit ic
****: 14 00                     .word   $ - p1_0        ; enter ic
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

ucsdpsys_libmap example.code -o test.out
test $? -eq 0 || no_result

diff ok_1 test.out
test $? -eq 0 || fail

ucsdpsys_disassemble -a example.code -o test.out
test $? -eq 0 || no_result

diff ok_2 test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
