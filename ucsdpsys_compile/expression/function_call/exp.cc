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

#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type/real.h>

#include <ucsdpsys_compile/expression/function_call/exp.h>
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_function_call_exp::~expression_function_call_exp()
{
}


expression_function_call_exp::expression_function_call_exp(
    const expression_function_call_exp &arg
) :
    expression_function_call(arg),
    code(arg.code)
{
}


expression_function_call_exp::expression_function_call_exp(
    const expression_list &args,
    translator_compile *cntxt
) :
    expression_function_call(args),
    code(*cntxt)
{
}


expression_function_call_exp::pointer
expression_function_call_exp::create(const expression_list &args,
    translator_compile *cntxt)
{
    return pointer(new expression_function_call_exp(args, cntxt));
}


void
expression_function_call_exp::post_order_traversal(int, int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(CSP);
    code.emit_byte(CSP_EXP);
}


bool
expression_function_call_exp::is_constant(void)
    const
{
    assert(get_child_count() == 1);
    return get_child(0)->is_constant();
}


expression::pointer
expression_function_call_exp::optimize(void)
    const
{
    assert(get_child_count() == 1);
    expression::pointer ep = get_child(0)->optimize();
    if (ep->is_constant())
    {
        double x = ep->get_real_value();
        x = expf(x);
        if (x == HUGE_VALF || x == -HUGE_VALF)
        {
            pascal_lex_error
            (
                ep->get_location(),
                "exp(%g) result overflow",
                x
            );
            return expression_error::create(ep->get_location());
        }
        return expression_real_compile::create(ep->get_location(), x, &code);
    }

    expression_list rhs2;
    rhs2.push_back(ep);
    return pointer(new expression_function_call_exp(rhs2, &code));
}


expression::pointer
expression_function_call_exp::clone(const location &locn)
    const
{
    return create(get_children().clone(locn), &code);
}


type::pointer
expression_function_call_exp::get_type()
    const
{
    return type_real::create();
}


// vim: set ts=8 sw=4 et :
