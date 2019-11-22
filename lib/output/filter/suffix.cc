//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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

#include <lib/ac/ctype.h>

#include <lib/output/filter/suffix.h>


output_filter_suffix::~output_filter_suffix()
{
    flush();
}


static rcstring
trim_left(const rcstring &s)
{
    const char *cp = s.c_str();
    const char *end = cp + s.size();
    while (cp < end && isspace((unsigned char)*cp))
        ++cp;
    return rcstring(cp, end - cp);
}


output_filter_suffix::output_filter_suffix(
    const output::pointer &a_deeper,
    const rcstring &a_sfx
) :
    output_filter(a_deeper),
    sfx(a_sfx),
    sfx0(trim_left(a_sfx)),
    column(0)
{
}


output_filter_suffix::pointer
output_filter_suffix::create(const output::pointer &a_deeper,
    const rcstring &a_sfx)
{
    return pointer(new output_filter_suffix(a_deeper, a_sfx));
}


void
output_filter_suffix::write_inner(const void *data, size_t length)
{
    const char *cp = (const char *)data;
    while (length > 0)
    {
        unsigned char c = *cp++;
        --length;
        if (c == '\n')
        {
            deeper_puts(column ? sfx : sfx0);
            column = 0;
        }
        else
            ++column;
        deeper_putc(c);
    }
}


static unsigned
width_in_columns(const rcstring &text)
{
    unsigned result = 0;
    const char *cp = text.c_str();
    const char *end = cp + text.size();
    while (cp < end)
    {
        unsigned char c = *cp++;
        switch (c)
        {
        case '\t':
            result = (result + 8) & ~7;
            break;

        case ' ':
        case '#':
        case ';':
            ++result;
            break;

        default:
            assert(isprint(c));
            ++result;
            break;
        }
    }
    return result;
}


unsigned
output_filter_suffix::get_width(void)
    const
{
    unsigned w1 = output_filter::get_width();
    unsigned w2 = width_in_columns(sfx);
    if (w1 < 8 + w2)
        return 8;
    return (w1 - w2);
}


// vim: set ts=8 sw=4 et :
