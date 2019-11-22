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

TEST_SUBJECT="RECORD subtype"
. test_prelude

#
# RECORD subtype
#
cat > test.in << 'fubar'
program record_subtype_test;
type
    daterec = packed record
        month: 0..12;
        day: 0..31;
        year: 0..100
    end;
var
    dtv: daterec;
begin
    dtv.year := 70;
    dtv.month := 1;
    dtv.day := 1
end.
fubar
test $? -eq 0 || fail

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    34  RECORDSU  LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: A5 03             p1_0:   LAO     3
****: 07                        SLDC    7
****: 09                        SLDC    9
****: 46                        SLDC    70              ; 'F'
****: BB                        STP
****: A5 03                     LAO     3
****: 04                        SLDC    4
****: 00                        SLDC    0
****: 01                        SLDC    1
****: BB                        STP
****: A5 03                     LAO     3
****: 05                        SLDC    5
****: 04                        SLDC    4
****: 01                        SLDC    1
****: BB                        STP
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 02 00                     .word   2               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 1A 00                     .word   $ - p1_0        ; enter ic
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
test $? -eq 0 || fail

$bindir/ucsdpsys_compile test.in -o test.code
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble -a test.code > test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
