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

TEST_SUBJECT="LDB (load byte)"
. test_prelude

cat > example.text << 'fubar'
(*$U-*)
program example;
const
  out_unit = 1;
var
  a7, a6, a5, a4, a3, a2, a1, a0: char;
  number_of_errors: integer;
  byte_sex_lohi: boolean;
  crlf: char;

  PROCEDURE BAD;
  VAR B1, B0: CHAR;
  BEGIN
    B0 := ' ';
    B1 := 'B';
    UNITWRITE(OUTUNIT, B0, 4);
    number_of_errors := number_of_errors + 1;
  END;

  PROCEDURE GOOD;
  VAR G1, G0: CHAR;
  BEGIN
    G0 := ' ';
    G1 := 'G';
    UNITWRITE(OUTUNIT, G0, 4);
  END;

  PROCEDURE CHECKLDBSTB;
  VAR INFRONT: INTEGER;
      SMALLLDBSTB: RECORD CASE BOOLEAN OF
                    TRUE: (INT0: INTEGER;
                          INT1: INTEGER);
                    FALSE: (PACKS: PACKED ARRAY[0..3] OF 0..255);
                  END;
      INBACK: INTEGER;
  BEGIN
    (* Check LDB+STB with positive offsets only. *)
    A0 := 'L'; A1 := 'D'; A2 := 'B'; A3 := '+';
    A4 := 'S'; A5 := 'T'; A6 := 'B'; A7 := ':';
    UNITWRITE(OUTUNIT, A0, 16);

    WITH SMALLLDBSTB DO
    BEGIN
      INT0 := 23155;
      INT1 := 4660;
      INFRONT := 0;
      INBACK := 0;
      IF BYTESEXLOHI THEN
        BEGIN
          IF PACKS[0] = 115 THEN GOOD ELSE BAD;
          IF PACKS[1] = 90 THEN GOOD ELSE BAD;
          IF PACKS[2] = 52 THEN GOOD ELSE BAD;
          IF PACKS[3] = 18 THEN GOOD ELSE BAD;
          PACKS[0] := 20;
          IF INT0 = 23060 THEN GOOD ELSE BAD;
          PACKS[3] := 42;
          IF INT1 = 10804 THEN GOOD ELSE BAD;
        END
      ELSE
        BEGIN
          IF PACKS[0]=90 THEN GOOD ELSE BAD;
          IF PACKS[1]=115 THEN GOOD ELSE BAD;
          IF PACKS[2]=18 THEN GOOD ELSE BAD;
          IF PACKS[3]=52 THEN GOOD ELSE BAD;
          PACKS[0] := 20;
          IF INT0=5235 THEN GOOD ELSE BAD;
          PACKS[3] := 42;
          IF INT1=4650 THEN GOOD ELSE BAD;
        END;
      IF INFRONT=0 THEN GOOD ELSE BAD;
      IF INBACK=0 THEN GOOD ELSE BAD;
    END;
    UNITWRITE(OUTUNIT, CRLF, 2);
  END;

  procedure get_byte_sex;
  var
    x: packed record
       case boolean of
       true: (n: integer);
       false: (a: packed array[0..1] of char);
       end;
  begin
    x.n := 1;
    byte_sex_lohi := (1 = ord(x.a[0]));
  end;

