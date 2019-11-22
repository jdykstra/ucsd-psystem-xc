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

#include <lib/ac/math.h>

#include <lib/cardinal.h>
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/type/real.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/ln.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/multiplication/real.h>
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/symbol/function/log.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_log::~symbol_function_log()
{
}


symbol_function_log::symbol_function_log(translator_compile *cntxt) :
    symbol_function("log", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // function log(x: real): real;
    //
    variable_name_list names;
    names.push_back(variable_name("x", location::builtin()));
    type::pointer tp = type_real::create();
    name_type_list args;
    args.push_back(name_type(names, tp));
    attach_function_parameters(args);
    attach_return_type(type_real::create());
}


symbol_function_log::pointer
symbol_function_log::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_log(cntxt));
}


expression::pointer
symbol_function_log::function_call_expression_factory(
    const expression_list &rhs) const
{
    expression_list rhs2 = check_parameters(rhs, code);
    if (rhs2.empty())
        return expression_error::create(rhs.get_location());
    assert(rhs2.size() == 2);

    //
    // Now build the expression tree.
    //
    expression::pointer e2 =
        expression_function_call_ln::create(rhs2.tail(), &code);
    expression::pointer e1 =
        expression_real_compile::create(rhs.get_location(), M_LOG10E, &code);
    expression::pointer e3 =
        expression_multiplication_real::create(e1, e2, &code);
    return e3;
}


expression::pointer
symbol_function_log::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
