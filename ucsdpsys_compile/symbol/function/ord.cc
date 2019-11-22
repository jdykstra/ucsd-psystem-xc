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
#include <lib/type/pointer.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/cast.h>
#include <ucsdpsys_compile/expression/char/compile.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/symbol/function/ord.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_ord::~symbol_function_ord()
{
}


symbol_function_ord::symbol_function_ord(translator_compile *cntxt) :
    symbol_function("ord", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // function ord(value: anything): integer;
    // ... sort of
    //
    variable_name_list names;
    names.push_back(variable_name("value", location::builtin()));
    type::pointer tp = type_integer::create();
    name_type_list args;
    args.push_back(name_type(names, tp));
    attach_function_parameters(args);
    attach_return_type(type_char::create());
}


symbol_function_ord::pointer
symbol_function_ord::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_ord(cntxt));
}


expression::pointer
symbol_function_ord::function_call_expression_factory(
    const expression_list &rhs) const
{
    if (rhs.size() != 2)
    {
        parameter_error_wrong_number(rhs, 1);
        return expression_error::create(rhs.get_location());
    }
    expression::pointer e1 = rhs.get(1);

    //
    // Constant strings of length one are interpreted as char constants
    // in this context.
    //
    {
        expression::pointer e1c = code.char_from_string_of_length_one(e1);
        if (e1c)
            e1 = e1c;
    }

    // Call functions with no parameters.
    e1 = code.call_noparam_func_if_necessary(e1);

    //
    // See if they are using the ord/odd hack, and warn them if so.
    //
    if (type_boolean::is_a(e1))
    {
        expression::pointer e1a = e1->ord_odd_hack();
        if (e1a)
        {
            static bool long_warning;
            if (long_warning)
            {
                pascal_lex_warning
                (
                    e1->get_location(),
                    "this expression appears to be using the ord/odd hack"
                );
            }
            else
            {
                long_warning = true;
                pascal_lex_warning
                (
                    e1->get_location(),
                    "this expression appears to be using the ord/odd hack to "
                    "gain access to the bit-wise opcodes; this cross compiler "
                    "(and the ucsd-psystem-os native compiler) support integer "
                    "parameters to the \"and\", \"or\" and \"not\" operators, "
                    "allowing bit-wise operations without this ugly hack"
                );
            }
            return e1a;
        }
    }

    if (e1->is_error())
        return e1;

    if
    (
        !type_boolean::is_a(e1)
    &&
        !type_char::is_a(e1)
    &&
        !type_enumerated::is_a(e1)
    &&
        !type_integer::is_a(e1)
    &&
        // This seems oddly inconsistent, in a language as intent as Pascal
        // is, with the protection of the programmer from his own folly.
        !type_pointer::is_a(e1)
    )
    {
        pascal_lex_error
        (
            e1->get_location(),
            "function %s: parameter one: must be an enumerated type, not a %s",
            get_name().c_str(),
            e1->get_description().c_str()
        );
        return expression_error::create(e1->get_location());
    }

    //
    // Now build the expression node.
    //
    return expression_cast::create(type_integer::create(), e1, &code);
}


expression::pointer
symbol_function_ord::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
