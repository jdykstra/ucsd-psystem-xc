//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at
// you option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#include <ucsdpsys_charset/sink.h>


sink::~sink()
{
}


sink::sink()
{
}


void
sink::write_whole_font(const font::pointer &fp)
{
    write_whole_font_begin();
    for (unsigned c = 0; c < 256; ++c)
    {
        glyph::pointer gp = fp->get_glyph(c);
        if (gp)
            write_one_glyph(gp);
    }
    write_whole_font_end();
}


void
sink::write_whole_font_begin(void)
{
}


void
sink::write_whole_font_end(void)
{
}


void
sink::print_decode_header(FILE *fp)
{
    fputs
    (
"#\n"
"# This file defines the shapes of the characters for drawing them on the\n"
"# screen.  This what is known as a bit-mapped font.\n"
"#\n"
"# Note that font outlines and bit-mapped fonts are not eligible for\n"
"# copyright protection.  Only fonts which contain computer code (e.g.\n"
"# some PostScript fonts) are eligible for copyright protection.\n"
"#\n",
        fp
    );
}


bool
sink::put_boot_logo(const unsigned char *)
{
    return false;
}


bool
sink::is_binary(void)
    const
{
    return false;
}


bool
sink::looks_like_a_stdout_synonym(const rcstring &filename)
{
    return
        (
            filename.empty()
        ||
            filename == "-"
        ||
            filename == "/dev/stdout"
        ||
            filename == "/dev/fd/1"
        );
}


// vim: set ts=8 sw=4 et :
