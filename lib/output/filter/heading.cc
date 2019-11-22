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

#include <lib/output/filter/heading.h>
#include <lib/rcstring/accumulator.h>


output_filter_heading::~output_filter_heading()
{
    flush();
}


output_filter_heading::output_filter_heading(const output::pointer &a_deeper) :
    output_filter(a_deeper),
    high_water(32767),
    line(0),
    column(0),
    small(false),
    page(0)
{
    int h = output_filter::get_height();
    small = h < 50;
    high_water = h < 4 ? 1 : (small ? h - 4 : h - 8);
}


output_filter_heading::pointer
output_filter_heading::create(const output::pointer &arg)
{
    return pointer(new output_filter_heading(arg));
}


unsigned
output_filter_heading::get_height(void)
    const
{
    return high_water;
}


void
output_filter_heading::write_inner(const void *data, size_t length)
{
    const char *cp = (const char *)data;
    for (;length > 0; --length)
    {
        unsigned char c = *cp++;
        if (line == 0 && column == 0 && c != '\f')
            draw_heading();
        switch (c)
        {
        case '\f':
            while (column > 0 || line > 0)
                write_inner("\n", 1);
            break;

        case '\n':
            deeper_putc(c);
            column = 0;
            ++line;
            if (line >= high_water)
            {
                line = 0;
                draw_footing();
            }
            break;

        case '\t':
            for (;;)
            {
                deeper_putc(' ');
                ++column;
                if (!(column & 7))
                    break;
            }
            break;

        default:
            deeper_putc(c);
            ++column;
            break;
        }
    }
}


void
output_filter_heading::draw_heading(void)
{
    ++page;
    rcstring_accumulator pg;
    pg.printf("  Page %d", page);

    unsigned pw = get_width();
    unsigned w = (pw <= pg.size() ? 1 : pw - pg.size());
    rcstring_accumulator ac;
    if (small)
        ac.push_back("\n\n");
    ac.printf("%-*.*s", w, w, heading1.c_str());
    ac.push_back(pg);
    ac.push_back('\n');
    ac.push_back(heading2.substring(0, w));
    ac.push_back("\n\n");
    if (!small)
        ac.push_back('\n');
    deeper_puts(ac.mkstr());
}


void
output_filter_heading::draw_footing(void)
{
    if (small)
        deeper_puts("\n\n");
}


void
output_filter_heading::set_heading1(const rcstring &text)
{
    heading1 = text.trim();
}


void
output_filter_heading::set_heading2(const rcstring &text)
{
    heading2 = text.trim();
}


// vim: set ts=8 sw=4 et :
