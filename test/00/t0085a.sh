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

TEST_SUBJECT="compile something"
. test_prelude

#
# insert your test here...
#
cat > test.text << 'fubar'
{$S+}
(*$U-*)
PROGRAM PASCALSYSTEM;

CONST
     MMAXINT = 32767;   (*MAXIMUM INTEGER VALUE*)
     MAXUNIT = 12;      (*MAXIMUM PHYSICAL UNIT # FOR UREAD*)
     MAXDIR = 77;       (*MAX NUMBER OF ENTRIES IN A DIRECTORY*)
     VIDLENG = 7;       (*NUMBER OF CHARS IN A VOLUME ID*)
     TIDLENG = 15;      (*NUMBER OF CHARS IN TITLE ID*)
     MAX_SEG = 31;      (*MAX CODE SEGMENT NUMBER*)
     FBLKSIZE = 512;    (*STANDARD DISK BLOCK LENGTH*)
     DIRBLK = 2;        (*DISK ADDR OF DIRECTORY*)
     AGELIMIT = 300;    (*MAX AGE FOR GDIRP...IN TICKS*)
     EOL = 13;          (*END-OF-LINE...ASCII CR*)
     DLE = 16;          (*BLANK COMPRESSION CODE*)
     NAME_LEN = 23;     {Length of CONCAT(VIDLENG,':',TIDLENG)}
     FILL_LEN = 11;     {Maximum # of nulls in FILLER}

TYPE

     IORSLTWD = (INOERROR,IBADBLOCK,IBADUNIT,IBADMODE,ITIMEOUT,
                 ILOSTUNIT,ILOSTFILE,IBADTITLE,INOROOM,INOUNIT,
                 INOFILE,IDUPFILE,INOTCLOSED,INOTOPEN,IBADFORMAT,
                 ISTRGOVFL);

                                        (*COMMAND STATES...SEE GETCMD*)

     CMDSTATE = (HALTINIT,DEBUGCALL,
                 UPROGNOU,UPROGUOK,SYSPROG,
                 COMPONLY,COMPANDGO,COMPDEBUG,
                 LINKANDGO,LINKDEBUG);

                                        (*CODE FILES USED IN GETCMD*)

     SYSFILE = (ASSMBLER,COMPILER,EDITOR,FILER,LINKER);

                                        (*ARCHIVAL INFO...THE DATE*)

     DATEREC = PACKED RECORD
                 MONTH: 0..12;          (*0 IMPLIES DATE NOT MEANINGFUL*)
                 DAY: 0..31;            (*DAY OF MONTH*)
                 YEAR: 0..100           (*100 IS TEMP DISK FLAG*)
               END (*DATEREC*) ;

                                        (*VOLUME TABLES*)
     UNITNUM = 0..MAXUNIT;
     VID = STRING[VIDLENG];

                                        (*DISK DIRECTORIES*)
     DIRRANGE = 0..MAXDIR;
     TID = STRING[TIDLENG];
     FULL_ID = STRING[NAME_LEN];

     FILE_TABLE = ARRAY [SYSFILE] OF FULL_ID;

     FILEKIND = (UNTYPEDFILE,XDSKFILE,CODEFILE,TEXTFILE,
                 INFOFILE,DATAFILE,GRAFFILE,FOTOFILE,SECUREDIR);

     DIRENTRY = PACKED RECORD
                  DFIRSTBLK: INTEGER;   (*FIRST PHYSICAL DISK ADDR*)
                  DLASTBLK: INTEGER;    (*POINTS AT BLOCK FOLLOWING*)
                  CASE DFKIND: FILEKIND OF
                    SECUREDIR,
                    UNTYPEDFILE: (*ONLY IN DIR[0]...VOLUME INFO*)
                       (FILLER1 : 0..2048; {for downward compatibility,13 bits}
                        DVID: VID;              (*NAME OF DISK VOLUME*)
                        DEOVBLK: INTEGER;       (*LASTBLK OF VOLUME*)
                        DNUMFILES: DIRRANGE;    (*NUM FILES IN DIR*)
                        DLOADTIME: INTEGER;     (*TIME OF LAST ACCESS*)
                        DLASTBOOT: DATEREC);    (*MOST RECENT DATE SETTING*)
                    XDSKFILE,CODEFILE,TEXTFILE,INFOFILE,
                    DATAFILE,GRAFFILE,FOTOFILE:
                       (FILLER2 : 0..1024; {for downward compatibility}
                        STATUS : BOOLEAN;        {for FILER wildcards}
                        DTID: TID;              (*TITLE OF FILE*)
                        DLASTBYTE: 1..FBLKSIZE; (*NUM BYTES IN LAST BLOCK*)
                        DACCESS: DATEREC)       (*LAST MODIFICATION DATE*)
                END (*DIRENTRY*) ;

     DIRP = ^DIRECTORY;

     DIRECTORY = ARRAY [DIRRANGE] OF DIRENTRY;

                                        (*FILE INFORMATION*)

     CLOSETYPE = (CNORMAL,CLOCK,CPURGE,CCRUNCH);
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

                                        (*USER WORKFILE STUFF*)

     INFOREC = RECORD
                 SYMFIBP,CODEFIBP: FIBP;        (*WORKFILES FOR SCRATCH*)
                 ERRSYM,ERRBLK,ERRNUM: INTEGER; (*ERROR STUFF IN EDIT*)
                 SLOWTERM,STUPID: BOOLEAN;      (*STUDENT PROGRAMMER ID!!*)
                 ALTMODE: CHAR;                 (*WASHOUT CHAR FOR COMPILER*)
                 GOTSYM,GOTCODE: BOOLEAN;       (*TITLES ARE MEANINGFUL*)
                 WORKVID,SYMVID,CODEVID: VID;   (*PERM&CUR WORKFILE VOLUMES*)
                 WORKTID,SYMTID,CODETID: TID    (*PERM&CUR WORKFILES TITLE*)
               END (*INFOREC*) ;

                                        (*CODE SEGMENT LAYOUTS*)

     SEG_RANGE = 0..MAX_SEG;
     SEG_DESC = RECORD
                 DISKADDR: INTEGER;     (*REL BLK IN CODE...ABS IN SYSCOM^*)
                 CODELENG: INTEGER      (*# BYTES TO READ IN*)
               END (*SEGDESC*) ;

                                        (*DEBUGGER STUFF*)

     BYTERANGE = 0..255;
     TRICKARRAY = RECORD        {Memory diddling for execerror}
                    CASE BOOLEAN OF
                      TRUE : (WORD : ARRAY [0..0] OF INTEGER);
                      FALSE : (BYTE : PACKED ARRAY [0..0] OF BYTERANGE)
                    END;
     MSCWP = ^ MSCW;            (*MARK STACK RECORD POINTER*)
     MSCW = RECORD
              STATLINK: MSCWP;  (*POINTER TO PARENT MSCW*)
              DYNLINK: MSCWP;   (*POINTER TO CALLER'S MSCW*)
              MSSEG,MSJTAB: ^TRICKARRAY;
              MSIPC: INTEGER;
              LOCALDATA: TRICKARRAY
            END (*MSCW*) ;

                                        (*SYSTEM COMMUNICATION AREA*)
                                        (*SEE INTERPRETERS...NOTE  *)
                                        (*THAT WE ASSUME BACKWARD  *)
                                        (*FIELD ALLOCATION IS DONE *)
     SEG_ENTRY = RECORD
                   CODEUNIT: UNITNUM;
                   CODEDESC: SEGDESC
                 END;
     SYSCOMREC = RECORD
                   IORSLT: IORSLTWD;    (*RESULT OF LAST IO CALL*)
                   XEQERR: INTEGER;     (*REASON FOR EXECERROR CALL*)
                   SYSUNIT: UNITNUM;    (*PHYSICAL UNIT OF BOOTLOAD*)
                   BUGSTATE: INTEGER;   (*DEBUGGER INFO*)
                   GDIRP: DIRP;         (*GLOBAL DIR POINTER,SEE VOLSEARCH*)
                   LASTMP,STKBASE,BOMBP: MSCWP;
                   MEMTOP,SEG,JTAB: INTEGER;
                   BOMBIPC: INTEGER;    (*WHERE XEQERR BLOWUP WAS*)
                   HLTLINE: INTEGER;    (*MORE DEBUGGER STUFF*)
                   BRKPTS: ARRAY [0..3] OF INTEGER;
                   RETRIES: INTEGER;    (*DRIVERS PUT RETRY COUNTS*)
                   EXPANSION: ARRAY [0..8] OF INTEGER;
                   HIGHTIME,LOWTIME: INTEGER;
                   MISCINFO: PACKED RECORD
                               NOBREAK,STUPID,SLOWTERM,
                               HASXYCRT,HASLCCRT,HAS8510A,HASCLOCK: BOOLEAN;
                               USERKIND:(uNORMAL, AQUIZ, BOOKER, PQUIZ);
                               WORD_MACH, IS_FLIPT : BOOLEAN
                             END;
                   CRTTYPE: INTEGER;
                   CRTCTRL: PACKED RECORD
                              RLF,NDFS,ERASEEOL,ERASEEOS,HOME,ESCAPE: CHAR;
                              BACKSPACE: CHAR;
                              FILLCOUNT: 0..255;
                              CLEARSCREEN, CLEARLINE: CHAR;
                              PREFIXED: PACKED ARRAY [0..15] OF BOOLEAN
                            END;
                   CRTINFO: PACKED RECORD
                              WIDTH,HEIGHT: INTEGER;
                              RIGHT,LEFT,DOWN,UP: CHAR;
                              BADCH,CHARDEL,STOP,BREAK,FLUSH,EOF: CHAR;
                              ALTMODE,LINEDEL: CHAR;
                              ALPHA_LOCK,ETX,PREFIX: CHAR;
                              PREFIXED: PACKED ARRAY [0..15] OF BOOLEAN
                            END;
                   SEGTABLE: ARRAY [SEG_RANGE] OF SEG_ENTRY
                 END (*SYSCOM*);

     MISCINFOREC = RECORD
                     MSYSCOM: SYSCOMREC
                   END;

VAR
    SYSCOM: ^SYSCOMREC;                 (*MAGIC PARAM...SET UP IN BOOT*)
    GFILES: ARRAY [0..5] OF FIBP;       (*GLOBAL FILES, 0=INPUT, 1=OUTPUT*)
    USERINFO: INFOREC;                  (*WORK STUFF FOR COMPILER ETC*)
    EMPTYHEAP: ^INTEGER;                (*HEAP MARK FOR MEM MANAGING*)
    INPUTFIB,OUTPUTFIB,                 (*CONSOLE FILES...GFILES ARE COPIES*)
    SYSTERM,SWAPFIB: FIBP;              (*CONTROL AND SWAPSPACE FILES*)
    SYVID,DKVID: VID;                   (*SYSUNIT VOLID & DEFAULT VOLID*)
    THEDATE: DATEREC;                   (*TODAY...SET IN FILER OR SIGN ON*)
    DEBUGINFO: ^INTEGER;                (*DEBUGGERS GLOBAL INFO WHILE RUNIN*)
    STATE: CMDSTATE;                    (*FOR GETCOMMAND*)
    PL: STRING;                         (*PROMPTLINE STRING...SEE PROMPT*)
    IPOT: ARRAY [0..4] OF INTEGER;      (*INTEGER POWERS OF TEN*)
    FILLER: STRING[FILL_LEN];           (*NULLS FOR CARRIAGE DELAY*)
    DIGITS: SET OF '0'..'9';
    UNITABLE: ARRAY [UNITNUM] OF (*0 NOT USED*)
                RECORD
                  UVID: VID;    (*VOLUME ID FOR UNIT*)
                  CASE UISBLKD: BOOLEAN OF
                    TRUE: (UEOVBLK: INTEGER)
                END (*UNITABLE*) ;
    FILENAME : FILE_TABLE;
begin
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    16  PASCALSY  LINKED     0 Undefined Pre-II.1
;
; Procedure 1:
0000: D6                p1_0:   XIT
0001: D7                        NOP
; Procedure 1 Attribute Table:
0002: 0E 02                     .word   526             ; data size
0004: 00 00                     .word   0               ; param size
0006: 06 00                     .word   $ - p1_0        ; exit ic
0008: 08 00                     .word   $ - p1_0        ; enter ic
000A: 01                p1_at:  .byte   1               ; procedure number
000B: FF                        .byte   -1              ; lex level

;
; Procedure dictionary:
;
000C: 02 00                     .word   $ - p1_at       ; procedure 1
000E: 00                        .byte   0               ; segment number
000F: 01                        .byte   1               ; number of procedures
;
; ------------------------------------------------------------------
fubar
test $? -eq 0 || no_result

$bindir/ucsdpsys_compile test.text -o test.code
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble test.code > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
