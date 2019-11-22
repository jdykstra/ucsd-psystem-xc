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

TEST_SUBJECT="forward type reference"
. test_prelude

#
# forward type reference
#
cat > test.text << 'fubar'
program fwd_type_decl;
const
    maxdir = 77;
    fblksize = 512;
    VIDLENG = 7;
    TIDLENG = 15;
type
    daterec =
        packed record
            month: 0..12;
            day: 0..31;
            year: 0..100
        end;
    dirrange =
        0..maxdir;
    FILEKIND =
        (UNTYPEDFILE, XDSKFILE, CODEFILE, TEXTFILE, INFOFILE, DATAFILE,
        GRAFFILE, FOTOFILE, SECUREDIR);
    VID = STRING[VIDLENG];
    TID = STRING[TIDLENG];
    direntry =
        packed record
            dfirstblk: integer;
            dlastblk: integer;
            case dfkind: filekind of
            securedir,
            untypedfile: (
                filler1: 0..2048;
                dvid: vid;
                deovblk: integer;
                dnumfiles: dirrange;
                dloadtime: integer;
                dlastboot: daterec);
            xdskfile,
            codefile,
            textfile,
            infofile,
            datafile,
            graffile,
            fotofile: (
                filler2: 0..1024;
                status: boolean;
                dtid: tid;
                dlastbyte: 1..fblksize;
                daccess: daterec)
        end;
    dirp =
        ^directory;
    directory =
        array[dirrange] of direntry;
var
    x: dirp;
begin
    x^[1].dfirstblk := 66
end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
; Disassembly of "test.code"
;
; Code  Code   Segment   Segment  Seg Machine   System
; Addr  Leng   Name      Kind     Num Type      Version
; ----- -----  --------  -------- --- --------- --------
;     1    22  FWDTYPED  LINKED     1 P-Code LE 3.0
;
; Procedure 1:
****: EA                p1_0:   SLDO    3
****: A2 0D                     INC     13
****: 42                        SLDC    66              ; 'B'
****: 9A                        STO
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

$bindir/ucsdpsys_compile test.text -o test.code
test $? -eq 0 || fail

$bindir/ucsdpsys_disassemble -a test.code > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
