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
#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/type/anything.h>
#include <lib/type/integer.h>
#include <lib/type/reference.h>

#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/unitstatus.h>
#include <ucsdpsys_compile/expression/load/indirect_packed.h>
#include <ucsdpsys_compile/symbol/function/unitstatus.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_unitstatus::~symbol_function_unitstatus()
{
}


symbol_function_unitstatus::symbol_function_unitstatus(
    translator_compile *cntxt
) :
    symbol_function("unitstatus", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // procedure unitstatus(unitnumber: integer; var result: anything;
    //     result_size: integer);
    //
    name_type_list args;
    {
        variable_name_list names;
        variable_name vn("unit_number", location::builtin());
        names.push_back(vn);
        type::pointer tp = type_integer::create();
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        variable_name vn("result", location::builtin());
        names.push_back(vn);
        type::pointer tp = type_reference::create(type_anything::create());
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        variable_name vn("result_size", location::builtin());
        names.push_back(vn);
        type::pointer tp = type_integer::create();
        args.push_back(name_type(names, tp));
    }
    attach_function_parameters(args);
    // attach_return_type(type_nothing::create());
}


symbol_function_unitstatus::pointer
symbol_function_unitstatus::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_unitstatus(cntxt));
}


expression::pointer
symbol_function_unitstatus::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    if (rhs.size() != 4)
    {
        parameter_error_wrong_number(rhs, 3);
        DEBUG(1, "}");
        return expression_error::create(rhs.get_location());
    }

    expression_list params;

    // unit_number: integer
    {
        expression::pointer p1 = code.call_noparam_func_if_necessary(rhs[1]);
        DEBUG(2, "arg 1 = %s", p1->lisp_representation().c_str());
        if (!type_integer::is_a(p1))
        {
            parameter_error_wrong_type(1, p1, type_integer::create());
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        params.push_back(p1);
    }

    // var result: packed array of char
    // or, it could be a[n]
    // or, it could be a simple variable (usually an integer)
    {
        expression::pointer p2 = code.call_noparam_func_if_necessary(rhs[2]);
        DEBUG(2, "arg 2 = %s", p2->lisp_representation().c_str());
        expression::pointer ep = p2->get_byte_pointer();
        if (!ep)
        {
            pascal_lex_error
            (
                p2->get_location(),
                "function %s: parameter two: must be a packed array of char, "
                    "not a %s",
                get_name().c_str(),
                p2->get_description().c_str()
            );
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        params.push_back(ep);
    }

    // result_size: integer
    {
        expression::pointer p3 = code.call_noparam_func_if_necessary(rhs[3]);
        DEBUG(2, "arg 3 = %s", p3->lisp_representation().c_str());
        if (!type_integer::is_a(p3))
        {
            parameter_error_wrong_type(3, p3, type_integer::create());
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        params.push_back(p3);
    }

    //
    // Build the function call expression node.
    //
    expression::pointer result =
        expression_function_call_unitstatus::create(params, &code);
    DEBUG(1, "}");
    return result;
}


expression::pointer
symbol_function_unitstatus::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
