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

#include <lib/symbol/expression/constant.h>

#include <ucsdpsys_compile/scope/builtin/compile.h>
#include <ucsdpsys_compile/symbol/variable/normal/compile.h>


scope_builtin_compile::~scope_builtin_compile()
{
}


scope_builtin_compile::scope_builtin_compile(translator_compile *a_context) :
    context(*a_context)
{
}


scope_builtin_compile::pointer
scope_builtin_compile::create(translator_compile *a_context)
{
    return pointer(new scope_builtin_compile(a_context));
}


symbol_variable::pointer
scope_builtin_compile::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    return
        symbol_variable_normal_compile::create
        (
            vname,
            vtype,
            lex_level,
            symbol_variable::kind_variable,
            &context
        );
}


symbol_variable::pointer
scope_builtin_compile::parameter_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    return
        symbol_variable_normal_compile::create
        (
            vname,
            vtype,
            lex_level,
            symbol_variable::kind_parameter,
            &context
        );
}


symbol::pointer
scope_builtin_compile::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    return symbol_expression_constant::create(vname, lex_level, value);
}


// vim: set ts=8 sw=4 et :
