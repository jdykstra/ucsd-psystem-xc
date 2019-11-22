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

#include <lib/symbol/typedef.h>


symbol_typedef::~symbol_typedef()
{
}


symbol_typedef::symbol_typedef(
    const variable_name &a_name,
    const type::pointer &a_type,
    int a_lex_level
) :
    symbol(a_name, a_type, a_lex_level)
{
}


symbol_typedef::pointer
symbol_typedef::create(const variable_name &a_name,
    const type::pointer &a_type, int a_lex_level)
{
    return pointer(new symbol_typedef(a_name, a_type, a_lex_level));
}


const char *
symbol_typedef::get_kind()
    const
{
    return "named type";
}


#include <lib/expression/error.h>
#include <lib/pascal/lex.h>


expression::pointer
symbol_typedef::name_expression_factory(const symbol::pointer &p,
    const location &where, int)
{
    assert(this == p.get());
    pascal_lex_error
    (
        where,
        "unable to reference type %s here",
        get_name().quote_c().c_str()
    );
    return expression_error::create(where);
}


// vim: set ts=8 sw=4 et :
