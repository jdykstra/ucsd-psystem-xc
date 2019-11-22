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
#include <lib/pcode.h>
#include <lib/type/integer.h>

#include <ucsdpsys_compile/expression/function_call/length.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_length::~expression_function_call_length()
{
}


expression_function_call_length::expression_function_call_length(
    const expression_list &args,
    translator_compile *cntxt
) :
    expression_function_call(args),
    code(*cntxt)
{
    assert(args.size() == 1);
}


expression_function_call_length::pointer
expression_function_call_length::create(const expression_list &args,
    translator_compile *cntxt)
{
    return pointer(new expression_function_call_length(args, cntxt));
}


void
expression_function_call_length::post_order_traversal(int, int)
    const
{
    code.breakpoint(get_location());
    code.emit_ldci(0);
    code.emit_byte(LDB);
}


bool
expression_function_call_length::is_constant(void)
    const
{
    assert(get_child_count() == 1);
    return get_child(0)->is_constant();
}


expression::pointer
expression_function_call_length::optimize()
    const
{
    assert(get_child_count() == 1);
    expression::pointer ep = get_child(0);
    assert(ep);
    assert(!ep->get_location().empty());
    DEBUG(1, "ep = %s", ep->lisp_representation().c_str());
    ep = ep->optimize();
    if (ep->is_constant())
    {
        long n = ep->get_string_value().size();
        return expression_integer_compile::create(ep->get_location(), n, &code);
    }
    DEBUG(1, "ep = %s", ep->lisp_representation().c_str());
    assert(ep);
    assert(!ep->get_location().empty());
    expression_list args2;
    args2.push_back(ep);
    return pointer(new expression_function_call_length(args2, &code));
}


expression::pointer
expression_function_call_length::clone(const location &locn)
    const
{
    return create(get_children().clone(locn), &code);
}


type::pointer
expression_function_call_length::get_type()
    const
{
    return type_integer::create();
}


// vim: set ts=8 sw=4 et :