begin
  get_byte_sex;
  number_of_errors := 0;
  CHECKLDBSTB;
  if number_of_errors <> 0 then
    begin
      a0 := 'o';
      a1 := 'o';
      a2 := 'p';
      a3 := 's';
      a4 := chr(13);
      unitwrite(outunit, a0, 10);
    end;
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   426  EXAMPLE   LINKED     0 Undefined Pre-II.1
;
; Procedure 2:
****: 20                p2_0:   SLDC    32              ; ' '
****: AB 01                     SRO     1
****: 42                        SLDC    66              ; 'B'
****: AB 02                     SRO     2
****: 01                        SLDC    1
****: A5 01                     LAO     1
****: 00                        SLDC    0
****: 04                        SLDC    4
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 9E 06                     CSP     UNITWRITE
****: B6 01 09                  LOD     1, 9
****: 01                        SLDC    1
****: 82                        ADI
****: B8 01 09                  STR     1, 9
****: C1 00             p2_1:   RBP     0
****: D7                        NOP
; Procedure 2 Attribute Table:
****: 04 00                     .word   4               ; data size
****: 00 00                     .word   0               ; param size
****: 07 00                     .word   $ - p2_1        ; exit ic
****: 20 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 00                        .byte   0               ; lex level
;
; Procedure 3:
****: 20                p3_0:   SLDC    32              ; ' '
****: AB 01                     SRO     1
****: 47                        SLDC    71              ; 'G'
****: AB 02                     SRO     2
****: 01                        SLDC    1
****: A5 01                     LAO     1
****: 00                        SLDC    0
****: 04                        SLDC    4
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 9E 06                     CSP     UNITWRITE
****: C1 00             p3_1:   RBP     0
****: D7                        NOP
; Procedure 3 Attribute Table:
****: 04 00                     .word   4               ; data size
****: 00 00                     .word   0               ; param size
****: 07 00                     .word   $ - p3_1        ; exit ic
****: 18 00                     .word   $ - p3_0        ; enter ic
****: 03                p3_at:  .byte   3               ; procedure number
****: 00                        .byte   0               ; lex level
;
; Procedure 4:
****: 4C                p4_0:   SLDC    76              ; 'L'
****: B8 01 01                  STR     1, 1
****: 44                        SLDC    68              ; 'D'
****: B8 01 02                  STR     1, 2
****: 42                        SLDC    66              ; 'B'
****: B8 01 03                  STR     1, 3
****: 2B                        SLDC    43              ; '+'
****: B8 01 04                  STR     1, 4
****: 53                        SLDC    83              ; 'S'
****: B8 01 05                  STR     1, 5
****: 54                        SLDC    84              ; 'T'
****: B8 01 06                  STR     1, 6
****: 42                        SLDC    66              ; 'B'
****: B8 01 07                  STR     1, 7
****: 3A                        SLDC    58              ; ':'
****: B8 01 08                  STR     1, 8
****: 01                        SLDC    1
****: B2 01 01                  LDA     1, 1
****: 00                        SLDC    0
****: 10                        SLDC    16
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 9E 06                     CSP     UNITWRITE
****: C7 73 5A                  LDCI    23155
****: AB 02                     SRO     2
****: C7 34 12                  LDCI    4660
****: AB 03                     SRO     3
****: 00                        SLDC    0
****: AB 01                     SRO     1
****: 00                        SLDC    0
****: AB 04                     SRO     4
****: B6 01 0A                  LOD     1, 10
****: A1 58                     FJP     p4_3
****: A5 02                     LAO     2
****: 00                        SLDC    0
****: BE                        LDB
****: 73                        SLDC    115             ; 's'
****: D3 04                     EFJ     p4_4
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_5
****: C2 02             p4_4:   CBP     2
****: A5 02             p4_5:   LAO     2
****: 01                        SLDC    1
****: BE                        LDB
****: 5A                        SLDC    90              ; 'Z'
****: D3 04                     EFJ     p4_6
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_7
****: C2 02             p4_6:   CBP     2
****: A5 02             p4_7:   LAO     2
****: 02                        SLDC    2
****: BE                        LDB
****: 34                        SLDC    52              ; '4'
****: D3 04                     EFJ     p4_8
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_9
****: C2 02             p4_8:   CBP     2
****: A5 02             p4_9:   LAO     2
****: 03                        SLDC    3
****: BE                        LDB
****: 12                        SLDC    18
****: D3 04                     EFJ     p4_10
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_11
****: C2 02             p4_10:  CBP     2
****: A5 02             p4_11:  LAO     2
****: 00                        SLDC    0
****: 14                        SLDC    20
****: BF                        STB
****: E9                        SLDO    2
****: C7 14 5A                  LDCI    23060
****: D3 04                     EFJ     p4_12
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_13
****: C2 02             p4_12:  CBP     2
****: A5 02             p4_13:  LAO     2
****: 03                        SLDC    3
****: 2A                        SLDC    42              ; '*'
****: BF                        STB
****: EA                        SLDO    3
****: C7 34 2A                  LDCI    10804
****: D3 04                     EFJ     p4_14
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_15
****: C2 02             p4_14:  CBP     2
****: B9 56             p4_15:  UJP     p4_16
****: A5 02             p4_3:   LAO     2
****: 00                        SLDC    0
****: BE                        LDB
****: 5A                        SLDC    90              ; 'Z'
****: D3 04                     EFJ     p4_17
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_18
****: C2 02             p4_17:  CBP     2
****: A5 02             p4_18:  LAO     2
****: 01                        SLDC    1
****: BE                        LDB
****: 73                        SLDC    115             ; 's'
****: D3 04                     EFJ     p4_19
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_20
****: C2 02             p4_19:  CBP     2
****: A5 02             p4_20:  LAO     2
****: 02                        SLDC    2
****: BE                        LDB
****: 12                        SLDC    18
****: D3 04                     EFJ     p4_21
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_22
****: C2 02             p4_21:  CBP     2
****: A5 02             p4_22:  LAO     2
****: 03                        SLDC    3
****: BE                        LDB
****: 34                        SLDC    52              ; '4'
****: D3 04                     EFJ     p4_23
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_24
****: C2 02             p4_23:  CBP     2
****: A5 02             p4_24:  LAO     2
****: 00                        SLDC    0
****: 14                        SLDC    20
****: BF                        STB
****: E9                        SLDO    2
****: C7 73 14                  LDCI    5235
****: D3 04                     EFJ     p4_25
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_26
****: C2 02             p4_25:  CBP     2
****: A5 02             p4_26:  LAO     2
****: 03                        SLDC    3
****: 2A                        SLDC    42              ; '*'
****: BF                        STB
****: EA                        SLDO    3
****: C7 2A 12                  LDCI    4650
****: D3 04                     EFJ     p4_27
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_16
****: C2 02             p4_27:  CBP     2
****: E8                p4_16:  SLDO    1
****: 00                        SLDC    0
****: D3 04                     EFJ     p4_28
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_29
****: C2 02             p4_28:  CBP     2
****: EB                p4_29:  SLDO    4
****: 00                        SLDC    0
****: D3 04                     EFJ     p4_30
****: C2 03                     CBP     3
****: B9 02                     UJP     p4_31
****: C2 02             p4_30:  CBP     2
****: 01                p4_31:  SLDC    1
****: B2 01 0B                  LDA     1, 11
****: 00                        SLDC    0
****: 02                        SLDC    2
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 9E 06                     CSP     UNITWRITE
****: C1 00             p4_1:   RBP     0
****: D7                        NOP
; Procedure 4 Attribute Table:
****: 08 00                     .word   8               ; data size
****: 00 00                     .word   0               ; param size
****: 07 00                     .word   $ - p4_1        ; exit ic
****: 14 01                     .word   $ - p4_0        ; enter ic
****: 04                p4_at:  .byte   4               ; procedure number
****: 00                        .byte   0               ; lex level
;
; Procedure 5:
****: 01                p5_0:   SLDC    1
****: AB 01                     SRO     1
****: 01                        SLDC    1
****: A5 01                     LAO     1
****: 00                        SLDC    0
****: BE                        LDB
****: C3                        EQUI
****: B8 01 0A                  STR     1, 10
****: C1 00             p5_1:   RBP     0
; Procedure 5 Attribute Table:
****: 02 00                     .word   2               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p5_1        ; exit ic
****: 14 00                     .word   $ - p5_0        ; enter ic
****: 05                p5_at:  .byte   5               ; procedure number
****: 00                        .byte   0               ; lex level
;
; Procedure 1:
****: C2 05             p1_0:   CBP     5
****: 00                        SLDC    0
****: CC 09                     STL     9
****: C2 04                     CBP     4
****: E0                        SLDL    9
****: 00                        SLDC    0
****: D4 18                     NFJ     p1_1
****: 6F                        SLDC    111             ; 'o'
****: CC 01                     STL     1
****: 6F                        SLDC    111             ; 'o'
****: CC 02                     STL     2
****: 70                        SLDC    112             ; 'p'
****: CC 03                     STL     3
****: 73                        SLDC    115             ; 's'
****: CC 04                     STL     4
****: 0D                        SLDC    13
****: CC 05                     STL     5
****: 01                        SLDC    1
****: C6 01                     LLA     1
****: 00                        SLDC    0
****: 0A                        SLDC    10
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 9E 06                     CSP     UNITWRITE
****: D6                p1_1:   XIT
; Procedure 1 Attribute Table:
****: 16 00                     .word   22              ; data size
****: 00 00                     .word   0               ; param size
****: 05 00                     .word   $ - p1_1        ; exit ic
****: 2A 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: FF                        .byte   -1              ; lex level

;
; Procedure dictionary:
;
****: 30 00                     .word   $ - p5_at       ; procedure 5
****: 4A 00                     .word   $ - p4_at       ; procedure 4
****: 64 01                     .word   $ - p3_at       ; procedure 3
****: 82 01                     .word   $ - p2_at       ; procedure 2
****: 0A 00                     .word   $ - p1_at       ; procedure 1
****: 00                        .byte   0               ; segment number
****: 05                        .byte   5               ; number of procedures
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
