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

TEST_SUBJECT="long interface definitions"
. test_prelude

# NOTE: NOTE: NOTE:
#
# The big comment as a word that looks line the IMPLEMENTATION keyword.
# That is on purpose, so that we test the tokenizing required to locate
# the actual keyword, not just any text that looks sort-of similar.

cat > example.text << 'fubar'
(*$S+*)
unit example;
interface
  (*
   * What happens when UNIT interface text crosses a 1KiB boundary?
   *
   * ISO/IEC 10206:1990(E)
   *
   * Introduction
   *
   * This International Standard provides an unambiguous and machine
   * independent definition of the programming language Extended
   * Pascal.  Its purpose is to facilitate portability of Extended
   * Pascal programs for use on a wide variety of data processing
   * systems.
   *
   * Language history
   *
   * The computer programming language Pascal was designed by Professor
   * Niklaus Wirth to satisfy two principal aims
   *
   * a) to make available a language suitable for teaching programming
   *    as a systematic discipline based on certain fundamental
   *    concepts clearly and naturally reflected by the language; and
   *
   * b) to design a language whose implementations could be reliable
   *    and efficent on then-available computers.
   *
   * However, it has become apparent that Pascal has attributes that
   * go far beyond those original goals.  It is now being increasingly
   * used commercially in the writing of system and application
   * software.  With this increased use, there has been an increased
   * demand for and availability of extensions to ISO 7185:1983,
   * Programming languages - PASCAL.  Programs using such extensions
   * attain the benefits of the extended features at the cost of
   * portability with standard Pascal and with other processors
   * supporting different sets of extensions.  In the absence of a
   * standard for an extended language, these processors have become
   * increasingly incompatible.  This International Standard is
   * primarily a consequence of the growing commercial interest in
   * Pascal and the need to promote the portability of Pascal programs
   * between data processing systems.
   *)
  procedure fred;

implementation

  procedure fred;
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
;     5    30  EXAMPLE   UNITSEG    7 P-Code LE 3.0
;        (*
;         * What happens when UNIT interface text crosses a 1KiB boundary?
;         *
;         * ISO/IEC 10206:1990(E)
;         *
;         * Introduction
;         *
;         * This International Standard provides an unambiguous and machine
;         * independent definition of the programming language Extended
;         * Pascal.  Its purpose is to facilitate portability of Extended
;         * Pascal programs for use on a wide variety of data processing
;         * systems.
;         *
;         * Language history
;         *
;         * The computer programming language Pascal was designed by Professor
;         * Niklaus Wirth to satisfy two principal aims
;         *
;         * a) to make available a language suitable for teaching programming
;         *    as a systematic discipline based on certain fundamental
;         *    concepts clearly and naturally reflected by the language; and
;         *
;         * b) to design a language whose implementations could be reliable
;         *    and efficent on then-available computers.
;         *
;         * However, it has become apparent that Pascal has attributes that
;         * go far beyond those original goals.  It is now being increasingly
;         * used commercially in the writing of system and application
;         * software.  With this increased use, there has been an increased
;         * demand for and availability of extensions to ISO 7185:1983,
;         * Programming languages - PASCAL.  Programs using such extensions
;         * attain the benefits of the extended features at the cost of
;         * portability with standard Pascal and with other processors
;         * supporting different sets of extensions.  In the absence of a
;         * standard for an extended language, these processors have become
;         * increasingly incompatible.  This International Standard is
;         * primarily a consequence of the growing commercial interest in
;         * Pascal and the need to promote the portability of Pascal programs
;         * between data processing systems.
;         *)
;        procedure fred;
;                        EOFMARK          1
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
; Procedure 1:
****: AD 00             p1_0:   RNP     0
; Procedure 1 Attribute Table:
****: 00 00                     .word   0               ; data size
****: 00 00                     .word   0               ; param size
****: 06 00                     .word   $ - p1_0        ; exit ic
****: 08 00                     .word   $ - p1_0        ; enter ic
****: 01                p1_at:  .byte   1               ; procedure number
****: 01                        .byte   1               ; lex level

;
; Procedure dictionary:
;
****: 0E 00                     .word   $ - p2_at       ; procedure 2
****: 04 00                     .word   $ - p1_at       ; procedure 1
****: 07                        .byte   7               ; segment number
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
