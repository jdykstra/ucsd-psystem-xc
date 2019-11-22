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
#include <lib/type/integer.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/comma.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/expression/string/compile.h>
#include <ucsdpsys_compile/symbol/function/copy.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_copy::~symbol_function_copy()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_copy::symbol_function_copy(
    translator_compile *a_code
) :
    symbol_function("copy", location::builtin(), scope::pointer(), -1),
    code(*a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);

    name_type_list args;
    {
        variable_name_list names;
        names.push_back(variable_name("src", location::builtin()));
        type::pointer tp = type_string::create(255);
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        names.push_back(variable_name("index", location::builtin()));
        type::pointer tp = type_integer::create();
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        names.push_back(variable_name("count", location::builtin()));
        type::pointer tp = type_integer::create();
        args.push_back(name_type(names, tp));
    }
    attach_function_parameters(args);
}


symbol_function_copy::pointer
symbol_function_copy::create(translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_copy(a_code));
}


static int
get_size_of_string(const type::pointer &tp)
{
    type_string::pointer tsp =
        boost::dynamic_pointer_cast<type_string>(tp);
    assert(tsp);
    return tsp->get_size();
}


static int
get_size_of_string(const expression::pointer &ep)
{
    return get_size_of_string(ep->get_type());
}


expression::pointer
symbol_function_copy::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_list rhs2 = check_parameters(rhs, code);
    if (rhs2.empty())
        return expression_error::create(rhs.get_location());
    assert(rhs2.size() == 4);

    if
    (
        rhs2[1]->is_constant()
    &&
        rhs2[2]->is_constant()
    &&
        rhs2[3]->is_constant()
    )
    {
        // The p-System implementation is very dumb, and has poorly
        // defined error semantics (completely undocumented, actually).
        //
        //   PROCEDURE SCOPY(*VAR SRC,DEST: STRING; SRCINX,COPYLENG: INTEGER*);
        //   BEGIN
        //     DEST := '';
        //     IF (SRCINX > 0) AND (COPYLENG > 0) AND
        //        (SRCINX+COPYLENG-1 <= LENGTH(SRC))
        //     THEN
        //       BEGIN
        //         MOVELEFT(SRC[SRCINX], DEST[1], COPYLENG);
        //         DEST[0] := CHR(COPYLENG)
        //       END
        //   END
        //
        rcstring src = rhs2[1]->optimize()->get_string_value();
        int index = rhs2[2]->optimize()->get_integer_value();
        int count = rhs2[3]->optimize()->get_integer_value();
        rcstring result;
        if (index < 1)
        {
            pascal_lex_error
            (
                rhs[1]->get_location(),
                "the string index may not be less than one"
            );
        }
        else if (count < 0)
        {
            pascal_lex_error
            (
                rhs[2]->get_location(),
                "the count may not be negative"
            );
        }
        else if (size_t(index - 1 + count) > src.size())
        {
            pascal_lex_error
            (
                rhs[1]->get_location(),
                "the source index (%d) plus the count (%d) exceed the "
                    "source string size (%d)",
                index,
                count,
                (int)src.size()
            );
        }
        else
        {
            // Do the same bone-headed thing that SCOPY does,
            // at least until we fix it.
            result = src.substring(index - 1, count);
        }

        return
            expression_string_compile::create
            (
                rhs.get_location(),
                result,
                &code
            );
    }

    if (rhs2[2]->is_constant())
    {
        // See above comments about bone-headeded-ness.
        int srcinx = rhs2[2]->optimize()->get_integer_value();
        if (srcinx < 1)
        {
            return
                expression_string_compile::create
                (
                    rhs.get_location(),
                    "",
                    &code
                );
        }
        if (srcinx >= get_size_of_string(rhs2[1]))
        {
            pascal_lex_error
            (
                rhs2[2]->get_location(),
                "string index is always too large"
            );
            return expression_error::create(rhs[2]->get_location());
        }
    }

    if (rhs2[3]->is_constant())
    {
        // See above comments about bone-headeded-ness.
        int count = rhs2[3]->optimize()->get_integer_value();
        if (count <= 0)
        {
            return
                expression_string_compile::create
                (
                    rhs.get_location(),
                    "",
                    &code
                );
        }
        if (count > get_size_of_string(rhs2[1]))
        {
            pascal_lex_error
            (
                rhs2[3]->get_location(),
                "count is always too large"
            );
            return expression_error::create(rhs[3]->get_location());
        }
    }

    //
    // Estimate the largest possible result.
    //
    int destleng = get_size_of_string(rhs2[1]);
    if (rhs2[3]->is_constant())
    {
        int count = rhs2[3]->optimize()->get_integer_value();
        if (count >= 1 && count < destleng)
            destleng = count;
    }

    //
    // Ask the translator for a temporary variable,
    // the size of the largest possible result.
    //
    symbol::pointer tvar =
        code.allocate_temporary_variable(type_string::create(destleng));

    // The result is actually two expressions, like a C comma operator.
    // The first is of type nothing.
    expression_list collection;

    //
    // Build the parameters of a sys:scopy procedure call.
    //
    // See system/system.c.text near line 521, from the II.0 sources.
    //
    {
        expression_list params;
        expression::pointer func =
            code.name_expression(rhs.get_location(), "sys:scopy");
        params.push_back(func);
        // var src: string
        params.push_back(rhs2[1]);
        // var dest: string
        expression::pointer ep1 =
            tvar->name_expression_factory
            (
                tvar,
                rhs2[1]->get_location(),
                code.get_lex_level()
            );
        params.push_back(ep1);
        // srcinx: integer
        params.push_back(rhs2[2]);
        // copyleng: integer
        params.push_back(rhs2[3]);
        DEBUG(1, "mark");
        expression::pointer ep =
            expression_function_call_user_defined::create(params, &code);
        DEBUG(1, "mark");
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
symbol_function_copy::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
