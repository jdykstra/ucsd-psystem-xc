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

TEST_SUBJECT="ucsdpsys_charset vs psftools"
. test_prelude

cat > example.text << 'fubar'
%PSF2
//
Version: 0
Flags: 1
Length: 3
Width: 8
Height: 8
%
// Character 0
Bitmap: -#####-- \
        -#----#- \
        -#----#- \
        -#----#- \
        -#####-- \
        -#------ \
        -#------ \
        --------
Unicode: [00000050];[000003a1];
%
// Character 1
Bitmap: -#-----# \
        -##---## \
        -#-#-#-# \
        -#--#--# \
        -#-----# \
        -#-----# \
        -#-----# \
        --------
Unicode: [0000004d];[0000039c];
%
// Character 2
Bitmap: -------- \
        -------- \
        -------- \
        -------- \
        -------- \
        -------- \
        -------- \
        --------
Unicode:
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
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
    " X     X",
    " XX   XX",
    " X X X X",
    " X  X  X",
    " X     X",
    " X     X",
    " X     X",
    "        ",
};

# LATIN CAPITAL LETTER P
'P' = {
    " XXXXX ",
    " X    X",
    " X    X",
    " X    X",
    " XXXXX ",
    " X     ",
    " X     ",
    "       ",
};
fubar
test $? -eq 0 || no_result

ucsdpsys_charset --input-format=psftools -e example.text example.charset
test $? -eq 0 || fail

ucsdpsys_charset -d example.charset test.out.text
test $? -eq 0 || no_result

diff ok test.out.text
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
