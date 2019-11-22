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
#include <lib/type/pointer.h>


type_pointer::~type_pointer()
{
}


type_pointer::type_pointer(const type::pointer &a_subtype) :
    subtype(a_subtype)
{
}


rcstring
type_pointer::get_name()
    const
{
    return ("pointer to " + subtype->get_name());
}


rcstring
type_pointer::get_pascal_name()
    const
{
    return ("^" + subtype->get_pascal_name());
}


rcstring
type_pointer::get_c_declaration(const rcstring &vname)
    const
{
    // pointer to array of 7 int
    // int (*fred)[7];
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(get_subtype());
    if (tap)
    {
        return get_subtype()->get_c_declaration("(*" + vname + ")");
    }

    // array of 7 pointers to int
    // int *fred[7]
    return get_subtype()->get_c_declaration("*" + vname);
}


bool
type_pointer::same_type_as(const type::pointer &rhs)
    const
{
    return (is_a(rhs) && subtype == rhs->get_subtype());
}


type::pointer
type_pointer::get_subtype()
    const
{
    return subtype;
}


bool
type_pointer::is_a(const expression::pointer &candidate)
{
    return is_a(candidate->get_type());
}


bool
type_pointer::is_a(const type::pointer &candidate)
{
    return is_a(candidate.get());
}


bool
type_pointer::is_a(const type *candidate)
{
    return (0 != dynamic_cast<const type_pointer *>(candidate));
}


bool
type_pointer::is_a_variable()
    const
{
    return true;
}


bool
type_pointer::need_late_binding(void)
    const
{
    return subtype->need_late_binding();
}


rcstring
type_pointer::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :
