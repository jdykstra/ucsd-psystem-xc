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
#include <lib/pascal/lex.h>
#include <lib/type/anything.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/reference.h>

#include <ucsdpsys_compile/symbol/function/release.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/release.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_release::~symbol_function_release()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_release::symbol_function_release(translator_compile *cntxt) :
    symbol_function("release", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    //
    // procedure release(var pointer: ^anything);
    //
    variable_name_list names;
    names.push_back(variable_name("pointer", location::builtin()));
    type::pointer tp =
        type_reference::create
        (
            type_pointer_unpacked::create(type_anything::create())
        );
    name_type_list args;
    args.push_back(name_type(names, tp));
    attach_function_parameters(args);
    // attach_return_type(type_nothing::create());
    DEBUG(1, "}");
}


symbol_function_release::pointer
symbol_function_release::create(translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_release(cntxt));
}


expression::pointer
symbol_function_release::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "rhs.size = %d", (int)rhs.size());

    //
    // Checking for number and types of parameters.
    //
    if (rhs.size() != 2)
    {
        parameter_error_wrong_number(rhs, 1);
        DEBUG(1, "}");
        return expression_error::create(rhs.get_location());
    }

    expression::pointer ep = code.call_noparam_func_if_necessary(rhs[1]);
    DEBUG(2, "ep = %s", ep->lisp_representation().c_str());
    if (!type_pointer::is_a(ep->get_type()))
    {
        oops:
        pascal_lex_error
        (
            ep->get_location(),
            "procedure release: parameter one: must be a pointer variable, "
                "not a %s",
            ep->get_description().c_str()
        );
        DEBUG(1, "}");
        return expression_error::create(ep->get_location());
    }

    //
    // Now build the expression node.
    //
    ep = ep->strip_indirection();
    if (!ep)
        goto oops;
    expression_list args;
    args.push_back(ep);
    expression::pointer result =
        expression_function_call_release::create(args, &code);
    DEBUG(1, "}");
    return result;
}


expression::pointer
symbol_function_release::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
