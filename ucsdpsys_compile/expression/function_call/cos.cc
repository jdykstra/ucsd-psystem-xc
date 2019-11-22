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

#include <lib/ac/math.h>

#include <lib/debug.h>
#include <lib/pcode.h>
#include <lib/type/real.h>

#include <ucsdpsys_compile/expression/function_call/cos.h>
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_cos::~expression_function_call_cos()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_function_call_cos::expression_function_call_cos(
    const expression_function_call_cos &arg
) :
    expression_function_call(arg),
    code(arg.code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_function_call_cos::expression_function_call_cos(
    const expression_list &args,
    translator_compile *cntxt
) :
    expression_function_call(args),
    code(*cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_function_call_cos::pointer
expression_function_call_cos::create(const expression_list &args,
    translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_function_call_cos(args, cntxt));
}


void
expression_function_call_cos::post_order_traversal(int, int)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    code.breakpoint(get_location());
    code.emit_byte(CSP);
    code.emit_byte(CSP_COS);
}


bool
expression_function_call_cos::is_constant(void)
    const
{
    assert(get_child_count() == 1);
    return get_child(0)->is_constant();
}


expression::pointer
expression_function_call_cos::clone(const location &locn)
    const
{
    return create(get_children().clone(locn), &code);
}


expression::pointer
expression_function_call_cos::optimize()
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(get_child_count() == 1);
    expression::pointer ep = get_child(0)->optimize();
    if (ep->is_constant())
    {
        double x = ep->get_real_value();
        x = cos(x);
        return expression_real_compile::create(ep->get_location(), x, &code);
    }

    expression_list rhs2;
    rhs2.push_back(ep);
    return expression_function_call_cos::create(rhs2, &code);
}


type::pointer
expression_function_call_cos::get_type()
    const
{
    return type_real::create();
}


// vim: set ts=8 sw=4 et :
