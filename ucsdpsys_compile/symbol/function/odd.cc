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

#include <lib/expression/error.h>
#include <lib/type/integer.h>
#include <lib/type/boolean.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/odd.h>
#include <ucsdpsys_compile/symbol/function/odd.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_odd::~symbol_function_odd()
{
}


symbol_function_odd::symbol_function_odd(translator_compile *cntxt) :
    symbol_function("odd", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // procedure odd(var high, low: integer);
    //
    variable_name_list names;
    names.push_back(variable_name("number", location::builtin()));
    type::pointer tp = type_integer::create();
    name_type_list args;
    args.push_back(name_type(names, tp));
    attach_function_parameters(args);
    attach_return_type(type_boolean::create());
}


symbol_function_odd::pointer
symbol_function_odd::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_odd(cntxt));
}


expression::pointer
symbol_function_odd::function_call_expression_factory(
    const expression_list &rhs) const
{
    //
    // Checking for number and types of parameters.
    // (this does automatic promotions as well)
    //
    expression_list args = check_parameters(rhs, code);
    if (args.empty())
        return expression_error::create(rhs.get_location());
    assert(args.size() == 2);

    //
    // Now build the expression node.
    //
    return expression_function_call_odd::create(args.tail(), &code);
}


expression::pointer
symbol_function_odd::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
