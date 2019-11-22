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

#include <lib/input/divert.h>


input_divert::~input_divert()
{
}


input_divert::input_divert(
    const input::pointer &a_deeper,
    const output::pointer &a_copy
) :
    deeper(a_deeper),
    copy(a_copy)
{
}


input_divert::pointer
input_divert::create(const input::pointer &a_deeper,
    const output::pointer &a_copy)
{
    return pointer(new input_divert(a_deeper, a_copy));
}


rcstring
input_divert::name(void)
{
    return deeper->name();
}


long
input_divert::length(void)
{
    return deeper->length();
}


int
input_divert::fpathconf_name_max(void)
{
    return deeper->fpathconf_name_max();
}


void
input_divert::fstat(struct stat &data)
{
    return deeper->fstat(data);
}


long
input_divert::read_inner(void *data, size_t nbytes)
{
    char *start = (char *)data;
    char *cp = start;
    char *end = start + nbytes;
    while (cp < end)
    {
        int c = deeper->getch();
        if (c < 0)
            break;
        copy->fputc(c);
        *cp++ = c;

        // The assembler needs this to be line-by-line so that it can
        // direct it into the appropriate listing column.
        if (c == '\n')
            break;
    }
    return (cp - start);
}


// vim: set ts=8 sw=4 et :
