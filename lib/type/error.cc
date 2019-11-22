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

#include <lib/type/error.h>


type_error::~type_error()
{
}


type_error::type_error()
{
}


type_error::pointer
type_error::create()
{
    static pointer p;
    if (!p)
        p = pointer(new type_error());
    return p;
}


rcstring
type_error::get_name()
    const
{
    return "error";
}


rcstring
type_error::get_pascal_name()
    const
{
    return get_name();
}


rcstring
type_error::get_c_declaration(const rcstring &vname)
    const
{
    return ("$#@error&%$ " + vname);
}


unsigned
type_error::get_size_in_bits()
    const
{
    return 0;
}


bool
type_error::same_type_as(const type::pointer &rhs)
    const
{
    return is_a(rhs);
}


bool
type_error::is_a_variable()
    const
{
    return false;
}


bool
type_error::is_a(type::pointer const &rhs)
{
    return is_a(rhs.get());
}


bool
type_error::is_a(const type *rhs)
{
    return (0 != dynamic_cast<const type_error *>(rhs));
}


type::pointer
type_error::get_subtype()
    const
{
    assert(0);
    return create();
}


bool
type_error::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_error::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :
