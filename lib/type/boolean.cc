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

#include <lib/type/boolean.h>


type_boolean::~type_boolean()
{
}


type_boolean::type_boolean()
{
}


type_boolean::pointer
type_boolean::create()
{
    static pointer tsp;
    if (!tsp)
        tsp = pointer(new type_boolean());
    return tsp;
}


rcstring
type_boolean::get_name()
    const
{
    return "boolean";
}


rcstring
type_boolean::get_pascal_name()
    const
{
    return get_name();
}


rcstring
type_boolean::get_c_declaration(const rcstring &vname)
    const
{
    return ("bool " + vname);
}


unsigned
type_boolean::get_size_in_bits()
    const
{
    return 1;
}


bool
type_boolean::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_boolean::is_a(const type::pointer &arg)
{
    return is_a(arg.get());
}


bool
type_boolean::is_a(const type *arg)
{
    return (0 != dynamic_cast<const type_boolean *>(arg));
}


bool
type_boolean::same_type_as(const type::pointer &rhs)
    const
{
    // Don't worry about matching "anything" as well,
    // because the == operator took care of it.
    return is_a(rhs);
}


bool
type_boolean::is_a_variable()
    const
{
    return true;
}


type::pointer
type_boolean::get_subtype()
    const
{
    assert(0);
    return create();
}


int
type_boolean::get_minimum_value()
    const
{
    return 0;
}


int
type_boolean::get_maximum_value()
    const
{
    return 1;
}


bool
type_boolean::may_be_used_as_array_index()
    const
{
    return true;
}


bool
type_boolean::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_boolean::get_human_readable_representation_of_value(int n)
    const
{
    if (n < 0 || n >= 2)
        return rcstring::printf("%d", n);
    return (n ? "true" : "false");
}


// vim: set ts=8 sw=4 et :
