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

#include <ucsdpsys_compile/expression/function_call/halt.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_halt::~expression_function_call_halt()
{
}


expression_function_call_halt::expression_function_call_halt(
    const location &locn,
    translator_compile *a_code
) :
    expression_function_call(locn),
    code(*a_code)
{
}


expression_function_call_halt::pointer
expression_function_call_halt::create(const location &locn,
    translator_compile *a_code)
{
    return pointer(new expression_function_call_halt(locn, a_code));
}


void
expression_function_call_halt::post_order_traversal(int, int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(CSP);
    code.emit_byte(CSP_HALT);
}


expression::pointer
expression_function_call_halt::optimize()
    const
{
    return create(get_location(), &code);
}


expression::pointer
expression_function_call_halt::clone(const location &locn)
    const
{
    return create(locn, &code);
}


type::pointer
expression_function_call_halt::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_function_call_halt::execution_flows_out(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
