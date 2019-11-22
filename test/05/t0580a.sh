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

TEST_SUBJECT="parameters vs set sizes"
. test_prelude

cat > example.text << 'fubar'
program example;

  procedure chk(b: boolean);
  begin
    write(': ');
    if b then
      write('ok')
    else
      write('wrong');
    writeln;
  end;

  PROCEDURE EQUPOWRNEQPOWR;

    type set40 = set of 0..39;

    procedure eq(a, b: set40; expect: boolean);
    begin
      chk((a = b) = expect);
    end;

    procedure ne(a, b: set40; expect: boolean);
    begin
      chk((a <> b) = expect);
    end;

  BEGIN
    write('1. EQUPOWR [0, 9, 13, 17] = [0, 9, 13, 17]');
    eq([0, 9, 13, 17], [0, 9, 13, 17], true);

    write('2. EQUPOWR [0, 9, 13, 17, 34] = [0, 9, 13, 17]');
    eq([0, 9, 13, 17], [0, 9, 13, 17, 34], false);

    write('3. EQUPOWR [0, 9, 13, 17, 34] = [0, 9, 13, 17]');
    eq([0, 9, 13, 17, 34], [0, 9, 13, 17], false);

    write('4. EQUPOWR [0] = [1]');
    eq([0], [1], false);

    write('5. EQUPOWR [] = []');
    eq([], [], true);

    write('6. EQUPOWR [] = [3]');
    eq([], [3], false);

    write('7. EQUPOWR [5] = []');
    eq([5], [], false);

    write('8. EQUPOWR [1, 7, 10] = [17, 23, 26]');
    eq([1, 7, 10], [17, 23, 26], false);

    write('9. EQUPOWR [17, 23, 26] = [1, 7, 10]');
    eq([17, 23, 26], [1, 7, 10], false);

    write('10. NEQPOWR [0, 9, 13, 17] <> [0, 9, 13, 17]');
    ne([0, 9, 13, 17], [0, 9, 13, 17], false);

    write('11. NEQPOWR [0, 9, 13, 17] <> [0, 9, 13, 17, 34]');
    ne([0, 9, 13, 17], [0, 9, 13, 17, 34], true);

    write('12. NEQPOWR [0, 9, 13, 17, 34] <> [0, 9, 13, 17]');
    ne([0, 9, 13, 17, 34], [0, 9, 13, 17], true);

    write('13. NEQPOWR [0] <> [1]');
    ne([0], [1], true);

    write('14. NEQPOWR [] <> []');
    ne([], [], false);

    write('15. NEQPOWR [] <> [3]');
    ne([], [3], true);

    write('16. NEQPOWR [5] <> []');
    ne([5], [], true);

    write('17. NEQPOWR [1, 7, 10] <> [17, 23, 26]');
    ne([1, 7, 10], [17, 23, 26], true);

    write('18. NEQPOWR [17, 23, 26] <> [1, 7, 10]');
    ne([17, 23, 26], [1, 7, 10], true);
  END;

