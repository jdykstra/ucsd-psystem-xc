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
#include <utime.h>

#include <lib/output/null.h>
#include <lib/rcstring.h>


output_null::~output_null()
{
    flush();
}


output_null::output_null()
{
}


output_null::pointer
output_null::create(void)
{
    return pointer(new output_null());
}


rcstring
output_null::filename(void)
{
    return "/dev/null";
}

void
output_null::utime(struct utimbuf &utb)
{
    memset(&utb, 0, sizeof(utb));
}


unsigned
output_null::get_width(void)
    const
{
    return 80;
}


unsigned
output_null::get_height(void)
    const
{
    return 66;
}


void
output_null::write_inner(const void *, size_t)
{
}


void
output_null::flush_inner(void)
{
}


// vim: set ts=8 sw=4 et :
