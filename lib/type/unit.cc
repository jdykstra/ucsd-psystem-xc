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

#include <lib/ac/assert.h>

#include <lib/type/unit.h>


type_unit::~type_unit()
{
}


type_unit::type_unit()
{
}


type_unit::pointer
type_unit::create(void)
{
    static pointer singleton;
    if (!singleton)
        singleton = pointer(new type_unit());
    return singleton;
}


rcstring
type_unit::get_name(void)
    const
{
    return "unit";
}


rcstring
type_unit::get_pascal_name(void)
    const
{
    return get_name();
}


rcstring
type_unit::get_c_declaration(const rcstring &vname)
    const
{
    return ("unit_t " + vname);
}


unsigned
type_unit::get_size_in_bits(void)
    const
{
    return 0;
}


bool
type_unit::same_type_as(const type::pointer &)
    const
{
    return false;
}


bool
type_unit::is_a_variable()
    const
{
    return false;
}


type::pointer
type_unit::get_subtype(void)
    const
{
    assert(!"seriously?");
    return create();
}


bool
type_unit::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_unit::get_human_readable_representation_of_value(int)
    const
{
    assert(!"that's the funniest thing I've been asked all day");
    return rcstring();
}


bool
type_unit::is_a(const type::pointer &rhs)
{
    return !!dynamic_cast<const type_unit *>(rhs.get());
}


// vim: set ts=8 sw=4 et :
