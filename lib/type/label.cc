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

#include <lib/type/label.h>


type_label::~type_label()
{
}


type_label::type_label()
{
}


type_label::pointer
type_label::create(void)
{
    static pointer singleton;
    if (!singleton)
        singleton = pointer(new type_label());
    return singleton;
}


rcstring
type_label::get_name(void)
    const
{
    return "label";
}


rcstring
type_label::get_pascal_name(void)
    const
{
    return "label";
}


rcstring
type_label::get_c_declaration(const rcstring &vname)
    const
{
    return ("label_t " + vname);
}


type::pointer
type_label::get_subtype(void)
    const
{
    assert(!"labels don't have a subtype");
}


unsigned
type_label::get_size_in_bits(void)
    const
{
    return 0;
}


bool
type_label::same_type_as(const type::pointer &rhs)
    const
{
    // Don't worry about matching "anything" as well,
    // because the == operator took care of it.
    return !!dynamic_cast<const type_label *>(rhs.get());
}


bool
type_label::is_a_variable(void)
    const
{
    return false;
}


bool
type_label::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_label::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :
