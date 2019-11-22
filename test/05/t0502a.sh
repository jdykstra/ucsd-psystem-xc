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

TEST_SUBJECT="labels"
. test_prelude

cat > example.text << 'fubar'
program example;

type
  window = packed array [0..1] of char;
  windowp = ^window;
  FIB =
    RECORD
      FWINDOW: WINDOWP;  (*USER WINDOW...F^, USED BY GET-PUT*)
      FISOPEN: BOOLEAN;
      FSOFTBUF: BOOLEAN;
      funit: integer;
    END;

  PROCEDURE FWRITECHAR(VAR F: FIB; CH: CHAR; RLENG: INTEGER);
    LABEL 1;
  BEGIN
    WITH F DO
      IF FISOPEN THEN
        IF FSOFTBUF THEN
          BEGIN
            WHILE RLENG > 1 DO
              BEGIN
                FWINDOW^[0] := ' ';
                RLENG := RLENG-1
              END;
            FWINDOW^[0] := CH;
          END
        ELSE
          BEGIN
            WHILE RLENG > 1 DO
              BEGIN
                FWINDOW^[0] := ' ';
                UNITWRITE(FUNIT, FWINDOW^, 1);
                RLENG := RLENG-1
              END;
            FWINDOW^[0] := CH;
            UNITWRITE(FUNIT, FWINDOW^, 1)
          END
      ELSE
        begin end;
    1:
  END;

begin
end.
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
ucsdpsys_compile: example.text: 15: warning: Go To Statement Considered
    Harmful (Edsger Dijkstra, 1968); to silence this message use (*$warning
    goto false*)
ucsdpsys_compile: example.text: 41: warning: label "1" is not used ...
ucsdpsys_compile: example.text: 15: warning: ... here is the "1"
    declaration
fubar
test $? -eq 0 || no_result

ucsdpsys_compile example.text > test.out 2>&1
if test $? -ne 0
then
    cat test.out
    fail
fi

diff ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
