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

#include <lib/ac/string.h>

#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/type/integer.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/symbol/function/pos.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_pos::~symbol_function_pos()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_pos::symbol_function_pos(translator_compile *a_code) :
    symbol_function("pos", location::builtin(), scope::pointer(), -1),
    code(*a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);

    name_type_list args;
    {
        variable_name_list names;
        names.push_back(variable_name("needle", location::builtin()));
        // This is actually a limitation of the p-System implementation.
        type::pointer tp = type_string::create(80);
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        names.push_back(variable_name("haystack", location::builtin()));
        type::pointer tp = type_string::create(255);
        args.push_back(name_type(names, tp));
    }
    attach_function_parameters(args);
}


symbol_function_pos::pointer
symbol_function_pos::create(translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_pos(a_code));
}


expression::pointer
symbol_function_pos::function_call_expression_factory(
    const expression_list &rhs) const
{
    //
    // Verify the function parameters.
    //
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_list rhs2 = check_parameters(rhs, code);
    if (rhs2.empty())
        return expression_error::create(rhs.get_location());
    assert(rhs2.size() == 3);

    //
    // Look for constant parameters.
    //
    if (rhs2[1]->is_constant())
    {
        rcstring needle = rhs2[1]->optimize()->get_string_value();
        if (needle.empty())
        {
            return
                expression_integer_compile::create
                (
                    rhs.get_location(),
                    0,
                    &code
                );
        }

        if (rhs2[2]->is_constant())
        {
            rcstring haystack = rhs2[2]->optimize()->get_string_value();
            const char *cp = strstr(haystack.c_str(), needle.c_str());
            return
                expression_integer_compile::create
                (
                    rhs.get_location(),
                    (cp ? (cp - haystack.c_str() + 1) : 0),
                    &code
                );
        }
    }

    //
    // Build the parameters of a sys:spos procedure call.
    //
    // See system/system.c.text near line 547, from the II.0 sources.
    //
    expression_list args;
    expression::pointer func =
        code.name_expression(rhs.get_location(), "sys:spos");
    args.push_back(func);
    // var needle: string[80]
    args.push_back(rhs2[1]);
    // var haystack: string[255]
    args.push_back(rhs2[2]);

    //
    // Now build the expression node.
    //
    return expression_function_call_user_defined::create(args, &code);
}


expression::pointer
symbol_function_pos::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
