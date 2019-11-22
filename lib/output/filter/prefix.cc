//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/output/filter/prefix.h>


output_filter_prefix::~output_filter_prefix()
{
    flush();
}


static rcstring
trim_right(const rcstring &s)
{
    const char *cp = s.c_str();
    size_t len = s.size();
    while (len > 0 && isspace((unsigned char)cp[len - 1]))
        --len;
    return rcstring(cp, len);
}


output_filter_prefix::output_filter_prefix(
    const output::pointer &a_deeper,
    const rcstring &a_pfx
) :
    output_filter(a_deeper),
    pfx(a_pfx),
    pfx0(trim_right(a_pfx)),
    column(0)
{
}


output_filter_prefix::pointer
output_filter_prefix::create(const output::pointer &a_deeper,
    const rcstring &a_pfx)
{
    return pointer(new output_filter_prefix(a_deeper, a_pfx));
}


void
output_filter_prefix::write_inner(const void *data, size_t length)
{
    const char *cp = (const char *)data;
    while (length > 0)
    {
        unsigned char c = *cp++;
        --length;
        if (column == 0)
            deeper_puts(c == '\n' ? pfx0 : pfx);
        deeper_putc(c);
        if (c == '\n')
            column = 0;
        else
            ++column;
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
output_filter_prefix::get_width(void)
    const
{
    unsigned w1 = output_filter::get_width();
    unsigned w2 = width_in_columns(pfx);
    if (w1 < 8 + w2)
        return 8;
    return (w1 - w2);
}


// vim: set ts=8 sw=4 et :
