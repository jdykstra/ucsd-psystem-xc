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

#include <lib/symbol/variable.h>


symbol_variable::~symbol_variable()
{
}


symbol_variable::symbol_variable(
    const variable_name &a_name,
    const type::pointer &a_type,
    int a_lex_level,
    kind_t a_kind
) :
    symbol(a_name, a_type, a_lex_level),
    definition(a_name),
    kind(a_kind)
{
}


const char *
symbol_variable::get_kind()
    const
{
    switch (kind)
    {
    case kind_parameter:
        return "parameter";

    case kind_record_field:
        return "record field";

    case kind_variable:
    default:
        return "variable";
    }
}


// vim: set ts=8 sw=4 et :
