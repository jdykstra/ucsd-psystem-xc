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

#include <ucsdpsys_assemble/type/linksym_relative.h>


type_linksym_relative::~type_linksym_relative()
{
}


type_linksym_relative::type_linksym_relative(const rcstring &a_name) :
    symbol_name(a_name)
{
}


type_linksym_relative::pointer
type_linksym_relative::create(const rcstring &a_name)
{
    return pointer(new type_linksym_relative(a_name));
}


bool
type_linksym_relative::is_a(const type::pointer &tp)
{
    return !!dynamic_cast<const type_linksym_relative *>(tp.get());
}


bool
type_linksym_relative::is_a(const expression::pointer &ep)
{
    return is_a(ep->get_type());
}


rcstring
type_linksym_relative::get_name(void)
    const
{
    return "linker-symbol-relative byte address";
}


rcstring
type_linksym_relative::get_pascal_name(void)
    const
{
    return "^";
}


rcstring
type_linksym_relative::get_c_declaration(const rcstring &nam)
    const
{
    return ("void *" + nam);
}


type::pointer
type_linksym_relative::get_subtype(void)
    const
{
    assert(!"there is no subtype");
}


unsigned
type_linksym_relative::get_size_in_bits(void)
    const
{
    return 16;
}


bool
type_linksym_relative::same_type_as(const type::pointer &rhs)
    const
{
    const type_linksym_relative *rhs2  =
        dynamic_cast<const type_linksym_relative *>(rhs.get());
    return (rhs2 && rhs2->symbol_name == symbol_name);
}


bool
type_linksym_relative::is_a_variable(void)
    const
{
    return false;
}


bool
type_linksym_relative::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_linksym_relative::get_human_readable_representation_of_value(int value)
    const
{
    assert(!"should not happen");
    return rcstring::printf("$%+d", value);
}


// vim: set ts=8 sw=4 et :