begin
  EQUPOWRNEQPOWR;
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1  1152  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 2:
****: B6 02 03          p2_0:   LOD     2, 3
****: A6 02 3A 20               LSA     2, ": "
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: D8                        SLDL    1
****: A1 0F                     FJP     p2_2
****: B6 02 03                  LOD     2, 3
****: A6 02 6F 6B               LSA     2, "ok"
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B9 11                     UJP     p2_3
****: B6 02 03          p2_2:   LOD     2, 3
****: D7                        NOP
****: A6 05 77 72 6F            LSA     5, "wrong"
      6E 67
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 02 03          p2_3:   LOD     2, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: AD 00             p2_1:   RNP     0
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 02 00                     .word   2               ; param size
****: 06 00                     .word   $ - p2_1        ; exit ic
****: 40 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 4:
****: C6 05             p4_0:   LLA     5
****: BC 03                     LDM     3
****: 03                        SLDC    3
****: C6 02                     LLA     2
****: BC 03                     LDM     3
****: 03                        SLDC    3
****: AF 08                     EQU     POWR
****: D8                        SLDL    1
****: AF 06                     EQU     BOOL
****: CF 02                     CGP     2
****: AD 00             p4_1:   RNP     0
****: D7                        NOP
; Procedure 4 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 0E 00                     .word   14              ; param size
****: 07 00                     .word   $ - p4_1        ; exit ic
****: 1A 00                     .word   $ - p4_0        ; enter ic
****: 04                p4_at:  .byte   4               ; procedure number
****: 02                        .byte   2               ; lex level
;
; Procedure 5:
****: C6 05             p5_0:   LLA     5
****: BC 03                     LDM     3
****: 03                        SLDC    3
****: C6 02                     LLA     2
****: BC 03                     LDM     3
****: 03                        SLDC    3
****: B7 08                     NEQ     POWR
****: D8                        SLDL    1
****: AF 06                     EQU     BOOL
****: CF 02                     CGP     2
****: AD 00             p5_1:   RNP     0
****: D7                        NOP
; Procedure 5 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 0E 00                     .word   14              ; param size
****: 07 00                     .word   $ - p5_1        ; exit ic
****: 1A 00                     .word   $ - p5_0        ; enter ic
****: 05                p5_at:  .byte   5               ; procedure number
****: 02                        .byte   2               ; lex level
;
; Procedure 3:
****: B6 02 03          p3_0:   LOD     2, 3
****: A6 2A 31 2E 20            LSA     42, "1. EQUPOWR [0, 9, 13, 17] = [0, 9,
      45 51 55 50 4F                    13, 17]"
      57 52 20 5B 30
      2C 20 39 2C 20
      31 33 2C 20 31
      37 5D 20 3D 20
      5B 30 2C 20 39
      2C 20 31 33 2C
      20 31 37 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 00                        SLDC    0
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 01                        SLDC    1
****: CE 04                     CLP     4
****: B6 02 03                  LOD     2, 3
****: A6 2E 32 2E 20            LSA     46, "2. EQUPOWR [0, 9, 13, 17, 34] = [0,
      45 51 55 50 4F                    9, 13, 17]"
      57 52 20 5B 30
      2C 20 39 2C 20
      31 33 2C 20 31
      37 2C 20 33 34
      5D 20 3D 20 5B
      30 2C 20 39 2C
      20 31 33 2C 20
      31 37 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 04                        SLDC    4
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 00                        SLDC    0
****: CE 04                     CLP     4
****: B6 02 03                  LOD     2, 3
****: A6 2E 33 2E 20            LSA     46, "3. EQUPOWR [0, 9, 13, 17, 34] = [0,
      45 51 55 50 4F                    9, 13, 17]"
      57 52 20 5B 30
      2C 20 39 2C 20
      31 33 2C 20 31
      37 2C 20 33 34
      5D 20 3D 20 5B
      30 2C 20 39 2C
      20 31 33 2C 20
      31 37 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 04                        SLDC    4
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 00                        SLDC    0
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 00                        SLDC    0
****: CE 04                     CLP     4
****: B6 02 03                  LOD     2, 3
****: A6 14 34 2E 20            LSA     20, "4. EQUPOWR [0] = [1]"
      45 51 55 50 4F
      57 52 20 5B 30
      5D 20 3D 20 5B
      31 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 01                        SLDC    1
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 02                        SLDC    2
****: 00                        SLDC    0
****: CE 04                     CLP     4
****: B6 02 03                  LOD     2, 3
****: A6 12 35 2E 20            LSA     18, "5. EQUPOWR [] = []"
      45 51 55 50 4F
      57 52 20 5B 5D
      20 3D 20 5B 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 01                        SLDC    1
****: CE 04                     CLP     4
****: B6 02 03                  LOD     2, 3
****: A6 13 36 2E 20            LSA     19, "6. EQUPOWR [] = [3]"
      45 51 55 50 4F
      57 52 20 5B 5D
      20 3D 20 5B 33
      5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 08                        SLDC    8
****: 00                        SLDC    0
****: CE 04                     CLP     4
****: B6 02 03                  LOD     2, 3
****: D7                        NOP
****: A6 13 37 2E 20            LSA     19, "7. EQUPOWR [5] = []"
      45 51 55 50 4F
      57 52 20 5B 35
      5D 20 3D 20 5B
      5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 20                        SLDC    32              ; ' '
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CE 04                     CLP     4
****: B6 02 03                  LOD     2, 3
****: D7                        NOP
****: A6 24 38 2E 20            LSA     36, "8. EQUPOWR [1, 7, 10] = [17, 23,
      45 51 55 50 4F                    26]"
      57 52 20 5B 31
      2C 20 37 2C 20
      31 30 5D 20 3D
      20 5B 31 37 2C
      20 32 33 2C 20
      32 36 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: C7 82 04                  LDCI    1154
