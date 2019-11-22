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
#include <lib/expression/empty.h>
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/type/anything.h>
#include <lib/type/integer.h>
#include <lib/type/reference.h>

#include <ucsdpsys_compile/expression/load/indirect_packed.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/moveright.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/symbol/function/moveright.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_moveright::~symbol_function_moveright()
{
}


symbol_function_moveright::symbol_function_moveright(
    translator_compile *cntxt
) :
    symbol_function("moveright", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // procedure moveright(var src; var dest; count: integer);
    //
    name_type_list args;
    {
        variable_name_list names;
        variable_name vn("source", location::builtin());
        names.push_back(vn);
        type::pointer tp = type_reference::create(type_anything::create());
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        variable_name vn("destination", location::builtin());
        names.push_back(vn);
        type::pointer tp = type_reference::create(type_anything::create());
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        variable_name vn("length", location::builtin());
        names.push_back(vn);
        type::pointer tp = type_integer::create();
        args.push_back(name_type(names, tp));
    }
    attach_function_parameters(args);
    // attach_return_type(type_nothing::create());
}


symbol_function_moveright::pointer
symbol_function_moveright::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_moveright(cntxt));
}


expression::pointer
symbol_function_moveright::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    if (rhs.size() != 4)
    {
        parameter_error_wrong_number(rhs, 3);
        DEBUG(1, "}");
        return expression_error::create(rhs.get_location());
    }

    expression_list args;

    // var src: packed array of char
    // or, it could be a[n]
    {
        expression::pointer a1 = code.call_noparam_func_if_necessary(rhs[1]);
        DEBUG(2, "arg 1 = %s", a1->lisp_representation().c_str());
        expression::pointer ep = a1->get_byte_pointer();
        if (!ep)
        {
            pascal_lex_error
            (
                a1->get_location(),
                "procedure %s: parameter one: must be a packed array of char, "
                    "not a %s",
                get_name().c_str(),
                a1->get_description().c_str()
            );
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        args.push_back(ep);
    }

    // var dest: packed array of char
    // or, it could be a[n]
    {
        expression::pointer a2 = code.call_noparam_func_if_necessary(rhs[2]);
        DEBUG(2, "arg 2 = %s", a2->lisp_representation().c_str());
        expression::pointer ep = a2->get_byte_pointer();
        if (!ep)
        {
            pascal_lex_error
            (
                a2->get_location(),
                "procedure %s: parameter two: must be a packed array of char, "
                    "not a %s",
                get_name().c_str(),
                a2->get_description().c_str()
            );
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        args.push_back(ep);
    }

    {
        expression::pointer ep = code.call_noparam_func_if_necessary(rhs[3]);
        if (!type_integer::is_a(ep))
        {
            parameter_error_wrong_type(3, ep, type_integer::create());
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        args.push_back(ep);

        if (ep->is_constant())
        {
            int value = ep->optimize()->get_integer_value();
            if (value <= 0)
            {
                // Do not issue a warning here, the "expression
                // statement" code will do it shortly.
                return expression_empty::create(rhs.get_location());
            }
        }
    }

    //
    // Now build the expression node.
    //
    return expression_function_call_moveright::create(args, &code);
}


expression::pointer
symbol_function_moveright::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
