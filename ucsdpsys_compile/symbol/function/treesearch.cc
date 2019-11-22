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
#include <lib/expression/list.h>
#include <lib/type/anything.h>
#include <lib/type/char.h>
#include <lib/type/integer.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/reference.h>
#include <lib/type/subrange.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/treesearch.h>
#include <ucsdpsys_compile/symbol/function/treesearch.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_treesearch::~symbol_function_treesearch()
{
}


symbol_function_treesearch::symbol_function_treesearch(
    translator_compile *cntxt
) :
    symbol_function("treesearch", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // function treesearch(fcp: ^anything; var fcp2: ^anything;
    //     var name: packed array [0..7] of char): integer;
    //
    name_type_list args;
    {
        variable_name_list names;
        names.push_back(variable_name("fcp", location::builtin()));
        type::pointer tp =
            type_pointer_unpacked::create(type_anything::create());
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        names.push_back(variable_name("fcp2", location::builtin()));
        type::pointer tp =
            type_reference::create
            (
                type_pointer_unpacked::create(type_anything::create())
            );
        args.push_back(name_type(names, tp));
    }
    {
        variable_name_list names;
        names.push_back(variable_name("name", location::builtin()));
        type::pointer tp =
            type_reference::create
            (
                type_array::create
                (
                    true,
                    type_subrange::create
                    (
                        0,
                        7,
                        type_integer::create()
                    ),
                    type_char::create()
                )
            );
        args.push_back(name_type(names, tp));
    }
    attach_function_parameters(args);
    attach_return_type(type_integer::create());
}


symbol_function_treesearch::pointer
symbol_function_treesearch::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_treesearch(cntxt));
}


expression::pointer
symbol_function_treesearch::function_call_expression_factory(
    const expression_list &rhs) const
{
    //
    // Checking for number and types of parameters.
    // (this does automatic promotions as well)
    //
    expression_list args = check_parameters(rhs, code);
    if (args.empty())
        return expression_error::create(rhs.get_location());
    assert(args.size() == 4);

    // FIXME: make sure fcp and fcp2 are the same type.
    // FIXME: make sure they are record types
    // FIXME: make sure the records are the correct shape

    //
    // Now build the expression node.
    //
    return expression_function_call_treesearch::create(args.tail(), &code);
}


expression::pointer
symbol_function_treesearch::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
