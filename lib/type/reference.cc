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

#include <lib/type/array.h>
#include <lib/type/reference.h>


type_reference::~type_reference()
{
}


type_reference::type_reference(const type::pointer &a_subtype) :
    subtype(a_subtype)
{
}


type_reference::pointer
type_reference::create(const type::pointer &a_subtype)
{
    return pointer(new type_reference(a_subtype));
}


rcstring
type_reference::get_name(void)
    const
{
    return ("reference to " + subtype->get_name());
}


rcstring
type_reference::get_pascal_name(void)
    const
{
    return subtype->get_pascal_name();
}


rcstring
type_reference::get_c_declaration(const rcstring &vname)
    const
{
    // reference to array of 7 int
    // int (&fred)[7];
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(get_subtype());
    if (tap)
    {
        return get_subtype()->get_c_declaration("(&" + vname + ")");
    }

    // array of 7 references to int
    // int &fred[7];
    return get_subtype()->get_c_declaration("&" + vname);
}


unsigned
type_reference::get_size_in_bits(void)
    const
{
    return 16;
}


type::pointer
type_reference::get_subtype(void)
    const
{
    return subtype;
}


bool
type_reference::is_a(const type::pointer &arg)
{
    return is_a(arg.get());
}


bool
type_reference::is_a(const type *arg)
{
    return (0 != dynamic_cast<const type_reference *>(arg));
}


bool
type_reference::same_type_as(const type::pointer &rhs)
    const
{
    return (is_a(rhs) && subtype == rhs->get_subtype());
}


bool
type_reference::is_a_variable(void)
    const
{
    return true;
}


int
type_reference::get_minimum_value(void)
    const
{
    return subtype->get_minimum_value();
}


int
type_reference::get_maximum_value(void)
    const
{
    return subtype->get_maximum_value();
}


bool
type_reference::may_be_used_as_array_index(void)
    const
{
    return subtype->may_be_used_as_array_index();
}


bool
type_reference::need_late_binding(void)
    const
{
    return subtype->need_late_binding();
}


rcstring
type_reference::get_human_readable_representation_of_value(int n)
    const
{
    return subtype->get_human_readable_representation_of_value(n);
}


// vim: set ts=8 sw=4 et :
