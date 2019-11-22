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

#include <lib/output/filter/wrap.h>


output_filter_wrap::~output_filter_wrap()
{
    flush();
}


output_filter_wrap::output_filter_wrap(
    const output::pointer &a_deeper,
    bool a_hard_break
) :
    output_filter(a_deeper),
    hard_break(a_hard_break)
{
}


output_filter_wrap::pointer
output_filter_wrap::create(const output::pointer &a_deeper, bool a_hard_break)
{
    return pointer(new output_filter_wrap(a_deeper, a_hard_break));
}


void
output_filter_wrap::write_inner(const void *data, size_t length)
{
    const char *cp = (const char *)data;
    const char *end = cp + length;
    while (cp < end)
    {
        unsigned char c = *cp++;
        if (c == '\n')
            wrap();
        else
            buf.push_back(c);
    }
}


void
output_filter_wrap::wrap(void)
{
    const char *s = buf.get_data();
    const char *s_end = s + buf.size();
    unsigned column = 0;
    unsigned width = get_width();
    rcstring_accumulator space;
    rcstring_accumulator word;
    while (s < s_end)
    {
        // collect the white space
        space.clear();
        while (s < s_end && isspace(*s))
        {
            space.push_back(' ');
            ++s;
        }

        // collect the next word
        word.clear();
        while (s < s_end && !isspace(*s))
        {
            unsigned char c = *s++;
            word.push_back(c);
            if (hard_break && (c == '_' || c == '-' || c == '/'))
                break;
        }

        // see if we have reached the end of the line.
        if (word.empty())
            break;

        if (column + space.size() + word.size() <= width)
        {
            // the space+word fits
            deeper_write(space.get_data(), space.size());
            column += space.size();
        }
        else if (column == 0)
        {
            if (word.size() < width)
            {
                // Use as much of the white space as possible.
                unsigned n = width - word.size();
                deeper_write(space.get_data(), n);
                column += n;
            }
            if (hard_break && word.size() > width)
            {
                size_t x;
                // Yes, I meant less-than, so convoluted conditional
                // continue logic is not required.
                for (x = 0; x + width < word.size(); x += width)
                {
                    deeper_write(word.get_data() + x, width);
                    deeper_putc('\n');
                }
                word.pop_front(x);
                column = 0;
            }
        }
        else
        {
            deeper_putc('\n');
            column = 0;
        }
        deeper_write(word.get_data(), word.size());
        column += word.size();
    }

    //
    // End the line with a newline, even if the input didn't have one.
    //
    deeper_putc('\n');

    //
    // Reset the line, now that we've written it out.
    //
    buf.clear();
}


void
output_filter_wrap::flush_inner()
{
    if (!buf.empty())
        wrap();
    output_filter::flush_inner();
}


// vim: set ts=8 sw=4 et :
