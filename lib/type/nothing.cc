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

#include <lib/type/nothing.h>


type_nothing::~type_nothing()
{
}


type_nothing::type_nothing()
{
}


type_nothing::pointer
type_nothing::create(void)
{
    static pointer singleton;
    if (!singleton)
    {
       singleton = pointer(new type_nothing());
       assert(singleton);
    }
    return singleton;
}


bool
type_nothing::is_a(const type::pointer &candidate)
{
    return (0 != dynamic_cast<const type_nothing *>(candidate.get()));
}


rcstring
type_nothing::get_name(void)
    const
{
    return "nothing";
}


rcstring
type_nothing::get_pascal_name(void)
    const
{
    return "";
}


rcstring
type_nothing::get_c_declaration(const rcstring &vname)
    const
{
    return ("void " + vname);
}


type::pointer
type_nothing::get_subtype(void)
    const
{
    assert(!"nothing doesn't have a subtype");
    return pointer();
}


unsigned
type_nothing::get_size_in_bits(void)
    const
{
    return 0;
}


bool
type_nothing::same_type_as(const type::pointer &tp)
    const
{
    // A whole class, just for this.
    return is_a(tp);
}


bool
type_nothing::is_a_variable(void)
    const
{
    return false;
}


bool
type_nothing::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_nothing::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return "";
}


int
type_nothing::get_finit_recwords()
    const
{
    // See system/system.b.text in the II.0
    // source for the FINIT implementation.
    return -1;
}


// vim: set ts=8 sw=4 et :
