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

#include <lib/output/stracc.h>


output_stracc::~output_stracc()
{
}


output_stracc::output_stracc(unsigned a_width) :
    width(a_width)
{
}


output_stracc::pointer
output_stracc::create(unsigned a_width)
{
    return pointer(new output_stracc(a_width));
}


rcstring
output_stracc::filename(void)
{
    return "memory";
}


void
output_stracc::utime(struct utimbuf &)
{
}


void
output_stracc::write_inner(const void *data, size_t length)
{
    content.push_back(data, length);
}


void
output_stracc::flush_inner(void)
{
}


unsigned
output_stracc::get_width(void)
    const
{
    return width;
}


unsigned
output_stracc::get_height(void)
    const
{
    return 66;
}


// vim: set ts=8 sw=4 et :
