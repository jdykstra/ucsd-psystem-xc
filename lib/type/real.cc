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
#include <lib/type/real.h>


type_real::~type_real()
{
}


type_real::type_real()
{
}


type_real::pointer
type_real::create()
{
    static pointer tsp;
    if (!tsp)
        tsp = pointer(new type_real());
    return tsp;
}


rcstring
type_real::get_name()
    const
{
    return "real";
}


rcstring
type_real::get_pascal_name()
    const
{
    return get_name();
}


rcstring
type_real::get_c_declaration(const rcstring &vname)
    const
{
    return ("float " + vname);
}


unsigned
type_real::get_size_in_bits()
    const
{
    return 32;
}


bool
type_real::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_real::is_a(const type::pointer &arg)
{
    return is_a(arg.get());
}


bool
type_real::is_a(const type *arg)
{
    return (0 != dynamic_cast<const type_real *>(arg));
}


bool
type_real::same_type_as(const type::pointer &rhs)
    const
{
    return is_a(rhs);
}


bool
type_real::is_a_variable()
    const
{
    return true;
}


type::pointer
type_real::get_subtype()
    const
{
    assert(0);
    return create();
}


bool
type_real::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_real::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :
