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

#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/type/anything.h>
#include <lib/type/file.h>
#include <lib/type/reference.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/expression/nil/compile.h>
#include <ucsdpsys_compile/symbol/function/rewrite.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_rewrite::~symbol_function_rewrite()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_rewrite::symbol_function_rewrite(translator_compile *cntxt) :
    symbol_function("rewrite", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    //
    // procedure rewrite(var f: file; title: string);
    //
    name_type_list args;
    {
        variable_name_list names;
        names.push_back(variable_name("f", location::builtin()));
        type::pointer tp =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        names.push_back(variable_name("title", location::builtin()));
        type::pointer tp = type_string::create(255);
        args.push_back(name_type(names, tp));
    }

    attach_function_parameters(args);
    DEBUG(1, "}");
}


symbol_function_rewrite::pointer
symbol_function_rewrite::create(translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_rewrite(cntxt));
}


expression::pointer
symbol_function_rewrite::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_list params = check_parameters(rhs, code);
    if (params.empty())
        return expression_error::create(rhs.get_location());
    assert(params.size() == 3);

    //
    // Now build the expression node.
    //
    // The IOCHECK opcode is added by
    // expression_function_call_user_defined::post_order_traversal
    // ucsdpsys_compile/expression/function_call/user_defined.cc
    //
    expression_list rhs2;
    expression::pointer ep =
        code.name_expression(rhs[0]->get_location(), "sys:fopen");
    rhs2.push_back(ep);
    // var f: fib
    rhs2.push_back(params[1]);
    // var ftitle: string
    rhs2.push_back(params[2]);
    // fopenold: boolean
    rhs2.push_back
    (
        expression_boolean_compile::create
        (
            rhs.back()->get_location(),
            false,
            &code
        )
    );
    // junk: fibp
    rhs2.push_back
    (
        expression_nil_compile::create(rhs.back()->get_location(), &code)
    );

    return expression_function_call_user_defined::create(rhs2, &code);
}


expression::pointer
symbol_function_rewrite::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
