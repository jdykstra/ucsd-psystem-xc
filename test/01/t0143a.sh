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

TEST_SUBJECT="forward procedure declarations"
. test_prelude

#
# forward procedure declarations
#
cat > test.text << 'fubar'
program test;

    procedure one(x: integer);
    forward;

    procedure two(y: integer);
    begin
        writeln('two ', y)
    end;

    procedure one;
    begin
        writeln('one ', x)
    end;

begin
    one(1);
    two(2)
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   118  TEST      LINKED     1 P-Code LE 3.0
;
; Procedure 3:
****: B6 02 03          p3_0:   LOD     2, 3
****: A6 04 74 77 6F            LSA     4, "two "
      20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 02 03                  LOD     2, 3
****: D8                        SLDL    1
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 02 03                  LOD     2, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: AD 00             p3_1:   RNP     0
****: D7                        NOP
; Procedure 3 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 02 00                     .word   2               ; param size
****: 07 00                     .word   $ - p3_1        ; exit ic
****: 2A 00                     .word   $ - p3_0        ; enter ic
****: 03                p3_at:  .byte   3               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 2:
****: B6 02 03          p2_0:   LOD     2, 3
****: A6 04 6F 6E 65            LSA     4, "one "
      20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 02 03                  LOD     2, 3
****: D8                        SLDL    1
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 02 03                  LOD     2, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: AD 00             p2_1:   RNP     0
****: D7                        NOP
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 02 00                     .word   2               ; param size
****: 07 00                     .word   $ - p2_1        ; exit ic
****: 2A 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: 01                p1_0:   SLDC    1
****: CE 02                     CLP     2
****: 02                        SLDC    2
****: CE 03                     CLP     3
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 0E 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 42 00                     .word   $ - p3_at       ; procedure 3
****: 16 00                     .word   $ - p2_at       ; procedure 2
****: 06 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 03                        .byte   3               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile test.text
test $? -eq 0 || fail

ucsdpsys_disassemble -a test.code > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
