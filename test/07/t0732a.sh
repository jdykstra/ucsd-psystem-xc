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

TEST_SUBJECT="ucsdpsys_charset vs 'include' format"
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

cat > ok << 'fubar'
#
# This file defines the shapes of the characters for drawing them on the
# screen.  This what is known as a bit-mapped font.
#
# Note that font outlines and bit-mapped fonts are not eligible for
# copyright protection.  Only fonts which contain computer code (e.g.
# some PostScript fonts) are eligible for copyright protection.
#


/* 'M' => LATIN CAPITAL LETTER M */

static glyph::pointer
glyph_4D(void)
{
    static const unsigned char data[] =
    {
        0x21,
        0x33,
        0x2D,
        0x2D,
        0x21,
        0x21,
        0x21,
    };
    return glyph::create(77, 6, 7, data);
}


/* 'P' => LATIN CAPITAL LETTER P */

static glyph::pointer
glyph_50(void)
{
    static const unsigned char data[] =
    {
        0x1F,
        0x22,
        0x22,
        0x1E,
        0x02,
        0x02,
        0x07,
    };
    return glyph::create(80, 6, 7, data);
}

typedef glyph::pointer (*create_t)(void);

static const create_t create[256] =
{
    NULL,     /* '\0' */
    NULL,     /* 1 */
    NULL,     /* 2 */
    NULL,     /* 3 */
    NULL,     /* 4 */
    NULL,     /* 5 */
    NULL,     /* 6 */
    NULL,     /* '\a' */
    NULL,     /* '\b' */
    NULL,     /* '\t' */
    NULL,     /* '\n' */
    NULL,     /* '\v' */
    NULL,     /* '\f' */
    NULL,     /* '\r' */
    NULL,     /* 14 */
    NULL,     /* 15 */
    NULL,     /* 16 */
    NULL,     /* 17 */
    NULL,     /* 18 */
    NULL,     /* 19 */
    NULL,     /* 20 */
    NULL,     /* 21 */
    NULL,     /* 22 */
    NULL,     /* 23 */
    NULL,     /* 24 */
    NULL,     /* 25 */
    NULL,     /* 26 */
    NULL,     /* 27 */
    NULL,     /* 28 */
    NULL,     /* 29 */
    NULL,     /* 30 */
    NULL,     /* 31 */
    NULL,     /* ' ' */
    NULL,     /* '!' */
    NULL,     /* '"' */
    NULL,     /* '#' */
    NULL,     /* '$' */
    NULL,     /* '%' */
    NULL,     /* '&' */
    NULL,     /* '\'' */
    NULL,     /* '(' */
    NULL,     /* ')' */
    NULL,     /* '*' */
    NULL,     /* '+' */
    NULL,     /* ',' */
    NULL,     /* '-' */
    NULL,     /* '.' */
    NULL,     /* '/' */
    NULL,     /* '0' */
    NULL,     /* '1' */
    NULL,     /* '2' */
    NULL,     /* '3' */
    NULL,     /* '4' */
    NULL,     /* '5' */
    NULL,     /* '6' */
    NULL,     /* '7' */
    NULL,     /* '8' */
    NULL,     /* '9' */
    NULL,     /* ':' */
    NULL,     /* ';' */
    NULL,     /* '<' */
    NULL,     /* '=' */
    NULL,     /* '>' */
    NULL,     /* '?' */
    NULL,     /* '@' */
    NULL,     /* 'A' */
    NULL,     /* 'B' */
    NULL,     /* 'C' */
    NULL,     /* 'D' */
    NULL,     /* 'E' */
    NULL,     /* 'F' */
    NULL,     /* 'G' */
    NULL,     /* 'H' */
    NULL,     /* 'I' */
    NULL,     /* 'J' */
    NULL,     /* 'K' */
    NULL,     /* 'L' */
    glyph_4D, /* 'M', LATIN CAPITAL LETTER M */
    NULL,     /* 'N' */
    NULL,     /* 'O' */
    glyph_50, /* 'P', LATIN CAPITAL LETTER P */
    NULL,     /* 'Q' */
    NULL,     /* 'R' */
    NULL,     /* 'S' */
    NULL,     /* 'T' */
    NULL,     /* 'U' */
    NULL,     /* 'V' */
    NULL,     /* 'W' */
    NULL,     /* 'X' */
    NULL,     /* 'Y' */
    NULL,     /* 'Z' */
    NULL,     /* '[' */
    NULL,     /* '\\' */
    NULL,     /* ']' */
    NULL,     /* '^' */
    NULL,     /* '_' */
    NULL,     /* '`' */
    NULL,     /* 'a' */
    NULL,     /* 'b' */
    NULL,     /* 'c' */
    NULL,     /* 'd' */
    NULL,     /* 'e' */
    NULL,     /* 'f' */
    NULL,     /* 'g' */
    NULL,     /* 'h' */
    NULL,     /* 'i' */
    NULL,     /* 'j' */
    NULL,     /* 'k' */
    NULL,     /* 'l' */
    NULL,     /* 'm' */
    NULL,     /* 'n' */
    NULL,     /* 'o' */
    NULL,     /* 'p' */
    NULL,     /* 'q' */
    NULL,     /* 'r' */
    NULL,     /* 's' */
    NULL,     /* 't' */
    NULL,     /* 'u' */
    NULL,     /* 'v' */
    NULL,     /* 'w' */
    NULL,     /* 'x' */
    NULL,     /* 'y' */
    NULL,     /* 'z' */
    NULL,     /* '{' */
    NULL,     /* '|' */
    NULL,     /* '}' */
    NULL,     /* '~' */
    NULL,     /* 127 */
    NULL,     /* 128 */
    NULL,     /* 129 */
    NULL,     /* 130 */
    NULL,     /* 131 */
    NULL,     /* 132 */
    NULL,     /* 133 */
    NULL,     /* 134 */
    NULL,     /* 135 */
    NULL,     /* 136 */
    NULL,     /* 137 */
    NULL,     /* 138 */
    NULL,     /* 139 */
    NULL,     /* 140 */
    NULL,     /* 141 */
    NULL,     /* 142 */
    NULL,     /* 143 */
    NULL,     /* 144 */
    NULL,     /* 145 */
    NULL,     /* 146 */
    NULL,     /* 147 */
    NULL,     /* 148 */
    NULL,     /* 149 */
    NULL,     /* 150 */
    NULL,     /* 151 */
    NULL,     /* 152 */
    NULL,     /* 153 */
    NULL,     /* 154 */
    NULL,     /* 155 */
    NULL,     /* 156 */
    NULL,     /* 157 */
    NULL,     /* 158 */
    NULL,     /* 159 */
    NULL,     /* 160 */
    NULL,     /* 161 */
    NULL,     /* 162 */
    NULL,     /* 163 */
    NULL,     /* 164 */
    NULL,     /* 165 */
    NULL,     /* 166 */
    NULL,     /* 167 */
    NULL,     /* 168 */
    NULL,     /* 169 */
    NULL,     /* 170 */
    NULL,     /* 171 */
    NULL,     /* 172 */
    NULL,     /* 173 */
    NULL,     /* 174 */
    NULL,     /* 175 */
    NULL,     /* 176 */
    NULL,     /* 177 */
    NULL,     /* 178 */
    NULL,     /* 179 */
    NULL,     /* 180 */
    NULL,     /* 181 */
    NULL,     /* 182 */
    NULL,     /* 183 */
    NULL,     /* 184 */
    NULL,     /* 185 */
    NULL,     /* 186 */
    NULL,     /* 187 */
    NULL,     /* 188 */
    NULL,     /* 189 */
    NULL,     /* 190 */
    NULL,     /* 191 */
    NULL,     /* 192 */
    NULL,     /* 193 */
    NULL,     /* 194 */
    NULL,     /* 195 */
    NULL,     /* 196 */
    NULL,     /* 197 */
    NULL,     /* 198 */
    NULL,     /* 199 */
    NULL,     /* 200 */
    NULL,     /* 201 */
    NULL,     /* 202 */
    NULL,     /* 203 */
    NULL,     /* 204 */
    NULL,     /* 205 */
    NULL,     /* 206 */
    NULL,     /* 207 */
    NULL,     /* 208 */
    NULL,     /* 209 */
    NULL,     /* 210 */
    NULL,     /* 211 */
    NULL,     /* 212 */
    NULL,     /* 213 */
    NULL,     /* 214 */
    NULL,     /* 215 */
    NULL,     /* 216 */
    NULL,     /* 217 */
    NULL,     /* 218 */
    NULL,     /* 219 */
    NULL,     /* 220 */
    NULL,     /* 221 */
    NULL,     /* 222 */
    NULL,     /* 223 */
    NULL,     /* 224 */
    NULL,     /* 225 */
    NULL,     /* 226 */
    NULL,     /* 227 */
    NULL,     /* 228 */
    NULL,     /* 229 */
    NULL,     /* 230 */
    NULL,     /* 231 */
    NULL,     /* 232 */
    NULL,     /* 233 */
    NULL,     /* 234 */
    NULL,     /* 235 */
    NULL,     /* 236 */
    NULL,     /* 237 */
    NULL,     /* 238 */
    NULL,     /* 239 */
    NULL,     /* 240 */
    NULL,     /* 241 */
    NULL,     /* 242 */
    NULL,     /* 243 */
    NULL,     /* 244 */
    NULL,     /* 245 */
    NULL,     /* 246 */
    NULL,     /* 247 */
    NULL,     /* 248 */
    NULL,     /* 249 */
    NULL,     /* 250 */
    NULL,     /* 251 */
    NULL,     /* 252 */
    NULL,     /* 253 */
    NULL,     /* 254 */
    NULL,     /* 255 */
};

glyph::pointer
test_out_glyph_create(int c)
{
    if (c < 0 || c >= 256 || !create[c])
        return glyph::pointer();
    return create[c]();
}
fubar
test $? -eq 0 || no_result

ucsdpsys_charset --arch=terak -i test.in.text test.out.h
test $? -eq 0 || fail

diff ok test.out.h
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass


/* vim: set ts=8 sw=4 et : */
