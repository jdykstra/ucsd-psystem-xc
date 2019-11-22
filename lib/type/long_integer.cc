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

#include <lib/type/long_integer.h>


type_long_integer::~type_long_integer()
{
}


type_long_integer::type_long_integer()
{
}


rcstring
type_long_integer::get_pascal_name()
    const
{
    return get_name();
}


rcstring
type_long_integer::get_c_declaration(const rcstring &vname)
    const
{
    return ("long_integer " + vname);
}


bool
type_long_integer::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_long_integer::is_a(const type::pointer &arg)
{
    return !!dynamic_cast<const type_long_integer *>(arg.get());
}


bool
type_long_integer::same_type_as(const type::pointer &arg)
    const
{
    return is_a(arg);
}


type::pointer
type_long_integer::get_subtype()
    const
{
    assert(!"long integers don't have a subtype");
    return create(max_digits);
}


bool
type_long_integer::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_long_integer::get_human_readable_representation_of_value(int n)
    const
{
    assert(!"this is probably a mistake");
    return rcstring::printf("%d", n);
}


// vim: set ts=8 sw=4 et :
