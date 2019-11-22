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

#include <ucsdpsys_assemble/type/special.h>


type_special::~type_special()
{
}


type_special::type_special()
{
}


type_special::pointer
type_special::create(void)
{
    return pointer(new type_special());
}


rcstring
type_special::get_name(void)
    const
{
    return "special";
}


rcstring
type_special::get_pascal_name(void)
    const
{
    return get_name();
}


rcstring
type_special::get_c_declaration(const rcstring &)
    const
{
    return get_name();
}


type::pointer
type_special::get_subtype(void)
    const
{
    return create();
}


unsigned
type_special::get_size_in_bits(void)
    const
{
    return 0;
}


bool
type_special::same_type_as(const type::pointer &)
    const
{
    return false;
}


bool
type_special::is_a_variable(void)
    const
{
    return false;
}


bool
type_special::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_special::get_human_readable_representation_of_value(int)
    const
{
    return "oops";
}


// vim: set ts=8 sw=4 et :
