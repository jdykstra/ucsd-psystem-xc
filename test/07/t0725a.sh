#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2006-2008, 2010, 2012 Peter Miller
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

TEST_SUBJECT="ucsdpsys_charset"
. test_prelude

cat > test.in.text << 'fubar'
'P' = {
    "XXXXX ",
    " X   X",
    " X   X",
    " XXXX ",
    " X    ",
    " X    ",
    "XXX   ",
    "      ",
};
'M' = {
    "X    X ",
    "XX  XX  ",
    "X XX X ",
    "X XX X  ",
    "X    X ",
    "X    X",
    "X    X "
};
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
#
# This file defines the shapes of the characters for drawing them on the
# screen.  This what is known as a bit-mapped font.
#
# Note that font outlines and bit-mapped fonts are not eligible for
# copyright protection.  Only fonts which contain computer code (e.g.
# some PostScript fonts) are eligible for copyright protection.
#

# LATIN CAPITAL LETTER M
'M' = {
    "X    X ",
    "XX  XX ",
    "X XX X ",
    "X XX X ",
    "X    X ",
    "X    X ",
    "X    X ",
    "       ",
};

# LATIN CAPITAL LETTER P
'P' = {
    "XXXXX  ",
    " X   X ",
    " X   X ",
    " XXXX  ",
    " X     ",
    " X     ",
    "XXX    ",
    "       ",
};
fubar
test $? -eq 0 || no_result

ucsdpsys_charset -e test.in.text test.bin
test $? -eq 0 || fail

ucsdpsys_charset -d test.bin test.out.text
test $? -eq 0 || fail

diff test.ok test.out.text
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
