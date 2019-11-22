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

#include <lib/symbol/expression.h>
#include <lib/type/integer.h>


symbol_expression::~symbol_expression()
{
}


static type::pointer
sanitize_type(const expression::pointer &a_value)
{
    if (!a_value)
        return type_integer::create();
    return a_value->get_type();
}


symbol_expression::symbol_expression(
    const variable_name &a_name,
    int lxlvl,
    const expression::pointer &a_value
) :
    symbol(a_name, sanitize_type(a_value), lxlvl),
    value(a_value)
{
}


symbol_expression::symbol_expression(
    const variable_name &a_name,
    const type::pointer &a_type,
    int lxlvl,
    const expression::pointer &a_value
) :
    symbol(a_name, a_type, lxlvl),
    value(a_value)
{
    assert(!a_value || a_type == a_value->get_type());
}


void
symbol_expression::set_value(const expression::pointer &new_value)
{
    assert(new_value);
    value = new_value;
    change_type(value->get_type());
}


expression::pointer
symbol_expression::name_expression_factory(const symbol::pointer &sp,
        const location &where, int)
{
    assert(this == sp.get());
    return value->clone(where);
}


// vim: set ts=8 sw=4 et :
