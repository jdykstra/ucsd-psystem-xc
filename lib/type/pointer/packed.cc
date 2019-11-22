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

#include <lib/type/pointer/packed.h>


type_pointer_packed::~type_pointer_packed()
{
}


type_pointer_packed::type_pointer_packed(const type::pointer &a_subtype) :
    type_pointer(a_subtype)
{
}


type_pointer_packed::pointer
type_pointer_packed::create(const type::pointer &a_subtype)
{
    return pointer(new type_pointer_packed(a_subtype));
}


unsigned
type_pointer_packed::get_size_in_bits()
    const
{
    return (3 * 16);
}


bool
type_pointer_packed::is_a_variable()
    const
{
    //
    // This need not be true, except that there is no grammar construct
    // which would allow declaring such a type.  Something like
    //
    //    var ptr: ^[3]0..5;
    //
    // would be consistent with integer[n] and string[n]
    //
    return false;
}


bool
type_pointer_packed::is_a(const type::pointer &arg)
{
    return (0 != dynamic_cast<const type_pointer_packed *>(arg.operator->()));
}


// vim: set ts=8 sw=4 et :
