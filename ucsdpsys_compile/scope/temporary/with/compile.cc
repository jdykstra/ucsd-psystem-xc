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

#include <ucsdpsys_compile/scope/temporary/with/compile.h>
#include <ucsdpsys_compile/symbol/variable/normal/compile.h>
#include <ucsdpsys_compile/symbol/variable/normal/global.h>
#include <ucsdpsys_compile/translator/compile.h>


scope_temporary_with_compile::~scope_temporary_with_compile()
{
}


scope_temporary_with_compile::scope_temporary_with_compile(
    scope_stack &a_stk,
    const expression::pointer &a_base,
    const statement::pointer &a_assign,
    translator_compile *a_context
) :
    scope_temporary_with(a_stk, a_base, a_assign, a_context),
    context(a_context)
{
}


scope_temporary_with_compile::pointer
scope_temporary_with_compile::create(scope_stack &a_stk,
    const expression::pointer &a_base, const statement::pointer &a_assign,
    translator_compile *a_context)
{
    return
        pointer
        (
            new scope_temporary_with_compile(a_stk, a_base, a_assign, a_context)
        );
}


symbol::pointer
scope_temporary_with_compile::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    return symbol_expression_constant::create(vname, lex_level, value);
}


symbol_variable::pointer
scope_temporary_with_compile::parameter_factory(const variable_name &,
    const type::pointer &, int)
{
    assert(!"cant have parameters here");
}


symbol_variable::pointer
scope_temporary_with_compile::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int lxlvl)
{
    if (lxlvl == 0)
    {
        // This would make perfect sense as a local variable.  We are only
        // doing this for compatibility with the UCSD native compiler.
        return
            symbol_variable_normal_global::create
            (
                vname,
                vtype,
                lxlvl,
                symbol_variable::kind_variable,
                context
            );
    }
    else
    {
        return
            symbol_variable_normal_compile::create
            (
                vname,
                vtype,
                lxlvl,
                symbol_variable::kind_variable,
                context
            );
    }
}


// vim: set ts=8 sw=4 et :
