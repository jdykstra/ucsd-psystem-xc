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
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/file.h>
#include <lib/type/reference.h>

#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/symbol/function/eoln.h>
#include <ucsdpsys_compile/translator/compile.h>


symbol_function_eoln::~symbol_function_eoln()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol_function_eoln::symbol_function_eoln(translator_compile *cntxt) :
    symbol_function("eoln", location::builtin(), scope::pointer(), -1),
    code(*cntxt)
{
    //
    // function eoln(var f: file of char): boolean;
    //
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    name_type_list args;
    {
        variable_name_list names;
        names.push_back(variable_name("f", location::builtin()));
        type::pointer tp =
            type_reference::create(type_file::create(type_char::create()));
        args.push_back(name_type(names, tp));
    }
    attach_function_parameters(args);
    attach_return_type(type_boolean::create());
}


symbol_function_eoln::pointer
symbol_function_eoln::create(translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new symbol_function_eoln(cntxt));
}


expression::pointer
symbol_function_eoln::function_call_expression_factory(
    const expression_list &rhs) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression_list rhs1(rhs);
    if (rhs1.size() < 2)
        rhs1.push_back(code.name_expression(rhs1.get_location(), "input"));

    expression_list rhs2 = check_parameters(rhs1, code);
    if (rhs2.empty())
        return expression_error::create(rhs1.get_location());
    assert(rhs2.size() == 2);

    expression_list args;
    expression::pointer func =
        code.name_expression(rhs.get_location(), "sys:feoln");
    args.push_back(func);
    args.push_back(rhs2[1]);
    return expression_function_call_user_defined::create(args, &code);
}


expression::pointer
symbol_function_eoln::name_expression_factory(const symbol::pointer &sym,
    const location &where, int)
{
    assert(this == sym.get());
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(sym);
    assert(sfp);
    return expression_function::create(where, sfp, &code);
}


// vim: set ts=8 sw=4 et :
