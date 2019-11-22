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
#include <lib/expression/empty.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/type/anything.h>
#include <lib/type/char.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>
#include <lib/type/reference.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/fillchar.h>
#include <ucsdpsys_compile/symbol/function/fillchar.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_fillchar::~symbol_function_fillchar()
{
}


symbol_function_fillchar::symbol_function_fillchar(translator_compile *cntxt) :
    symbol_function("fillchar", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // procedure fillchar(var destination: anything; count: integer;
    //     character: char);
    //
    name_type_list args;
    {
        variable_name_list names;
        variable_name vn("destination", location::builtin());
        names.push_back(vn);
        type::pointer tp = type_reference::create(type_anything::create());
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        variable_name vn("count", location::builtin());
        names.push_back(vn);
        type::pointer tp = type_integer::create();
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        variable_name vn("character", location::builtin());
        names.push_back(vn);
        type::pointer tp = type_char::create();
        args.push_back(name_type(names, tp));
    }
    attach_function_parameters(args);
    // attach_return_type(type_nothing::create());
}


symbol_function_fillchar::pointer
symbol_function_fillchar::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_fillchar(cntxt));
}


expression::pointer
symbol_function_fillchar::function_call_expression_factory(
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

    // var dest: anything
    {
        expression::pointer e1 = code.call_noparam_func_if_necessary(rhs[1]);
        DEBUG(2, "arg 1 = %s", e1->lisp_representation().c_str());
        expression::pointer ep = e1->get_byte_pointer();
        if (!ep)
        {
            pascal_lex_error
            (
                e1->get_location(),
                "procedure %s: parameter one: must be a packed array of char, "
                    "not a %s",
                get_name().c_str(),
                e1->get_description().c_str()
            );
            DEBUG(1, "}");
            return expression_error::create(e1->get_location());
        }
        args.push_back(ep);
    }

    // count: integer
    {
        expression::pointer ep = code.call_noparam_func_if_necessary(rhs[2]);
        DEBUG(2, "arg 2 = %s", ep->lisp_representation().c_str());
        if (!type_integer::is_a(ep))
        {
            parameter_error_wrong_type(2, ep, type_integer::create());
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        args.push_back(ep);

        if (ep->is_constant())
        {
            int value = ep->optimize()->get_integer_value();
            if (value <= 0)
            {
                // do not issue a warning here, the "expression
                // statement" will do that shortly.
                return expression_empty::create(rhs.get_location());
            }
        }
    }

    // character: char
    //
    // (Well, it may be *documented* as char, but the UCSD native
    // compiler actually accepted integer and enums as well.)
    {
        expression::pointer ep = code.call_noparam_func_if_necessary(rhs[3]);
        {
            expression::pointer ep2 = code.char_from_string_of_length_one(ep);
            if (ep2)
                ep = ep2;
        }
        if
        (
            !type_char::is_a(ep)
        &&
            !type_integer::is_a(ep)
        &&
            !type_enumerated::is_a(ep)
        )
        {
            parameter_error_wrong_type(3, ep, type_char::create());
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }
        args.push_back(ep);
    }

    //
    // Now build the expression node.
    //
    return expression_function_call_fillchar::create(args, &code);
}


expression::pointer
symbol_function_fillchar::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
