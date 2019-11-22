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

TEST_SUBJECT="integer call by reference"
. test_prelude

#
# integer call by reference
#
cat > test.in << 'fubar'
program test_call_by_reference;
var
    a: integer;

    procedure penguin(var arg: integer);
    begin
        writeln('arg = ', arg);
        arg := 0
    end;

begin
    a := 1;
    penguin(a)
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    78  TESTCALL  LINKED     1 P-Code LE 3.0
;
; Procedure 2:
****: B6 02 03          p2_0:   LOD     2, 3
****: A6 06 61 72 67            LSA     6, "arg = "
      20 3D 20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 02 03                  LOD     2, 3
****: D8                        SLDL    1
****: F8                        SIND    0
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 02 03                  LOD     2, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: D8                        SLDL    1
****: 00                        SLDC    0
****: 9A                        STO
****: AD 00             p2_1:   RNP     0
****: D7                        NOP
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 02 00                     .word   2               ; param size
****: 07 00                     .word   $ - p2_1        ; exit ic
****: 30 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: 01                p1_0:   SLDC    1
****: AB 03                     SRO     3
****: A5 03                     LAO     3
****: CE 02                     CLP     2
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 02 00                     .word   2               ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 10 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 16 00                     .word   $ - p2_at       ; procedure 2
****: 04 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 02                        .byte   2               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

$bindir/ucsdpsys_compile test.in -o test.code
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
