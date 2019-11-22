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

#include <lib/ac/string.h>

#include <lib/output/string_list.h>


output_string_list::~output_string_list()
{
    flush();
}


output_string_list::output_string_list(unsigned a_width) :
    width(a_width)
{
}


output_string_list::pointer
output_string_list::create(unsigned a_width)
{
    return pointer(new output_string_list(a_width));
}


rcstring
output_string_list::filename(void)
{
    return "memory";
}


void
output_string_list::utime(struct utimbuf &)
{
}


void
output_string_list::write_inner(const void *data, size_t length)
{
    const char *cp = (const char *)data;
    const char *end = cp + length;
    while (cp < end)
    {
        const char *newline = (const char *)memchr(cp, '\n', end - cp);
        if (!newline)
        {
            current_line.push_back(cp, end - cp);
            break;
        }
        current_line.push_back(cp, newline - cp);
        content.push_back(current_line.mkstr().trim_right());
        current_line.clear();
        cp = newline + 1;
    }
}


void
output_string_list::flush_inner(void)
{
    if (!current_line.empty())
    {
        content.push_back(current_line.mkstr());
        current_line.clear();
    }
}


unsigned
output_string_list::get_width(void)
    const
{
    return width;
}


unsigned
output_string_list::get_height(void)
    const
{
    return 66;
}


void
output_string_list::clear(void)
{
    current_line.clear();
    content.clear();
}


// vim: set ts=8 sw=4 et :
