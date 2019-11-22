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

TEST_SUBJECT="function call location"
. test_prelude

cat > example.text << 'fubar'
program example;

  procedure nothing;
  begin
  end;

begin
  nothing;
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Procedure 2:
; example.text: 1:      program example;
; example.text: 2:
; example.text: 3:        procedure nothing;
; example.text: 4:        begin
; example.text: 5:        end;
0000: AD 00             p2_0:   RNP     0
; Procedure 2 Attribute Table:
0002: 00 00                     .word   0               ; data size
0004: 00 00                     .word   0               ; param size
0006: 06 00                     .word   $ - p2_0        ; exit ic
0008: 08 00                     .word   $ - p2_0        ; enter ic
000A: 02                p2_at:  .byte   2               ; procedure number
000B: 01                        .byte   1               ; lex level
; example.text: 1:         0    1  parameter   bogus:out text
; example.text: 1:         1    1  parameter   bogus:in  text
; example.text: 1:                 program     example   program
; example.text: 3:                 procedure   nothing
; Procedure 1:
; example.text: 6:
; example.text: 7:      begin
; example.text: 8:        nothing;
000C: CE 02             p1_0:   CLP     2
; example.text: 9:      end.
000E: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
0010: 00 00                     .word   0               ; data size
0012: 04 00                     .word   4               ; param size
0014: 06 00                     .word   $ - p1_1        ; exit ic
0016: 0A 00                     .word   $ - p1_0        ; enter ic
0018: 01                p1_at:  .byte   1               ; procedure number
0019: 00                        .byte   0               ; lex level
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text --listing=test.out
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
