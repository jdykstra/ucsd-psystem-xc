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

TEST_SUBJECT="FILE record size"
. test_prelude

#
# These are essential data types used by the system.pascal runtime.
# If their size is disturbed, everything breaks.
# In particular, sizeof(fib) is sizeof(file)
#
# Extracted from the disassembly...
#     sizeof(daterec)            2
#     sizeof(direntry)          26
#     sizeof(fib)              580
#
# The compiler thinks that FILE types are 600 bytes.
# This leaves some flexibility for later.
#
cat > example.text << 'fubar'
program example;

const
  maxunit = 12;
  vidleng = 7;
  fblksize = 512;
  tidleng = 15;
  maxdir = 77;

type
  unitnum = 0..maxunit;
  vid = string[vidleng];
  tid = string[tidleng];
  windowp = ^window;
  window = packed array [0..0] of char;
  dirrange = 0..maxdir;

  filekind =
    (
      untypedfile,
      xdskfile,
      codefile,
      textfile,
      infofile,
      datafile,
      graffile,
      fotofile,
      securedir
    );

  daterec =
    packed record
      month: 0..12;          (*0 implies date not meaningful*)
      day: 0..31;            (*day of month*)
      year: 0..100           (*100 is temp disk flag*)
    end;


  direntry =
    packed record
      dfirstblk: integer;   (*first physical disk addr*)
      dlastblk: integer;    (*points at block following*)
      case dfkind: filekind of
        securedir,
        untypedfile:
          (
            (*only in dir[0]...volume info*)
            filler1 : 0..2048;      (*for downward compatibility,13 bits*)
            dvid: vid;              (*name of disk volume*)
            deovblk: integer;       (*lastblk of volume*)
            dnumfiles: dirrange;    (*num files in dir*)
            dloadtime: integer;     (*time of last access*)
            dlastboot: daterec      (*most recent date setting*)
          );
        xdskfile,
        codefile,
        textfile,
        infofile,
        datafile,
        graffile,
        fotofile:
          (
            filler2: 0..1024;       (*for downward compatibility*)
            status: boolean;        (*for filer wildcards*)
            dtid: tid;              (*title of file*)
            dlastbyte: 1..fblksize; (*num bytes in last block*)
            daccess: daterec        (*last modification date*)
          )
    end;

  fib =
    record
      fwindow: windowp;  (*user window...f^, used by get-put*)
      feof,feoln: boolean;
      fstate: (fjandw,fneedchar,fgotchar);
      frecsize: integer; (*in bytes...0=>blockfile, 1=>charfile*)
      case fisopen: boolean of
      true: (
        fisblkd: boolean; (*file is on block device*)
        funit: unitnum;   (*physical unit #*)
        fvid: vid;        (*volume name*)
        freptcnt,         (* # times f^ valid w/o get*)
        fnxtblk,          (*next rel block to io*)
        fmaxblk: integer; (*max rel block accessed*)
        fmodified:boolean;(*please set new date in close*)
        fheader: direntry;(*copy of disk dir entry*)
        case fsoftbuf: boolean of
        true: (
          (*disk get-put stuff*)
          fnxtbyte,fmaxbyte: integer;
          fbufchngd: boolean;
          fbuffer: packed array [0..fblksize] of char
          )
        )
    end;

begin
  writeln('sizeof(daterec) = ', sizeof(daterec));
  writeln('sizeof(direntry) = ', sizeof(direntry));
  writeln('sizeof(fib) = ', sizeof(fib))
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "example.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1   158  EXAMPLE   LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: B6 01 03          p1_0:   LOD     1, 3
****: A6 12 73 69 7A            LSA     18, "sizeof(daterec) = "
      65 6F 66 28 64
      61 74 65 72 65
      63 29 20 3D 20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: 02                        SLDC    2
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: D7                        NOP
****: A6 13 73 69 7A            LSA     19, "sizeof(direntry) = "
      65 6F 66 28 64
      69 72 65 6E 74
      72 79 29 20 3D
      20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: 1A                        SLDC    26
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: A6 0E 73 69 7A            LSA     14, "sizeof(fib) = "
      65 6F 66 28 66
      69 62 29 20 3D
      20
****: 00                        SLDC    0
****: CD 00 13                  CXP     0, 19           ; fwritestring
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: C7 44 02                  LDCI    580
****: 00                        SLDC    0
****: CD 00 0D                  CXP     0, 13           ; fwriteint
****: 9E 00                     CSP     IOCHECK
****: B6 01 03                  LOD     1, 3
****: CD 00 16                  CXP     0, 22           ; fwriteln
****: 9E 00                     CSP     IOCHECK
****: C1 00             p1_1:   RBP     0
****: D7                        NOP
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 04 00                     .word   4               ; param size
****: 07 00                     .word   $ - p1_1        ; exit ic
****: 96 00                     .word   $ - p1_0        ; enter ic
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

ucsdpsys_compile example.text -o example.code
test $? -eq 0 || fail

ucsdpsys_disassemble -a example.code > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
