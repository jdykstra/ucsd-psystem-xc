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

#include <ucsdpsys_compile/expression/function_call/fillchar.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_fillchar::~expression_function_call_fillchar()
{
}


expression_function_call_fillchar::expression_function_call_fillchar(
    const expression_function_call_fillchar &arg
) :
    expression_function_call(arg),
    code(arg.code)
{
}


expression_function_call_fillchar::expression_function_call_fillchar(
    const expression_list &args,
    translator_compile *cntxt
) :
    expression_function_call(args),
    code(*cntxt)
{
}


expression_function_call_fillchar::pointer
expression_function_call_fillchar::create(const expression_list &args,
    translator_compile *cntxt)
{
    return pointer(new expression_function_call_fillchar(args, cntxt));
}


void
expression_function_call_fillchar::post_order_traversal(int, int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(CSP);
    code.emit_byte(CSP_FILLCHAR);
}


expression::pointer
expression_function_call_fillchar::clone(const location &locn)
    const
{
    return create(get_children().clone(locn), &code);
}


expression::pointer
expression_function_call_fillchar::optimize()
    const
{
    expression_list args2;
    for (size_t j = 0; j < get_child_count(); ++j)
        args2.push_back(get_child(j)->optimize());
    return create(args2, &code);
}


type::pointer
expression_function_call_fillchar::get_type()
    const
{
    return type_nothing::create();
}


// vim: set ts=8 sw=4 et :
