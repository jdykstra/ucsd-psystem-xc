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

TEST_SUBJECT="enum bug"
. test_prelude

cat > example.text << 'fubar'
(*$warning shadow false*)
program example;
type
  ACCESS =
    (
      DRCT,
      INDRCT,
      PACKD,
      MULTI,
      BYTE
    );
  alpha = packed array [0..7] of char;

  procedure fubar;
  TYPE
    LITYPES = (EOFMARK,MODDULE,GLOBREF,PUBBLIC,PRIVVATE,CONNSTANT,GLOBDEF,
               PUBLICDEF,CONSTDEF,EXTPROC,EXTFUNC,SSEPPROC,SSEPFUNC,
               SEPPREF,SEPFREF);
    OPFORMAT =
      (
        WORD,
        BYTE,
        BIG
      );
    LIENTRY =
      RECORD
        LINAME: ALPHA;
        CASE LITYPE: LITYPES OF
        MODDULE,
        PUBBLIC,
        PRIVVATE,
        SEPPREF,
        SEPFREF: (
          FORMAT: OPFORMAT;
          NREFS: INTEGER;
          NWORDS: INTEGER
        );
        CONSTDEF: (
          CONSTANT: INTEGER
        );
        PUBLICDEF: (
          BASEOFFSET: INTEGER
        );
        EXTPROC,
        EXTFUNC,
        SSEPPROC,
        SSEPFUNC: (
          PROCNUM: INTEGER;
          NPARAMS: INTEGER;
          RANGE: ^INTEGER
        )
      END;

  VAR
    LIREC: LIENTRY;
  begin
    with lirec do
      BEGIN
        FORMAT := BYTE;
        NWORDS := NPARAMS
      END
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
;     1    36  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 2:
****: 01                p2_0:   SLDC    1
****: CC 06                     STL     6
****: DE                        SLDL    7
****: CC 08                     STL     8
****: AD 00             p2_1:   RNP     0
; Procedure 2 Attribute Table:
****: 10 00                     .word   16              ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p2_1        ; exit ic
****: 0E 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: C1 00             p1_0:   RBP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_0        ; exit ic
****: 08 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 0E 00                     .word   $ - p2_at       ; procedure 2
****: 04 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 02                        .byte   2               ; number of procedures
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
