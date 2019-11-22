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

TEST_SUBJECT="segment procedure forward"
. test_prelude

cat > example.text << 'fubar'
(*$U-*)
program seg0;

  segment procedure seg1; forward;

  segment procedure seg2; forward;
  segment procedure seg3; forward;
  segment procedure seg4; forward;

  segment procedure seg1;
    procedure junk1;
    begin
    end;

    function getch: char;
      forward;

    procedure junk;
      procedure junk2;
      begin
      end;
      procedure junk3;
      begin
      end;
    begin
    end;

    FUNCTION GETCH{:CHAR};
    var c: char;
    BEGIN
      READ(KEYBOARD,c);
      IF EOLN(KEYBOARD) THEN
        c := CHR(13)
      ELSE IF c IN ['a'..'z'] THEN
        c := CHR( ORD(c) - ORD('a') + ORD('A') );
      GETCH := c;
    END;

  begin
    write(getch);
  end;

  segment procedure seg2;
  begin
  end;

  segment procedure seg3;
  begin
  end;

  segment procedure seg4;
  begin
  end;

begin
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     5    16  SEG0      LINKED     0 Undefined Pre-II.1
;
; Procedure 1:
****: D6                p1_0:   XIT
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p1_0        ; exit ic
****: 08 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: FF                        .byte   -1              ; lex level

;
; Procedure dictionary:
;
****: 02 00                     .word   $ - p1_at       ; procedure 1
****: 00                        .byte   0               ; segment number
****: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   154  SEG1      LINKED     1 P-Code LE 3.0
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
; Procedure 5:
****: AD 00             p5_0:   RNP     0
; Procedure 5 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p5_0        ; exit ic
****: 08 00                     .word   $ - p5_0        ; enter ic
****: 05                p5_at:  .byte   5               ; procedure number
****: 02                        .byte   2               ; lex level
;
; Procedure 6:
****: AD 00             p6_0:   RNP     0
; Procedure 6 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p6_0        ; exit ic
****: 08 00                     .word   $ - p6_0        ; enter ic
****: 06                p6_at:  .byte   6               ; procedure number
****: 02                        .byte   2               ; lex level
;
; Procedure 4:
****: AD 00             p4_0:   RNP     0
; Procedure 4 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p4_0        ; exit ic
****: 08 00                     .word   $ - p4_0        ; enter ic
****: 04                p4_at:  .byte   4               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 3:
****: B6 02 04          p3_0:   LOD     2, 4
****: C6 03                     LLA     3
****: CD 00 10                  CXP     0, 16           ; freadchar
****: B6 02 04                  LOD     2, 4
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CD 00 0B                  CXP     0, 11           ; feoln
****: A1 05                     FJP     p3_2
****: 0D                        SLDC    13
****: CC 03                     STL     3
****: B9 1E                     UJP     p3_3
****: DA                p3_2:   SLDL    3
****: B3 08 FF 07 FE            LDC     8, 2047, 65534, 0, 0, 0, 0, 0, 0
      FF 00 00 00 00
      00 00 00 00 00
      00 00 00
****: 08                        SLDC    8
****: 8B                        INN
****: A1 07                     FJP     p3_3
****: DA                        SLDL    3
****: 61                        SLDC    97              ; 'a'
****: 95                        SBI
****: 41                        SLDC    65              ; 'A'
****: 82                        ADI
****: CC 03                     STL     3
****: DA                p3_3:   SLDL    3
****: CC 01                     STL     1
****: AD 01             p3_1:   RNP     1
; Procedure 3 Attribute Table:
****: 02 00                     .word   2               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p3_1        ; exit ic
****: 40 00                     .word   $ - p3_0        ; enter ic
****: 03                p3_at:  .byte   3               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: B6 01 03          p1_0:   LOD     1, 3
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CE 03                     CLP     3
****: 00                        SLDC    0
****: CD 00 11                  CXP     0, 17           ; fwritechar
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 14 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 6A 00                     .word   $ - p6_at       ; procedure 6
****: 78 00                     .word   $ - p5_at       ; procedure 5
****: 62 00                     .word   $ - p4_at       ; procedure 4
****: 20 00                     .word   $ - p3_at       ; procedure 3
****: 8A 00                     .word   $ - p2_at       ; procedure 2
****: 0C 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 06                        .byte   6               ; number of procedures
;
; ------------------------------------------------------------------
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     2    16  SEG2      LINKED     2 P-Code LE 3.0
;
; Procedure 1:
****: C1 00             p1_0:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p1_0        ; exit ic
****: 08 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 02 00                     .word   $ - p1_at       ; procedure 1
****: 02                        .byte   2               ; segment number
****: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     3    16  SEG3      LINKED     3 P-Code LE 3.0
;
; Procedure 1:
****: C1 00             p1_0:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p1_0        ; exit ic
****: 08 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 02 00                     .word   $ - p1_at       ; procedure 1
****: 03                        .byte   3               ; segment number
****: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     4    16  SEG4      LINKED     4 P-Code LE 3.0
;
; Procedure 1:
****: C1 00             p1_0:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p1_0        ; exit ic
****: 08 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 02 00                     .word   $ - p1_at       ; procedure 1
****: 04                        .byte   4               ; segment number
****: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text
test $? -eq 0 || fail

ucsdpsys_disassemble -a example.code -o test.out
test $? -eq 0 || no_result

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
