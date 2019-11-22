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
#include <lib/pascal/lex.h>
#include <lib/type/integer.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/ioresult.h>
#include <ucsdpsys_compile/symbol/function/ioresult.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_ioresult::~symbol_function_ioresult()
{
}


symbol_function_ioresult::symbol_function_ioresult(translator_compile *cntxt) :
    symbol_function("ioresult", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // function ioresult: integer;
    //
    name_type_list args;
    attach_function_parameters(args);
    attach_return_type(type_integer::create());
}


symbol_function_ioresult::pointer
symbol_function_ioresult::create(translator_compile *cntxt)
{
    return pointer(new symbol_function_ioresult(cntxt));
}


expression::pointer
symbol_function_ioresult::function_call_expression_factory(
    const expression_list &rhs) const
{
    //
    // If the translator has been generating IOCHECK opcodes after I/O
    // calls, this call to IORESULT will always return success.  Warn
    // the user, just in case they are puzzled why.
    //
    if (code.issue_iocheck())
    {
        pascal_lex_warning
        (
            rhs.get_location(),
            "the ioresult function will ways return success while "
            "(*$I+*) is active, because errors will terminate the "
            "program before it reaches this ioresult call"
        );
    }

    //
    // Checking for number and types of parameters.
    // (this does automatic promotions as well)
    //
    expression_list args = check_parameters(rhs, code);
    if (args.empty())
        return expression_error::create(rhs.get_location());
    assert(args.size() == 1);

    //
    // Now build the expression node.
    //
    return expression_function_call_ioresult::create(rhs.get_location(), &code);
}


expression::pointer
symbol_function_ioresult::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
