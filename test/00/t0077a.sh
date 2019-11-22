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

TEST_SUBJECT="compile sizeof"
. test_prelude

#
# compile sizeof
#
cat > test.in << 'fubar'
program test_sizeof;
var
    a: integer;
    b: record
           c: integer;
           d: real
       end;
begin
    writeln('sizeof(integer) = ', sizeof(integer));
    writeln('sizeof(a) = ', sizeof(a));
    writeln('sizeof(b) = ', sizeof(b))
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   148  TESTSIZE  LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: B6 01 03          p1_0:   LOD     1, 3
****: A6 12 73 69 7A            LSA     18, "sizeof(integer) = "
      65 6F 66 28 69
      6E 74 65 67 65
      72 29 20 3D 20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: 02                        SLDC    2
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: D7                        NOP
****: A6 0C 73 69 7A            LSA     12, "sizeof(a) = "
      65 6F 66 28 61
      29 20 3D 20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: 02                        SLDC    2
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: D7                        NOP
****: A6 0C 73 69 7A            LSA     12, "sizeof(b) = "
      65 6F 66 28 62
      29 20 3D 20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: 06                        SLDC    6
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 08 00                     .word   8               ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 8C 00                     .word   $ - p1_0        ; enter ic
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
