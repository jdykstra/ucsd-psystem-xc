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

#include <lib/type/pointer/unpacked.h>


type_pointer_unpacked::~type_pointer_unpacked()
{
}


type_pointer_unpacked::type_pointer_unpacked(const type::pointer &a_subtype) :
    type_pointer(a_subtype)
{
}


type_pointer_unpacked::pointer
type_pointer_unpacked::create(const type::pointer &a_subtype)
{
    return pointer(new type_pointer_unpacked(a_subtype));
}


unsigned
type_pointer_unpacked::get_size_in_bits()
    const
{
    return 16;
}


bool
type_pointer_unpacked::is_a(const type::pointer &candidate)
{
    return (dynamic_cast<const type_pointer_unpacked *>(candidate.operator->())
            != 0);
}


type::pointer
type_pointer_unpacked::late_binding_or_null()
    const
{
    return create(type::late_binding(get_subtype()));
}


// vim: set ts=8 sw=4 et :
