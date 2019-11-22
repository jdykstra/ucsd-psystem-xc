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

#include <ucsdpsys_littoral/expression/name/littoral.h>
#include <ucsdpsys_littoral/symbol/variable/normal/littoral.h>


symbol_variable_normal_littoral::~symbol_variable_normal_littoral()
{
}


symbol_variable_normal_littoral::symbol_variable_normal_littoral(
    const variable_name &a_name,
    const type::pointer &a_tp,
    int a_lex_level,
    kind_t a_kind,
    translator_littoral *a_context
) :
    symbol_variable_normal(a_name, a_tp, a_lex_level, a_kind),
    context(a_context)
{
}


symbol_variable_normal_littoral::pointer
symbol_variable_normal_littoral::create(const variable_name &a_name,
    const type::pointer &a_tp, int a_lex_level, kind_t a_kind,
    translator_littoral *a_context)
{
    return
        pointer
        (
            new symbol_variable_normal_littoral
            (
                a_name,
                a_tp,
                a_lex_level,
                a_kind,
                a_context
            )
        );
}


expression::pointer
symbol_variable_normal_littoral::name_expression_factory(
    const symbol::pointer &sp, const location &ref_locn, int)
{
    assert(this == sp.get());
    return expression_name_littoral::create(ref_locn, sp, context);
}


// vim: set ts=8 sw=4 et :
