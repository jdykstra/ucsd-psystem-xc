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

#include <lib/output/filter.h>
#include <lib/rcstring.h>


output_filter::~output_filter()
{
}


output_filter::output_filter(const output::pointer &a_deeper) :
    deeper(a_deeper)
{
}


rcstring
output_filter::filename(void)
{
    return deeper->filename();
}


void
output_filter::utime(struct utimbuf &utb)
{
    deeper->utime(utb);
}


unsigned
output_filter::get_width(void)
    const
{
    return deeper->get_width();
}


unsigned
output_filter::get_height(void)
    const
{
    return deeper->get_height();
}


void
output_filter::write_inner(const void *data, size_t length)
{
    deeper_write(data, length);
}


void
output_filter::flush_inner(void)
{
    deeper->flush();
}


void
output_filter::set_heading1(const rcstring &text)
{
    deeper->set_heading1(text);
}


void
output_filter::set_heading2(const rcstring &text)
{
    deeper->set_heading2(text);
}


// vim: set ts=8 sw=4 et :
