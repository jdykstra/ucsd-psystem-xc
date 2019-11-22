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
#include <lib/type/anything.h>
#include <lib/type/file.h>
#include <lib/type/reference.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/expression/nil/compile.h>
#include <ucsdpsys_compile/symbol/function/opennew.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_opennew::~symbol_function_opennew()
{
}


symbol_function_opennew::symbol_function_opennew(translator_compile *cntxt) :
    symbol_function("opennew", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // procedure opennew(var f: file; var name: string);
    //
    // BUT string values are only ever on the stack as pointers,
    // meaning that var string and string looks the same (they are
    // only different for the callee, not the caller) we write this
    // declaration instead as
    //
    // procedure opennew(var f: file; name: string);
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
}


symbol_function_opennew::pointer
symbol_function_opennew::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_opennew(cntxt));
}


expression::pointer
symbol_function_opennew::function_call_expression_factory(
    const expression_list &rhs) const
{
    expression_list rhs2 = check_parameters(rhs, code);
    if (rhs2.empty())
        return expression_error::create(rhs.get_location());
    assert(rhs2.size() == 3);

    //
    // Now build the expression node.
    //
    // The IOCHECK opcode is added by
    // expression_function_call_user_defined::post_order_traversal
    // ucsdpsys_compile/expression/function_call/user_defined.cc
    //
    rhs2.pop_front();
    expression::pointer ep =
        code.name_expression(rhs[0]->get_location(), "sys:fopen");
    rhs2.push_front(ep);

    // Now push the two extra parameters that sys:fopen needs.
    // fopennew: boolean
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
symbol_function_opennew::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
