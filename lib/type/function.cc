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

#include <lib/type/function.h>
#include <lib/type/nothing.h>


type_function::~type_function()
{
}


type_function::type_function(const type::pointer &rtp) :
    subtype(rtp)
{
}


type_function::pointer
type_function::create(const type::pointer &rtp)
{
    return pointer(new type_function(rtp));
}


type_function::pointer
type_function::create()
{
    return create(type_nothing::create());
}


rcstring
type_function::get_name()
    const
{
    return ("function returning " + subtype->get_name());
}


rcstring
type_function::get_pascal_name()
    const
{
    return subtype->get_pascal_name();
}


rcstring
type_function::get_c_declaration(const rcstring &vname)
    const
{
    return get_subtype()->get_c_declaration("(*" + vname + ")(...)");
}


unsigned
type_function::get_size_in_bits()
    const
{
    return subtype->get_size_in_bits();
}


bool
type_function::is_func()
    const
{
    return true;
}


type::pointer
type_function::get_subtype()
    const
{
    return subtype;
}


bool
type_function::is_a(const type::pointer &arg)
{
    return (0 != dynamic_cast<const type_function *>(arg.get()));
}


bool
type_function::same_type_as(const type::pointer &)
    const
{
    return false;
}


bool
type_function::is_a_variable()
    const
{
    return false;
}


bool
type_function::need_late_binding(void)
    const
{
    return subtype->need_late_binding();
}


rcstring
type_function::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :
