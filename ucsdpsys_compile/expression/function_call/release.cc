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

#include <ucsdpsys_compile/expression/function_call/release.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_release::~expression_function_call_release()
{
}


expression_function_call_release::expression_function_call_release(
    const expression_list &args,
    translator_compile *cntxt
) :
    expression_function_call(args),
    code(*cntxt)
{
}


expression_function_call_release::pointer
expression_function_call_release::create(const expression_list &args,
    translator_compile *cntxt)
{
    return pointer(new expression_function_call_release(args, cntxt));
}


expression::pointer
expression_function_call_release::optimize()
    const
{
    return create(get_children().optimize(), &code);
}


expression::pointer
expression_function_call_release::clone(const location &locn)
    const
{
    return create(get_children().clone(locn), &code);
}


type::pointer
expression_function_call_release::get_type()
    const
{
    return type_nothing::create();
}


void
expression_function_call_release::post_order_traversal(int, int)
    const
{
    assert(get_child_count() == 1);
    code.breakpoint(get_location());
    code.emit_byte(CSP);
    code.emit_byte(CSP_RELEASE);
}


void
expression_function_call_release::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"the grammar should have prevented this");
}


// vim: set ts=8 sw=4 et :