****: 00                        SLDC    0
****: C7 82 04                  LDCI    1154
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CE 04                     CLP     4
****: B6 02 03                  LOD     2, 3
****: A6 24 39 2E 20            LSA     36, "9. EQUPOWR [17, 23, 26] = [1, 7,
      45 51 55 50 4F                    10]"
      57 52 20 5B 31
      37 2C 20 32 33
      2C 20 32 36 5D
      20 3D 20 5B 31
      2C 20 37 2C 20
      31 30 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: C7 82 04                  LDCI    1154
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: C7 82 04                  LDCI    1154
****: 00                        SLDC    0
****: CE 04                     CLP     4
****: B6 02 03                  LOD     2, 3
****: A6 2C 31 30 2E            LSA     44, "10. NEQPOWR [0, 9, 13, 17] <> [0,
      20 4E 45 51 50                    9, 13, 17]"
      4F 57 52 20 5B
      30 2C 20 39 2C
      20 31 33 2C 20
      31 37 5D 20 3C
      3E 20 5B 30 2C
      20 39 2C 20 31
      33 2C 20 31 37
      5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 00                        SLDC    0
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 00                        SLDC    0
****: CE 05                     CLP     5
****: B6 02 03                  LOD     2, 3
****: A6 30 31 31 2E            LSA     48, "11. NEQPOWR [0, 9, 13, 17] <> [0,
      20 4E 45 51 50                    9, 13, 17, 34]"
      4F 57 52 20 5B
      30 2C 20 39 2C
      20 31 33 2C 20
      31 37 5D 20 3C
      3E 20 5B 30 2C
      20 39 2C 20 31
      33 2C 20 31 37
      2C 20 33 34 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 04                        SLDC    4
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 01                        SLDC    1
****: CE 05                     CLP     5
****: B6 02 03                  LOD     2, 3
****: A6 30 31 32 2E            LSA     48, "12. NEQPOWR [0, 9, 13, 17, 34] <>
      20 4E 45 51 50                    [0, 9, 13, 17]"
      4F 57 52 20 5B
      30 2C 20 39 2C
      20 31 33 2C 20
      31 37 2C 20 33
      34 5D 20 3C 3E
      20 5B 30 2C 20
      39 2C 20 31 33
      2C 20 31 37 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 04                        SLDC    4
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 00                        SLDC    0
****: 02                        SLDC    2
****: C7 01 22                  LDCI    8705
****: 01                        SLDC    1
****: CE 05                     CLP     5
****: B6 02 03                  LOD     2, 3
****: A6 16 31 33 2E            LSA     22, "13. NEQPOWR [0] <> [1]"
      20 4E 45 51 50
      4F 57 52 20 5B
      30 5D 20 3C 3E
      20 5B 31 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 01                        SLDC    1
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 02                        SLDC    2
****: 01                        SLDC    1
****: CE 05                     CLP     5
****: B6 02 03                  LOD     2, 3
****: A6 14 31 34 2E            LSA     20, "14. NEQPOWR [] <> []"
      20 4E 45 51 50
      4F 57 52 20 5B
      5D 20 3C 3E 20
      5B 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CE 05                     CLP     5
****: B6 02 03                  LOD     2, 3
****: A6 15 31 35 2E            LSA     21, "15. NEQPOWR [] <> [3]"
      20 4E 45 51 50
      4F 57 52 20 5B
      5D 20 3C 3E 20
      5B 33 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 08                        SLDC    8
****: 01                        SLDC    1
****: CE 05                     CLP     5
****: B6 02 03                  LOD     2, 3
****: D7                        NOP
****: A6 15 31 36 2E            LSA     21, "16. NEQPOWR [5] <> []"
      20 4E 45 51 50
      4F 57 52 20 5B
      35 5D 20 3C 3E
      20 5B 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 20                        SLDC    32              ; ' '
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 01                        SLDC    1
****: CE 05                     CLP     5
****: B6 02 03                  LOD     2, 3
****: D7                        NOP
****: A6 26 31 37 2E            LSA     38, "17. NEQPOWR [1, 7, 10] <> [17, 23,
      20 4E 45 51 50                    26]"
      4F 57 52 20 5B
      31 2C 20 37 2C
      20 31 30 5D 20
      3C 3E 20 5B 31
      37 2C 20 32 33
      2C 20 32 36 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: 00                        SLDC    0
****: C7 82 04                  LDCI    1154
****: 00                        SLDC    0
****: C7 82 04                  LDCI    1154
****: 00                        SLDC    0
****: 01                        SLDC    1
****: CE 05                     CLP     5
****: B6 02 03                  LOD     2, 3
****: A6 26 31 38 2E            LSA     38, "18. NEQPOWR [17, 23, 26] <> [1, 7,
      20 4E 45 51 50                    10]"
      4F 57 52 20 5B
      31 37 2C 20 32
      33 2C 20 32 36
      5D 20 3C 3E 20
      5B 31 2C 20 37
      2C 20 31 30 5D
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: 00                        SLDC    0
****: C7 82 04                  LDCI    1154
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 00                        SLDC    0
****: C7 82 04                  LDCI    1154
****: 01                        SLDC    1
****: CE 05                     CLP     5
****: AD 00             p3_1:   RNP     0
; Procedure 3 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p3_1        ; exit ic
****: E2 03                     .word   $ - p3_0        ; enter ic
****: 03                p3_at:  .byte   3               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: CE 03             p1_0:   CLP     3
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 0A 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: F6 03                     .word   $ - p5_at       ; procedure 5
****: 16 04                     .word   $ - p4_at       ; procedure 4
****: 14 00                     .word   $ - p3_at       ; procedure 3
****: 38 04                     .word   $ - p2_at       ; procedure 2
****: 0A 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
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
