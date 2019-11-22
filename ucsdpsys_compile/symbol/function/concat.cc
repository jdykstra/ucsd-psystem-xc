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
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/comma.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/string/compile.h>
#include <ucsdpsys_compile/symbol/function/concat.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_concat::~symbol_function_concat()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_concat::symbol_function_concat(
    translator_compile *a_code
) :
    symbol_function("concat", location::builtin(), scope::pointer(), -1),
    code(*a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_concat::pointer
symbol_function_concat::create(translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_concat(a_code));
}


expression::pointer
symbol_function_concat::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (rhs.size() < 3)
    {
        parameter_error_wrong_number(rhs, 2);
        return expression_error::create(rhs.get_location());
    }

    for (size_t j = 1; j < rhs.size(); ++j)
    {
        expression::pointer ep = rhs[j];
        if (ep->is_error())
            return ep;
        ep = code.call_noparam_func_if_necessary(ep);
        if (!type_string::is_a(ep))
        {
            parameter_error_wrong_type(j, ep, type_string::create());
            return expression_error::create(ep->get_location());
        }
    }

    //
    // Check for all-constant case.
    //
    size_t totlen = 0;
    {
        bool all_constant = true;
        for (size_t j = 1; j < rhs.size(); ++j)
        {
            expression::pointer ep = rhs[j];
            if (!ep->is_constant())
                all_constant = false;
            type_string::pointer tsp =
                boost::dynamic_pointer_cast<type_string>(ep->get_type());
            assert(tsp);
            totlen += tsp->get_size();
        }
        if (all_constant)
        {
            rcstring result;
            for (size_t j = 1; j < rhs.size(); ++j)
            {
                expression::pointer ep = rhs[j];
                result += ep->optimize()->get_string_value();
            }
            return
                expression_string_compile::create
                (
                    rhs.get_location(),
                    result,
                    &code
                );
        }
    }
    assert(totlen > 0);

    //
    // Ask the translator for a temporary variable,
    // the size of the largest permitted string.
    //
    int destleng = totlen < 256 ? totlen : 255;
    symbol::pointer tvar =
        code.allocate_temporary_variable(type_string::create(destleng));

    //
    // The first parameter is turned into a simple string assignment.
    //
    expression_list collection;
    {
        expression::pointer ep1 =
            tvar->name_expression_factory
            (
                tvar,
                rhs[1]->get_location(),
                code.get_lex_level()
            );
        expression::pointer ep2 =
            code.assignment_expression_factory(ep1, rhs[1]);
        collection.push_back(ep2);
    }

    //
    // The rest of the parameters are turned into sys:sconcat function calls.
    //
    // See system/system.c.text near line 492, from the II.0 sources.
    //
    for (size_t j = 2; j < rhs.size(); ++j)
    {
        expression::pointer ep = rhs[j];
        expression::pointer func =
            code.name_expression(ep->get_location(), "sys:sconcat");
        expression_list args;
        args.push_back(func);
        // var dest: string
        expression::pointer ep1 =
            tvar->name_expression_factory
            (
                tvar,
                ep->get_location(),
                code.get_lex_level()
            );
        args.push_back(ep1);
        // var src: string
        args.push_back(ep);
        // destleng: integer
        expression::pointer ep2 =
            expression_integer_compile::create
            (
                ep->get_location(),
                destleng,
                &code
            );
        args.push_back(ep2);
        ep = func->function_call_expression_factory(args);
        collection.push_back(ep);
    }

    //
    // Last of all comes the string itself.
    //
    {
        expression::pointer ep =
            tvar->name_expression_factory
            (
                tvar,
                rhs.back()->get_location(),
                code.get_lex_level()
            );
        collection.push_back(ep);
    }

    //
    // Now build the expression node.
    //
    return expression_comma::create(collection, &code);
}


expression::pointer
symbol_function_concat::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
