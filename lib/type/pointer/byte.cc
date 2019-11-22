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

#include <lib/debug.h>
#include <lib/type/char.h>
#include <lib/type/pointer/byte.h>
#include <lib/type/subrange.h>


type_pointer_byte::~type_pointer_byte()
{
}


type_pointer_byte::type_pointer_byte(const type::pointer &sub) :
    type_pointer(sub)
{
    DEBUG(2, "sub = %s", sub->get_pascal_name().c_str());
    assert(type_char::is_a(sub) || type_subrange::is_a(sub));
    assert(sub->get_maximum_value() < 256);
}


type_pointer_byte::pointer
type_pointer_byte::create(const type::pointer &sub)
{
    return pointer(new type_pointer_byte(sub));
}


unsigned
type_pointer_byte::get_size_in_bits(void)
    const
{
    return 32;
}


bool
type_pointer_byte::is_a(const type::pointer &candidate)
{
    return !!boost::dynamic_pointer_cast<const type_pointer_byte>(candidate);
}


// vim: set ts=8 sw=4 et :
