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

#include <lib/output/filter/expand.h>


output_filter_expand::~output_filter_expand()
{
    flush();
}


output_filter_expand::output_filter_expand(
    const output::pointer &a_deeper,
    unsigned a_tab_width
) :
    output_filter(a_deeper),
    tab_width(a_tab_width ? a_tab_width : 1),
    in_col(0),
    out_col(0)
{
}


output_filter_expand::pointer
output_filter_expand::create(const output::pointer &a_deeper,
    unsigned a_tab_width)
{
    return pointer(new output_filter_expand(a_deeper, a_tab_width));
}


void
output_filter_expand::write_inner(const void *data, size_t data_size)
{
    const char *cp = (const char *)data;
    const char *end = cp + data_size;
    while (cp < end)
    {
        unsigned char c = *cp++;
        switch (c)
        {
        case ' ':
            ++in_col;
            break;

        case '\t':
            in_col += tab_width - (in_col % tab_width);
            break;

        case '\n':
            deeper_putc('\n');
            in_col = 0;
            out_col = 0;
            break;

        default:
            while (out_col < in_col)
            {
                deeper_putc(' ');
                ++out_col;
            }
            deeper_putc(c);
            ++out_col;
            in_col = out_col;
            break;
        }
    }
}


void
output_filter_expand::flush_inner(void)
{
    in_col = 0;
    out_col = 0;
    output_filter::flush_inner();
}


// vim: set ts=8 sw=4 et :
