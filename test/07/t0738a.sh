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

TEST_SUBJECT="ucsdpsys_charset write psftools"
. test_prelude

cat > input.text << 'fubar'
'P' = {
    " XXXXX  ",
    " X    X ",
    " X    X ",
    " X    X ",
    " XXXXX  ",
    " X      ",
    " X      ",
    "        ",
};
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
fubar
test $? -eq 0 || no_result

cat > ok << 'fubar'
%PSF2
Version: 0
Flags: 1
Length: 2
Width: 8
Height: 7
%
// Character 0
// 'M', LATIN CAPITAL LETTER M
Bitmap: -#-----# \
        -##---## \
        -#-#-#-# \
        -#--#--# \
        -#-----# \
        -#-----# \
        -#-----#
Unicode: [004D];
%
// Character 1
// 'P', LATIN CAPITAL LETTER P
Bitmap: -#####-- \
        -#----#- \
        -#----#- \
        -#----#- \
        -#####-- \
        -#------ \
        -#------
Unicode: [0050];
fubar
test $? -eq 0 || no_result

ucsdpsys_charset --output-format=psftools -e input.text output.text
test $? -eq 0 || fail

diff ok output.text
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
