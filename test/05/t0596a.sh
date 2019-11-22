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

TEST_SUBJECT="compile --list -l"
. test_prelude

cat > example.text << 'fubar'
program example;

  procedure p2;
  begin
  end;

begin
  p2;
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Procedure 2:
; example.text: 1:              program example;
; example.text: 2:
; example.text: 3:                procedure p2;
; example.text: 4:                begin
; example.text: 5:                end;
s1 p2 o0:     AD 00             p2_0:   RNP     0
; Procedure 2 Attribute Table:
s1 p2 o2:     00 00                     .word   0               ; data size
s1 p2 o4:     00 00                     .word   0               ; param size
s1 p2 o6:     06 00                     .word   $ - p2_0        ; exit ic
s1 p2 o8:     08 00                     .word   $ - p2_0        ; enter ic
s1 p2 o10:    02                p2_at:  .byte   2               ; procedure
                                                                number
s1 p2 o11:    01                        .byte   1               ; lex level
; example.text: 3:                 procedure   p2
; example.text: 1:         0    1  parameter   bogus:out text
; example.text: 1:         1    1  parameter   bogus:in  text
; example.text: 1:                 program     example   program
; Procedure 1:
; example.text: 6:
; example.text: 7:              begin
; example.text: 8:                p2;
s1 p1 o0:     CE 02             p1_0:   CLP     2
; example.text: 9:              end.
s1 p1 o2:     C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
s1 p1 o4:     00 00                     .word   0               ; data size
s1 p1 o6:     04 00                     .word   4               ; param size
s1 p1 o8:     06 00                     .word   $ - p1_1        ; exit ic
s1 p1 o10:    0A 00                     .word   $ - p1_0        ; enter ic
s1 p1 o12:    01                p1_at:  .byte   1               ; procedure
                                                                number
s1 p1 o13:    00                        .byte   0               ; lex level
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text --list=test.out -l
test $? -eq 0 || fail

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
