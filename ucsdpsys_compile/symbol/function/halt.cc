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

#include <ucsdpsys_compile/symbol/function/halt.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/halt.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_halt::~symbol_function_halt()
{
}


symbol_function_halt::symbol_function_halt(translator_compile *cntxt) :
    symbol_function("halt", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
}


symbol_function_halt::pointer
symbol_function_halt::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_halt(cntxt));
}


expression::pointer
symbol_function_halt::function_call_expression_factory(
    const expression_list &rhs) const
{
    //
    // Checking for number and types of parameters.
    //
    expression_list args = check_parameters(rhs, code);
    if (args.empty())
        return expression_error::create(rhs.get_location());
    assert(args.size() == 1);

    //
    // Now build the expression node.
    //
    return expression_function_call_halt::create(rhs.get_location(), &code);
}


expression::pointer
symbol_function_halt::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
