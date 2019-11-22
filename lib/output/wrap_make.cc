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

#include <lib/output/wrap_make.h>


output_wrap_make::~output_wrap_make()
{
    flush();
    if (!buf.empty())
       wrap();
}


output_wrap_make::output_wrap_make(const output::pointer &a_deeper) :
    deeper(a_deeper)
{
}


output_wrap_make::pointer
output_wrap_make::create(const output::pointer &a_deeper)
{
    return pointer(new output_wrap_make(a_deeper));
}


unsigned
output_wrap_make::get_width(void)
    const
{
    return deeper->get_width();
}


unsigned
output_wrap_make::get_height(void)
    const
{
    return deeper->get_height();
}


void
output_wrap_make::write_inner(const void *data, size_t length)
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
output_wrap_make::wrap(void)
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
        unsigned space_columns = 0;
        space.clear();
        if (s == buf.get_data())
        {
            if (s < s_end && *s == '\t')
            {
                space_columns = 8;
                space.push_back('\t');
                ++s;
            }
        }
        else
        {
            if (s < s_end && isspace(*s))
            {
                space_columns = 1;
                space.push_back(' ');
                ++s;
            }
        }
        while (s < s_end && isspace(*s))
            ++s;

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

        unsigned rhs = (s >= s_end ? width : width - 2);
        if (column + space_columns + word.size() <= rhs)
        {
            // the space+word fits
            deeper->write(space.get_data(), space.size());
            column += space_columns;
        }
        else
        {
            deeper->fputs(" \\\n\t\t");
            column = 16;
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
output_wrap_make::filename(void)
{
    return deeper->filename();
}


void
output_wrap_make::utime(utimbuf &tbuf)
{
    deeper->utime(tbuf);
}


void
output_wrap_make::flush_inner(void)
{
    // Nothing to do.
}


// vim: set ts=8 sw=4 et :
