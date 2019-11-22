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

#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/symbol/unit.h>
#include <lib/type/unit.h>


symbol_unit::~symbol_unit()
{
}


symbol_unit::symbol_unit(
    const rcstring &a_name,
    const scope::pointer &a_sp,
    int a_lxl
) :
    symbol(a_name, type_unit::create(), a_lxl),
    sp(a_sp)
{
}


symbol_unit::pointer
symbol_unit::create(const rcstring &a_name, const scope::pointer &a_sp,
    int a_lxl)
{
    return pointer(new symbol_unit(a_name, a_sp, a_lxl));
}


const char *
symbol_unit::get_kind(void)
    const
{
    return "unit";
}


expression::pointer
symbol_unit::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    pascal_lex_error
    (
        where,
        "may not reference unit %s as a variable",
        get_name().quote_c().c_str()
    );
    return expression_error::create(where);
}


// vim: set ts=8 sw=4 et :
