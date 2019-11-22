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

#include <lib/cardinal.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/char/compile.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/subtraction/integer.h>
#include <ucsdpsys_compile/symbol/function/pred.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_pred::~symbol_function_pred()
{
}


symbol_function_pred::symbol_function_pred(
    const rcstring &a_name,
    translator_compile *cntxt
) :
    symbol_function(a_name, location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // function pred(value: integer): integer;
    // function pred(value: integer, amount: integer): integer;
    // ... sort of
    //
    variable_name_list names;
    names.push_back(variable_name("value", location::builtin()));
    name_type_list args;
    args.push_back(name_type(names, type_integer::create()));
    attach_function_parameters(args);
    attach_return_type(type_integer::create());
}


symbol_function_pred::pointer
symbol_function_pred::create(const rcstring &a_name, translator_compile *cntxt)
{
    return pointer(new symbol_function_pred(a_name, cntxt));
}


expression::pointer
symbol_function_pred::function_call_expression_factory(
    const expression_list &rhs) const
{
    switch (rhs.size())
    {
    case 2:
    case 3:
        break;

    default:
        parameter_error_wrong_number(rhs, 1);
        return expression_error::create(rhs.get_location());
    }

    expression::pointer e1 = rhs.get(1);

    //
    // string constants of length one are char constants in this case
    //
    {
        expression::pointer e1c = code.char_from_string_of_length_one(e1);
        if (e1c)
            e1 = e1c;
    }

    e1 = code.call_noparam_func_if_necessary(e1);

    if
    (
        !type_boolean::is_a(e1)
    &&
        !type_char::is_a(e1)
    &&
        !type_enumerated::is_a(e1)
    &&
        !type_integer::is_a(e1)
    )
    {
        pascal_lex_error
        (
            e1->get_location(),
            "function %s: parameter one: must be an enumerated type, "
                "not a %s",
            get_name().c_str(),
            e1->get_description().c_str()
        );
        return expression_error::create(rhs.get_location());
    }
    expression::pointer e2 =
        expression_integer_compile::create
        (
            rhs.back()->get_location(),
            1,
            &code
        );
    if (rhs.size() >= 3)
    {
        e2 = rhs.get(2);
        e2 = code.call_noparam_func_if_necessary(e2);
        if (!type_integer::is_a(e2))
        {
            parameter_error_wrong_type(2, e2, type_integer::create());
            return expression_error::create(rhs.get_location());
        }
    }

    //
    // Now build the expression node.
    //
    return expression_subtraction_integer::create(e1, e2, &code);
}


expression::pointer
symbol_function_pred::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
