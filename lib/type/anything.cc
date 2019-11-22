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

#include <lib/type/anything.h>


type_anything::~type_anything()
{
}


type_anything::type_anything()
{
}


type_anything::pointer
type_anything::create(void)
{
    static pointer singleton;
    if (!singleton)
    {
       singleton = pointer(new type_anything());
       assert(singleton);
    }
    return singleton;
}


bool
type_anything::is_a(const type::pointer &candidate)
{
    return (0 != dynamic_cast<const type_anything *>(candidate.get()));
}


rcstring
type_anything::get_name(void)
    const
{
    return "anything";
}


rcstring
type_anything::get_pascal_name(void)
    const
{
    return "";
}


type::pointer
type_anything::get_subtype(void)
    const
{
    assert(!"anything doesn't have a subtype");
    return pointer();
}


unsigned
type_anything::get_size_in_bits(void)
    const
{
    assert(!"anything doesn't have a size in bits");
    return 0;
}


bool
type_anything::same_type_as(const type::pointer &)
    const
{
    // A whole class, just for this.
    return true;
}


bool
type_anything::is_a_variable(void)
    const
{
    return false;
}


bool
type_anything::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_anything::get_human_readable_representation_of_value(int)
    const
{
    assert(!"I told you not to call me when I'm at work");
    return "";
}


rcstring
type_anything::get_c_declaration(const rcstring &name)
    const
{
    return ("void " + name);
}


// vim: set ts=8 sw=4 et :
