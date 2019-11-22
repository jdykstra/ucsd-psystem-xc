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

#include <lib/output/wrap_cook.h>


output_wrap_cook::~output_wrap_cook()
{
    flush();
    if (!buf.empty())
       wrap();
}


output_wrap_cook::output_wrap_cook(const output::pointer &a_deeper) :
    deeper(a_deeper)
{
}


output_wrap_cook::pointer
output_wrap_cook::create(const output::pointer &a_deeper)
{
    return pointer(new output_wrap_cook(a_deeper));
}


unsigned
output_wrap_cook::get_width(void)
    const
{
    return deeper->get_width();
}


unsigned
output_wrap_cook::get_height(void)
    const
{
    return deeper->get_height();
}


void
output_wrap_cook::write_inner(const void *data, size_t length)
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
output_wrap_cook::wrap(void)
{
    const char *s = buf.get_data();
    const char *s_end = s + buf.size();
    unsigned column = 0;
    unsigned width = deeper->get_width();
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
            word.push_back(*s);
            ++s;
        }

        // see if we have reached the end of the line.
        if (word.empty())
            break;

        if (column + space.size() + word.size() <= width)
        {
            // the space+word fits
            deeper->write(space.get_data(), space.size());
            column += space.size();
        }
        else
        {
            deeper->fputc('\n');
            column = 0;
            while (column < 8 && word.size() + column < width)
            {
                deeper->fputc(' ');
                ++column;
            }
        }
        deeper->write(word.get_data(), word.size());
        column += word.size();
    }

    //
    // End the line with a newline, even if the input didn't have one.
    //
    deeper->fputc('\n');

    //
    // Reset the line, now that we've written it out.
    //
    buf.clear();
}


rcstring
output_wrap_cook::filename(void)
{
    return deeper->filename();
}


void
output_wrap_cook::utime(utimbuf &tbuf)
{
    deeper->utime(tbuf);
}


void
output_wrap_cook::flush_inner(void)
{
    // Nothing to do.
}


// vim: set ts=8 sw=4 et :
