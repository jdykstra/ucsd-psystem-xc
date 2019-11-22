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
#include <lib/expression/list.h>
#include <lib/type/integer.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/symbol/function/gotoxy.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_gotoxy::~symbol_function_gotoxy()
{
}


symbol_function_gotoxy::symbol_function_gotoxy(translator_compile *cntxt) :
    symbol_function("gotoxy", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // procedure gotoxy(x, y: integer);
    //
    name_type_list params;
    {
        variable_name_list names;
        names.push_back(variable_name("x", location::builtin()));
        names.push_back(variable_name("y", location::builtin()));
        params.push_back(name_type(names, type_integer::create()));
    }
    attach_function_parameters(params);
}


symbol_function_gotoxy::pointer
symbol_function_gotoxy::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_gotoxy(cntxt));
}


expression::pointer
symbol_function_gotoxy::function_call_expression_factory(
    const expression_list &rhs) const
{
    expression_list rhs2 = check_parameters(rhs, code);
    if (rhs2.empty())
        return expression_error::create(rhs.get_location());
    assert(rhs2.size() == 3);

    rhs2.pop_front();
    expression::pointer ep =
        code.name_expression(rhs[0]->get_location(), "sys:fgotoxy");
    rhs2.push_front(ep);

    return expression_function_call_user_defined::create(rhs2, &code);
}


expression::pointer
symbol_function_gotoxy::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
