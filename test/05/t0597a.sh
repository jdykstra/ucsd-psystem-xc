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

TEST_SUBJECT="NEW"
. test_prelude

cat > example.text << 'fubar'
program example;

CONST
     MAXUNIT = 12;      (*MAXIMUM PHYSICAL UNIT # FOR UREAD*)
     MAXDIR = 77;       (*MAX NUMBER OF ENTRIES IN A DIRECTORY*)
     VIDLENG = 7;       (*NUMBER OF CHARS IN A VOLUME ID*)
     TIDLENG = 15;      (*NUMBER OF CHARS IN TITLE ID*)
     MAX_SEG = 31;      (*MAX CODE SEGMENT NUMBER*)
     FBLKSIZE = 512;    (*STANDARD DISK BLOCK LENGTH*)
     DIRBLK = 2;        (*DISK ADDR OF DIRECTORY*)
     EOL = 13;          (*END-OF-LINE...ASCII CR*)
     DLE = 16;          (*BLANK COMPRESSION CODE*)
     NAME_LEN = 23;     {Length of CONCAT(VIDLENG,':',TIDLENG)}

TYPE
     DATEREC = PACKED RECORD
                 MONTH: 0..12;
                 DAY: 0..31;
                 YEAR: 0..100
               END;

     UNITNUM = 0..MAXUNIT;
     VID = STRING[VIDLENG];

     DIRRANGE = 0..MAXDIR;
     TID = STRING[TIDLENG];
     FULL_ID = STRING[NAME_LEN];

     FILEKIND = (UNTYPEDFILE,XDSKFILE,CODEFILE,TEXTFILE,
                 INFOFILE,DATAFILE,GRAFFILE,FOTOFILE,SECUREDIR);

     DIRENTRY = PACKED RECORD
                  DFIRSTBLK: INTEGER;   (*FIRST PHYSICAL DISK ADDR*)
                  DLASTBLK: INTEGER;    (*POINTS AT BLOCK FOLLOWING*)
                  CASE DFKIND: FILEKIND OF
                    SECUREDIR,
                    UNTYPEDFILE: (*ONLY IN DIR[0]...VOLUME INFO*)
                       (FILLER1: 0..2048; {for downward compatibility,13 bits}
                        DVID: VID;              (*NAME OF DISK VOLUME*)
                        DEOVBLK: INTEGER;       (*LASTBLK OF VOLUME*)
                        DNUMFILES: DIRRANGE;    (*NUM FILES IN DIR*)
                        DLOADTIME: INTEGER;     (*TIME OF LAST ACCESS*)
                        DLASTBOOT: DATEREC);    (*MOST RECENT DATE SETTING*)
                    XDSKFILE,CODEFILE,TEXTFILE,INFOFILE,
                    DATAFILE,GRAFFILE,FOTOFILE:
                       (FILLER2: 0..1024; {for downward compatibility}
                        STATUS: BOOLEAN;        {for FILER wildcards}
                        DTID: TID;              (*TITLE OF FILE*)

                        (* DLASTBYTE: 1..FBLKSIZE; ... old declaration
                        happens too often for packed accesses to be efficient *)
                        DLASTBYTE: integer; (*NUM BYTES IN LAST BLOCK*)

                        DACCESS: DATEREC)       (*LAST MODIFICATION DATE*)
                END (*DIRENTRY*) ;

     WINDOWP = ^WINDOW;
     WINDOW = PACKED ARRAY [0..0] OF CHAR;
     FIBP = ^FIB;

     FIB = RECORD
             FWINDOW: WINDOWP;  (*USER WINDOW...F^, USED BY GET-PUT*)
             FEOF,FEOLN: BOOLEAN;
             FSTATE: (FJANDW,FNEEDCHAR,FGOTCHAR);
             FRECSIZE: INTEGER; (*IN BYTES...0=>BLOCKFILE, 1=>CHARFILE*)
             CASE FISOPEN: BOOLEAN OF
               TRUE: (FISBLKD: BOOLEAN; (*FILE IS ON BLOCK DEVICE*)
                      FUNIT: UNITNUM;   (*PHYSICAL UNIT #*)
                      FVID: VID;        (*VOLUME NAME*)
                      FREPTCNT,         (* # TIMES F^ VALID W/O GET*)
                      FNXTBLK,          (*NEXT REL BLOCK TO IO*)
                      FMAXBLK: INTEGER; (*MAX REL BLOCK ACCESSED*)
                      FMODIFIED:BOOLEAN;(*PLEASE SET NEW DATE IN CLOSE*)
                      FHEADER: DIRENTRY;(*COPY OF DISK DIR ENTRY*)
                      CASE FSOFTBUF: BOOLEAN OF (*DISK GET-PUT STUFF*)
                        TRUE: (FNXTBYTE,FMAXBYTE: INTEGER;
                               FBUFCHNGD: BOOLEAN;
                               FBUFFER: PACKED ARRAY [0..FBLKSIZE] OF CHAR))
           END (*FIB*) ;

var f: fibp;

begin
  new(f, true, false);
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    22  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: A5 03             p1_0:   LAO     3
****: 1E                        SLDC    30
****: 9E 01                     CSP     NEW
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 02 00                     .word   2               ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 0E 00                     .word   $ - p1_0        ; enter ic
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
