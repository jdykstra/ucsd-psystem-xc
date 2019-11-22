#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2012 Peter Miller
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

TEST_SUBJECT="terak foto display"
. test_prelude

cat > example.text << 'fubar'
program inspect;
  type
    ink_colors = (invisible, white, eraser, complement);
    quad =  0..3;
    mark_t =
      record
        (* lettered memory array *)
        x: integer; (* both x&y=32767->invalid as x, y pair *)
        y: integer (* x=32767 but y<32767->valid number *)
      end;
    terak_screen =
      (* the picture... by four different names *)
      record case quad of
      0:
        (
          i: packed array [0..239] of packed array [0..319] of boolean
        );
      1:
        (
          c: packed array [0..9599] of char
        );
      2:
        (
          d:
            packed record
              pix: packed array [0..9599] of char;
              x, y: integer; (* cursor location *)
              marks: packed array ['a'..'z'] of mark_t;
              angle: integer; (* heading *)
              pen: ink_colors end (* penstate *)
        );
      3:
        (
          q: packed array [0..4863] of integer (* 19 blks for bulk i/o *)
        )
      end; (* terak_screen *)
  var
    x, y, angle: integer; (* coordinates of a dot *)
    pen: ink_color;
    f: file;
    screen: terak_screen; (* the graphic  universe *)
    marks: packed array ['a'..'z'] of mark_t;

begin
  openold(f, 'pixfile.foto'); (* permanent name for demo *)
  if blockread(f, screen.q[0], 19) <> 19 then
    writeln (' *file read error*')
  else
    begin
      close(f);
      unitwrite(3, screen.c[0], 63); (* turn on display *)
      x := screen.d.x; (* get x of cursor *)
      y := screen.d.y; (* get y of cursor *)
      marks := screen.d.marks; (* load mark array *)
      angle := screen.d.angle; (* load heading *)
      pen := screen.d.pen; (* load penstate *)
      readln(x)
    end
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   180  INSPECT   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: A5 07             p1_0:   LAO     7
****: 9F                        LDCN
****: 01                        SLDC    1
****: 91                        NGI
****: CD 00 03                  CXP     0, 3            ; finit
****: A5 07                     LAO     7
****: D7                        NOP
****: A6 0C 70 69 78            LSA     12, "pixfile.foto"
      66 69 6C 65 2E
      66 6F 74 6F
****: 01                        SLDC    1
****: 9F                        LDCN
****: CD 00 05                  CXP     0, 5            ; fopen
****: 9E 00                     CSP     IOCHECK
****: A5 07                     LAO     7
****: A5 2F                     LAO     47
****: 00                        SLDC    0
****: 13                        SLDC    19
****: 01                        SLDC    1
****: 91                        NGI
****: 01                        SLDC    1
****: 00                        SLDC    0
****: 00                        SLDC    0
****: CD 00 1C                  CXP     0, 28           ; fblockio
****: 9E 00                     CSP     IOCHECK
****: 13                        SLDC    19
****: D4 28                     NFJ     p1_6
****: B6 01 03                  LOD     1, 3
****: D7                        NOP
****: A6 12 20 2A 66            LSA     18, " *file read error*"
      69 6C 65 20 72
      65 61 64 20 65
      72 72 6F 72 2A
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: B9 42                     UJP     p1_1
****: A5 07             p1_6:   LAO     7
****: 00                        SLDC    0
****: CD 00 06                  CXP     0, 6            ; fclose
****: 9E 00                     CSP     IOCHECK
****: 03                        SLDC    3
****: A5 2F                     LAO     47
****: 00                        SLDC    0
****: 3F                        SLDC    63              ; '?'
****: 00                        SLDC    0
****: 00                        SLDC    0
****: 9E 06                     CSP     UNITWRITE
****: A9 92 F0                  LDO     4848
****: AB 05                     SRO     5
****: A9 92 EF                  LDO     4847
****: AB 04                     SRO     4
****: A5 93 2F                  LAO     4911
****: A5 92 F1                  LAO     4849
****: A8 34                     MOV     52
****: A9 93 25                  LDO     4901
****: AB 03                     SRO     3
****: A5 93 26                  LAO     4902
****: 02                        SLDC    2
****: 00                        SLDC    0
****: BA                        LDP
****: AB 06                     SRO     6
****: B6 01 02                  LOD     1, 2
****: A5 05                     LAO     5
****: CD 00 0C                  CXP     0, 12           ; freadint
****: 9E 00                     CSP     IOCHECK
****: B6 01 02                  LOD     1, 2
****: CD 00 15                  CXP     0, 21           ; freadln
****: 9E 00                     CSP     IOCHECK
****: A5 07             p1_1:   LAO     7
****: 00                        SLDC    0
****: CD 00 06                  CXP     0, 6            ; fclose
****: C1 00                     RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: C0 26                     .word   9920            ; data size
****: 04 00                     .word   4               ; param size
****: 0D 00                     .word   $ - p1_1        ; exit ic
****: AC 00                     .word   $ - p1_0        ; enter ic
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
