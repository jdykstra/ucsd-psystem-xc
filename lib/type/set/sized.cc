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

#include <lib/ac/assert.h>

#include <lib/type/set/sized.h>


type_set_sized::~type_set_sized()
{
}


type_set_sized::type_set_sized(const type::pointer &arg) :
    type_set(arg)
{
}


type_set_sized::pointer
type_set_sized::create(const type::pointer &arg)
{
    return pointer(new type_set_sized(arg));
}


bool
type_set_sized::is_a(const type::pointer &tp)
{
    return (0 != dynamic_cast<const type_set_sized *>(tp.operator->()));
}


unsigned
type_set_sized::get_size_in_bits()
    const
{
    assert(0);
    return ~0;
}


unsigned
type_set_sized::get_size_in_words()
    const
{
    assert(0);
    return ~0;
}


// vim: set ts=8 sw=4 et :
