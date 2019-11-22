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

#include <lib/ac/string.h>

#include <lib/codefile.h>
#include <lib/debug.h>
#include <lib/segment.h>


segment::~segment()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


segment::segment()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


unsigned
segment::get_word(const unsigned char *data)
    const
{
    return byte_sex_get_word(get_byte_sex(), data);
}


void
segment::put_word(unsigned char *data, unsigned value)
    const
{
    byte_sex_put_word(get_byte_sex(), data, value);
}


// vim: set ts=8 sw=4 et :
