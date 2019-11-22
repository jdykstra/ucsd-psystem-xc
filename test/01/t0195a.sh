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

TEST_SUBJECT="record type comparisons"
. test_prelude

cat > example.text << 'fubar'
PROGRAM example;

CONST
  MAXLEVEL = 8;
  MAXADDR = 28000;
  INTSIZE = 1;
  BITSPERWD = 16;
  STRGLGTH = 255;
  MAXSEG = 15;
  MAXPROCNUM = 149;

TYPE
  CSTCLASS = (REEL, PSET, STRG, TRIX, LONG);
  CSP = ^ CONST_REC;
  CONST_REC =
    RECORD
      CASE CCLASS: CSTCLASS OF
      LONG:
      (
        LLENG, LLAST: INTEGER;
        LONGVAL: ARRAY[1..9] OF INTEGER
      );
      TRIX:
      (
        CSTVAL: ARRAY [1..8] OF INTEGER
      );
      REEL:
      (
        RVAL: REAL
      );
      PSET:
      (
        PVAL: SET OF 0..127
      );
      STRG:
      (
        SLGTH: 0..STRGLGTH;
        SVAL: PACKED ARRAY [1..STRGLGTH] OF CHAR
      )
    END;

  VALU =
    RECORD
      CASE BOOLEAN OF
      TRUE:
      (
        IVAL: INTEGER
      );
      FALSE:
      (
        VALP: CSP
      )
    END;

  BITRANGE = 0..BITSPERWD;
  PROCRANGE = 0..MAXPROCNUM;
  LEVRANGE = 0..MAXLEVEL;
  ADDRRANGE = 0..MAXADDR;
  SEGRANGE = 0..MAXSEG;

  STRUCT_FORM =
    (SCALAR, SUBRANGE, POINTER, LONGINT, POWER, ARRAYS, RECORDS, FILES,
    TAGFLD, VARIANT);

  DECLKIND = (STANDARD, DECLARED, SPECIAL);

  STP = ^ STRUCTURE;
  CTP = ^ IDENTIFIER;

  STRUCTURE =
    RECORD
      SIZE: ADDRRANGE;
      CASE FORM: STRUCT_FORM OF
      SCALAR:
      (
        CASE SCALKIND: DECLKIND OF
        DECLARED:
        (
          FCONST: CTP
        )
      );
      SUBRANGE:
      (
        RANGETYPE: STP;
        MIN, MAX: VALU
      );
      POINTER:
      (
        ELTYPE: STP
      );
      POWER:
      (
        ELSET: STP
      );
      ARRAYS:
      (
        AELTYPE, INXTYPE: STP;
        CASE AISPACKD:BOOLEAN OF
        TRUE:
        (
          ELSPERWD, ELWIDTH: BITRANGE;
          CASE AISSTRNG: BOOLEAN OF
          TRUE:
          (
            MAXLENG: 1..STRGLGTH
          )
        )
      );
      RECORDS:
      (
        FSTFLD: CTP;
        RECVAR: STP
      );
      FILES:
      (
        FILTYPE: STP
      );
      TAGFLD:
      (
        TAGFIELDP: CTP;
        FSTVAR: STP
      );
      VARIANT:
      (
        NXTVAR, SUBVAR: STP;
        VARVAL: VALU
      )
    END;

  IDCLASS = (TYPES, KONST, FORMALVARS, ACTUALVARS, FIELD, PROC, FUNC, MODULE);
  SETOFIDS = SET OF IDCLASS;
  IDKIND = (ACTUAL, FORMAL);
  ALPHA = PACKED ARRAY [1..8] OF CHAR;

  IDENTIFIER =
    RECORD
      NAME: ALPHA;
      LLINK, RLINK: CTP;
      IDTYPE: STP;
      NEXT: CTP;
      CASE KLASS: IDCLASS OF
      KONST:
      (
        VALUES: VALU
      );
      FORMALVARS,
      ACTUALVARS:
      (
        VLEV: LEVRANGE;
        VADDR: ADDRRANGE;
        CASE BOOLEAN OF
        TRUE:
        (
          PUBLIC: BOOLEAN
        )
      );
      FIELD:
      (
        FLDADDR: ADDRRANGE;
        CASE FISPACKD: BOOLEAN OF
        TRUE:
        (
          FLDRBIT, FLDWIDTH: BITRANGE
        )
      );
      PROC,
      FUNC:
      (
        CASE PFDECKIND: DECLKIND OF
        SPECIAL:
        (
          KEY: INTEGER
        );
        STANDARD:
        (
          CSPNUM: INTEGER
        );
        DECLARED:
        (
          PFLEV: LEVRANGE;
          PFNAME: PROCRANGE;
          PFSEG: SEGRANGE;
          CASE PFKIND: IDKIND OF
          ACTUAL:
          (
            LOCALLC: ADDRRANGE;
            FORWDECL: BOOLEAN;
            EXTURNAL: BOOLEAN;
            INSCOPE: BOOLEAN;
            CASE BOOLEAN OF
            TRUE:
            (
              IMPORTED:BOOLEAN
            )
          )
        )
      );
      MODULE:
      (
        SEGID: INTEGER
      )
    END;

VAR
  INTPTR: STP;

  PROCEDURE COMPINIT;
    PROCEDURE ENTSTDTYPES;
    BEGIN
      NEW(INTPTR, SCALAR, STANDARD);
      WITH INTPTR^ DO
        BEGIN
          SIZE := INTSIZE;
          FORM := SCALAR;
          SCALKIND := STANDARD
        END
    END;
  BEGIN
    ENTSTDTYPES
  END;
BEGIN
  compinit
END.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    66  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 3:
****: A5 03             p3_0:   LAO     3
****: 03                        SLDC    3
****: 9E 01                     CSP     NEW
****: EA                        SLDO    3
****: 01                        SLDC    1
****: 9A                        STO
****: EA                        SLDO    3
****: A2 01                     INC     1
****: 00                        SLDC    0
****: 9A                        STO
****: EA                        SLDO    3
****: A2 02                     INC     2
****: 00                        SLDC    0
****: 9A                        STO
****: AD 00             p3_1:   RNP     0
; Procedure 3 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p3_1        ; exit ic
****: 1A 00                     .word   $ - p3_0        ; enter ic
****: 03                p3_at:  .byte   3               ; procedure number
****: 02                        .byte   2               ; lex level
;
; Procedure 2:
****: CE 03             p2_0:   CLP     3
****: AD 00             p2_1:   RNP     0
; Procedure 2 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p2_1        ; exit ic
****: 0A 00                     .word   $ - p2_0        ; enter ic
****: 02                p2_at:  .byte   2               ; procedure number
****: 01                        .byte   1               ; lex level
;
; Procedure 1:
****: CE 02             p1_0:   CLP     2
****: C1 00             p1_1:   RBP     0
; Procedure 1 Attribute Table:
****: 02 00                     .word   2               ; data size
****: 04 00                     .word   4               ; param size
****: 06 00                     .word   $ - p1_1        ; exit ic
****: 0A 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 00                        .byte   0               ; lex level

;
; Procedure dictionary:
;
****: 1E 00                     .word   $ - p3_at       ; procedure 3
****: 12 00                     .word   $ - p2_at       ; procedure 2
****: 06 00                     .word   $ - p1_at       ; procedure 1
****: 01                        .byte   1               ; segment number
****: 03                        .byte   3               ; number of procedures
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
