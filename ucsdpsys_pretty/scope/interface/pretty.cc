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

#include <ucsdpsys_pretty/scope/interface/pretty.h>
#include <ucsdpsys_pretty/symbol/expression/constant/pretty.h>
#include <ucsdpsys_pretty/symbol/variable/normal/pretty.h>


scope_interface_pretty::~scope_interface_pretty()
{
}


scope_interface_pretty::scope_interface_pretty(
    const rcstring &a_name,
    translator_pretty *a_context
) :
    scope_interface(a_name),
    context(a_context)
{
}


scope_interface_pretty::pointer
scope_interface_pretty::create(const rcstring &a_name,
    translator_pretty *a_context)
{
    return pointer(new scope_interface_pretty(a_name, a_context));
}


symbol::pointer
scope_interface_pretty::constant_factory(const variable_name &vname,
    int lex_level, const expression::pointer &value)
{
    return
        symbol_expression_constant_pretty::create
        (
            vname,
            lex_level,
            value,
            context
        );
}


symbol_variable::pointer
scope_interface_pretty::parameter_factory(const variable_name &,
    const type::pointer &, int)
{
    assert(!"can't have a parameter here");
}


symbol_variable::pointer
scope_interface_pretty::variable_factory(const variable_name &vname,
    const type::pointer &vtype, int lex_level)
{
    return
        symbol_variable_normal_pretty::create
        (
            vname,
            vtype,
            lex_level,
            symbol_variable::kind_variable,
            context
        );
}


// vim: set ts=8 sw=4 et :
