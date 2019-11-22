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

#include <lib/symbol/enumerated.h>


symbol_enumerated::~symbol_enumerated()
{
}


symbol_enumerated::symbol_enumerated(
    const rcstring &a_name,
    const type::pointer &a_type,
    int a_lex_level,
    int a_value
) :
    symbol(a_name, a_type, a_lex_level),
    value(a_value)
{
}


symbol_enumerated::symbol_enumerated(
    const variable_name &a_name,
    const type::pointer &a_type,
    int a_lex_level,
    int a_value
) :
    symbol(a_name, a_type, a_lex_level),
    value(a_value)
{
}


const char *
symbol_enumerated::get_kind()
    const
{
    return "enumerated constant";
}


// vim: set ts=8 sw=4 et :
