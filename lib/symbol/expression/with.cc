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

#include <lib/symbol/expression/with.h>


symbol_expression_with::~symbol_expression_with()
{
}


symbol_expression_with::symbol_expression_with(
    const variable_name &a_name,
    int a_lex_level,
    const expression::pointer &a_value
) :
    symbol_expression(a_name, a_lex_level, a_value)
{
}


symbol_expression_with::pointer
symbol_expression_with::create(const variable_name &a_name, int a_lex_level,
    const expression::pointer &a_value)
{
    return pointer(new symbol_expression_with(a_name, a_lex_level, a_value));
}


const char *
symbol_expression_with::get_kind()
    const
{
    return "with record field";
}


// vim: set ts=8 sw=4 et :
