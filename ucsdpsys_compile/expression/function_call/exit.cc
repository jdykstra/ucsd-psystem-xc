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

#include <lib/pcode.h>
#include <lib/type/nothing.h>

#include <ucsdpsys_compile/expression/function_call/exit.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_exit::~expression_function_call_exit()
{
}


expression_function_call_exit::expression_function_call_exit(
    const expression_list &args,
    translator_compile *a_code
) :
    expression_function_call(args),
    code(*a_code)
{
}


expression_function_call_exit::pointer
expression_function_call_exit::create(const expression_list &args,
    translator_compile *a_code)
{
    return pointer(new expression_function_call_exit(args, a_code));
}


void
expression_function_call_exit::post_order_traversal(int, int)
    const
{
    assert(get_child_count() == 2);
    code.breakpoint(get_location());
    code.emit_byte(CSP);
    code.emit_byte(CSP_EXIT);
}


expression::pointer
expression_function_call_exit::clone(const location &locn)
    const
{
    return create(get_children().clone(locn), &code);
}


expression::pointer
expression_function_call_exit::optimize()
    const
{
    assert(get_child_count() == 2);
    expression_list args;
    args.push_back(get_child(0));
    args.push_back(get_child(1));
    return expression_function_call_exit::create(args, &code);
}


bool
expression_function_call_exit::execution_flows_out(void)
    const
{
    return false;
}


type::pointer
expression_function_call_exit::get_type(void)
    const
{
    return type_nothing::create();
}


// vim: set ts=8 sw=4 et :
