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

#include <lib/expression.h>
#include <lib/type/integer.h>
#include <lib/type/subrange.h>


type_integer::~type_integer()
{
}


type_integer::type_integer()
{
}


type_integer::pointer
type_integer::create()
{
    static pointer tsp;
    if (!tsp)
        tsp = pointer(new type_integer());
    return tsp;
}


rcstring
type_integer::get_name()
    const
{
    return "integer";
}


rcstring
type_integer::get_pascal_name()
    const
{
    return get_name();
}


rcstring
type_integer::get_c_declaration(const rcstring &vname)
    const
{
    return ("int16_t " + vname);
}


unsigned
type_integer::get_size_in_bits()
    const
{
    return 16;
}


bool
type_integer::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_integer::is_a(const type::pointer &arg)
{
    type::pointer r1 = arg;
    for (;;)
    {
        type_subrange::pointer rr =
            boost::dynamic_pointer_cast<type_subrange>(r1);
        if (!rr)
            break;
        r1 = rr->get_subtype();
    }

    return boost::dynamic_pointer_cast<type_integer>(r1);
}


bool
type_integer::same_type_as(const type::pointer &rhs)
    const
{
    // Don't worry about matching "anything" as well,
    // because the == operator took care of it.
    return is_a(rhs);
}


bool
type_integer::is_a_variable()
    const
{
    return true;
}


type::pointer
type_integer::get_subtype()
    const
{
    assert(!"integers don't have a subtype");
    return create();
}


int
type_integer::get_minimum_value()
    const
{
    return -32768;
}


int
type_integer::get_maximum_value()
    const
{
    return 32767;
}


bool
type_integer::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_integer::get_human_readable_representation_of_value(int n)
    const
{
    return rcstring::printf("%d", n);
}


// vim: set ts=8 sw=4 et :
