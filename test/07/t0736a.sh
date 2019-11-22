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

TEST_SUBJECT="ucsdpsys_charset vs boot logo"
. test_prelude

# make a charset with an image in it
cat > example.text << 'fubar'
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

cat > example.png.uue << 'fubar'
begin 664 example.png
MB5!.1PT*&@H````-24A$4@```$````!``0,```"0I^.=`````7-21T(`KLX<
MZ0````903%1%____````5<+3?@```$-)1$%4*,]C^`\%#`T,8,`XRL#!X#]X
MX`&$\?`AA,%3"&,8;DR`,C9#&863$J"*YT$9!Z$,'L8>#$8#+L9@#0U8L@$`
4BL)#$%G40A@`````245.1*Y"8((`
`
end
fubar
test $? -eq 0 || no_result
uudecode example.png.uue
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
    "X    X  ",
    "XX  XX  ",
    "X XX X  ",
    "X XX X  ",
    "X    X  ",
    "X    X  ",
    "X    X  ",
    "        ",
    "        ",
    "        ",
};

# LATIN CAPITAL LETTER P
'P' = {
    "XXXXX   ",
    " X   X  ",
    " X   X  ",
    " XXXX   ",
    " X      ",
    " X      ",
    "XXX     ",
    "        ",
    "        ",
    "        ",
};
fubar
test $? -eq 0 || no_result

# read example.text and example.png, write example.charset
ucsdpsys_charset -Aterak -e example.text example.charset example.png
test $? -eq 0 || fail

# verify file size
# it would be 2048 if it was just the font,
# but it will be 2560 if it is the font plus the boot logo.
sz=`ls -l example.charset | awk '{print $5}'`
if [ "$sz" -ne 2560 ]
then
    echo "size of the exmaple.charset -ne 2560" 1>&2
    fail
fi

ucsdpsys_charset -Aterak -d example.charset test.out.text test.out.png
test $? -eq 0 || fail

diff ok test.out.text
test $? -eq 0 || fail

if [ ! -s test.out.png ]
then
    echo "failed to create test.out.png" 1>&2
    fail
fi

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
