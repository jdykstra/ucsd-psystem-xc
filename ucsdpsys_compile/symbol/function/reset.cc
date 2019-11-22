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

#include <lib/cardinal.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/type/anything.h>
#include <lib/type/file.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/expression/nil/compile.h>
#include <ucsdpsys_compile/symbol/function/reset.h>
#include <ucsdpsys_compile/symbol/function/user_defined.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_reset::~symbol_function_reset()
{
}


symbol_function_reset::symbol_function_reset(translator_compile *cntxt) :
    symbol_function("reset", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // Both of...
    // procedure reset(f: file);
    // procedure reset(f: file; name: string);
    //
    variable_name_list names;
    names.push_back(variable_name("f", location::builtin()));
    name_type_list args;
    args.push_back
    (
        name_type(names, type_file::create(type_anything::create()))
    );
    attach_function_parameters(args);
}


symbol_function_reset::pointer
symbol_function_reset::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_reset(cntxt));
}


expression::pointer
symbol_function_reset::function_call_expression_factory(
    const expression_list &rhs) const
{
    expression::pointer p2;
    expression::pointer p1;
    switch (rhs.size())
    {
    case 3:
        // second parameter
        p2 = code.call_noparam_func_if_necessary(rhs[2]);
        if (!type_string::is_a(p2))
        {
            pascal_lex_error
            (
                p2->get_location(),
                "function %s: parameter two: must be a string, "
                    "not a %s",
                get_name().c_str(),
                p2->get_description().c_str()
            );
            return expression_error::create(p2->get_location());
        }
        // Fall through...

    case 2:
        // first parameter
        p1 = code.call_noparam_func_if_necessary(rhs[1]);
        if (!type_file::is_a(p1))
        {
            pascal_lex_error
            (
                p1->get_location(),
                "function %s: parameter one: must be an enumerated type, "
                    "not a %s",
                get_name().c_str(),
                p1->get_description().c_str()
            );
            return expression_error::create(rhs.get_location());
        }
        break;

    default:
        parameter_error_wrong_number(rhs, (rhs.size() < 2 ? 1 : 2));
        return expression_error::create(rhs.get_location());
    }
    assert(p1);

    //
    // Now build the expression node.
    //
    // The IOCHECK opcode is added by
    // expression_function_call_user_defined::post_order_traversal
    // ucsdpsys_compile/expression/function_call/user_defined.cc
    //
    expression_list rhs2;
    if (p2)
    {
        expression::pointer ep =
            code.name_expression(rhs[0]->get_location(), "sys:fopen");
        rhs2.push_back(ep);
        // var f: fib
        rhs2.push_back(p1->strip_indirection());
        // var ftitle: string
        rhs2.push_back(p2);
        // fopenold: boolean
        rhs2.push_back
        (
            expression_boolean_compile::create
            (
                rhs.back()->get_location(),
                true,
                &code
            )
        );
        // junk: fibp
        rhs2.push_back
        (
            expression_nil_compile::create(rhs.back()->get_location(), &code)
        );
    }
    else
    {
        expression::pointer ep =
            code.name_expression(rhs[0]->get_location(), "sys:freset");
        rhs2.push_back(ep);
        // var f: fib
        rhs2.push_back(p1->strip_indirection());
    }
    return expression_function_call_user_defined::create(rhs2, &code);
}


expression::pointer
symbol_function_reset::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
