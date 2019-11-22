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

#include <lib/symbol/expression/constant.h>


symbol_expression_constant::~symbol_expression_constant()
{
}


symbol_expression_constant::symbol_expression_constant(
    const variable_name &a_name,
    int lxlvl,
    const expression::pointer &a_value
) :
    symbol_expression(a_name, lxlvl, a_value)
{
}


symbol_expression_constant::pointer
symbol_expression_constant::create(const variable_name &a_name, int lxlvl,
    const expression::pointer &a_value)
{
    return pointer(new symbol_expression_constant(a_name, lxlvl, a_value));
}


const char *
symbol_expression_constant::get_kind()
    const
{
    return "constant";
}


// vim: set ts=8 sw=4 et :
