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

#include <lib/config.h>
#include <libexplain/output.h>

#include <ucsdpsys_charset/source.h>


source::~source()
{
}


source::source()
{
}


font::pointer
source::read_whole_font(void)
{
    read_whole_font_begin();
    font::pointer result = font::create(1, 1);
    result->read_whole_font_begin();
    for (;;)
    {
        glyph::pointer gp = read_one_glyph();
        if (!gp)
            break;

        //
        // check for duplicates
        //
        glyph::pointer gp2 = result->get_glyph(gp->get_number());
        if (gp2)
        {
            explain_output_error
            (
                "%sduplicate glyph %s definition...",
                gp->get_location().c_str(),
                gp->get_number_pretty().c_str()
            );
            explain_output_error_and_die
            (
                "%s...here is the first defintion",
                gp2->get_location().c_str()
            );
        }

        //
        // remember the glyph
        //
        result->set_glyph(gp);
    }
    result->read_whole_font_end();
    read_whole_font_end();
    return result;
}


void
source::read_whole_font_begin(void)
{
}


void
source::read_whole_font_end(void)
{
}


bool
source::looks_like_a_stdin_synonym(const rcstring &fn)
{
    return (fn.empty() || fn == "-" || fn == "/dev/stdin" || fn == "/dev/fd/0");
}


bool
source::is_binary(void)
    const
{
    return false;
}


bool
source::get_boot_logo(unsigned char *)
{
    return false;
}


// vim: set ts=8 sw=4 et :
