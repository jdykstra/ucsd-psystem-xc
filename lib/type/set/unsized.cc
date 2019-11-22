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

#include <lib/type/set/sized.h>
#include <lib/type/set/unsized.h>


type_set_unsized::~type_set_unsized()
{
}


type_set_unsized::type_set_unsized(const type::pointer &arg) :
    type_set(arg)
{
}


type_set_unsized::pointer
type_set_unsized::create(const type::pointer &arg)
{
    return pointer(new type_set_unsized(arg));
}


unsigned
type_set_unsized::get_size_in_bits()
    const
{
    return ((get_subtype()->get_maximum_value() | 15) + 1);
}


type::pointer
type_set_unsized::get_sized_equiv(void)
    const
{
    return type_set_sized::create(get_subtype());
}


bool
type_set_unsized::is_a(const type::pointer &tp)
{
    return (0 != dynamic_cast<const type_set_unsized *>(tp.operator->()));
}


// vim: set ts=8 sw=4 et :
