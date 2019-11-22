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

#include <lib/variable/name.h>

#include <ucsdpsys_compile/symbol/enumerated/compile.h>
#include <ucsdpsys_compile/expression/enum_tag/compile.h>


symbol_enumerated_compile::~symbol_enumerated_compile()
{
}


symbol_enumerated_compile::symbol_enumerated_compile(
    const variable_name &a_name,
    const type::pointer &a_tp,
    int a_lex_level,
    int a_value,
    translator_compile *a_context
) :
    symbol_enumerated(a_name, a_tp, a_lex_level, a_value),
    context(a_context)
{
}


symbol_enumerated_compile::pointer
symbol_enumerated_compile::create(const variable_name &a_name,
    const type::pointer &a_tp, int a_lex_level, int a_value,
    translator_compile *a_context)
{
    return
        pointer
        (
            new symbol_enumerated_compile
            (
                a_name,
                a_tp,
                a_lex_level,
                a_value,
                a_context
            )
        );
}


expression::pointer
symbol_enumerated_compile::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    //
    // Enumeration tags get inserted as constant integers in the code.
    //
    assert(this == sym.get());
    return
        expression_enum_tag_compile::create
        (
            variable_name(get_name(), where),
            get_type(),
            get_value(),
            context
        );
}


// vim: set ts=8 sw=4 et :
