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
#include <lib/type/array.h>
#include <lib/type/anything.h>
#include <lib/type/char.h>
#include <lib/type/integer.h>
#include <lib/type/reference.h>
#include <lib/type/subrange.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/idsearch.h>
#include <ucsdpsys_compile/symbol/function/idsearch.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_idsearch::~symbol_function_idsearch()
{
}


symbol_function_idsearch::symbol_function_idsearch(
    translator_compile *cntxt
) :
    symbol_function("idsearch", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // procedure idsearch(var p1: anything; var p2: packed array of char);
    //
    // The first parameter is fugly.  It is actually a pointer to
    //     record
    //       symcursor: integer;
    //       sym: integer;
    //       op: integer;
    //       name: packed array [0..7] of char
    //     end
    // except that the compiler does not declare such a beast, instead
    // it carefully declares them in the right order.  Bletch.
    // See compiler/compglbls.text around line 297 (SYMCURSOR).
    //
    name_type_list args;
    {
        variable_name_list names;
        names.push_back(variable_name("p1", location::builtin()));
        type::pointer tp = type_reference::create(type_anything::create());
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        names.push_back(variable_name("p2", location::builtin()));
        type::pointer tp =
            type_reference::create
            (
                type_array::create
                (
                    true,
                    type_subrange::create(0, 1023, type_integer::create()),
                    type_char::create()
                )
            );
        args.push_back(name_type(names, tp));
    }
    attach_function_parameters(args);
}


symbol_function_idsearch::pointer
symbol_function_idsearch::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_idsearch(cntxt));
}


expression::pointer
symbol_function_idsearch::function_call_expression_factory(
    const expression_list &rhs) const
{
    //
    // Checking for number and types of parameters.
    // (this does automatic promotions as well)
    //
    expression_list args = check_parameters(rhs, code);
    if (args.empty())
        return expression_error::create(rhs.get_location());
    assert(args.size() == 3);

    // FIXME: check that p1 is going to work.

    //
    // Now build the expression node.
    //
    return expression_function_call_idsearch::create(args.tail(), &code);
}


expression::pointer
symbol_function_idsearch::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
