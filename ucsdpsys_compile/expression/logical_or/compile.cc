//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/pascal/lex.h>

#include <ucsdpsys_compile/expression/bitwise_or/compile.h>
#include <ucsdpsys_compile/expression/logical_or/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_logical_or_compile::~expression_logical_or_compile()
{
}


expression_logical_or_compile::expression_logical_or_compile(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_logical_or(a_lhs, a_rhs, cntxt),
    code(*cntxt)
{
}


expression::pointer
expression_logical_or_compile::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    if (a_lhs->is_constant())
    {
        bool cond = a_lhs->optimize()->get_boolean_value();
        if (cntxt->warning_constant_branch())
        {
            pascal_lex_warning
            (
                a_lhs->get_location(),
                "left hand side of logical OR is constant, it is always %s",
                (cond ? "true" : "false")
            );
        }
        if (cond)
            return a_lhs;
        return a_rhs;
    }
    if (a_rhs->is_constant())
    {
        bool cond = a_rhs->optimize()->get_boolean_value();
        if (cntxt->warning_constant_branch())
        {
            pascal_lex_warning
            (
                a_rhs->get_location(),
                "right hand side of logical OR is constant, it is always %s",
                (cond ? "true" : "false")
            );
        }
        if (cond)
        {
            if (a_lhs->has_side_effect() != expression::side_effect_no)
            {
                pascal_lex_warning
                (
                    a_lhs->get_location(),
                    "side effects of left hand side of logical OR "
                    "expression have been lost"
                );
            }
            return a_rhs;
        }
        return a_lhs;
    }

    return pointer(new expression_logical_or_compile(a_lhs, a_rhs, cntxt));
}


expression::pointer
expression_logical_or_compile::optimize(void)
    const
{
    expression::pointer o1 = get_lhs();
    expression::pointer e1 = o1->optimize();

    expression::pointer o2 = get_rhs();
    expression::pointer e2 = o2->optimize();
    if (e1->is_constant())
    {
        if (e1->get_boolean_value())
            return e1;
        return e2;
    }
    if (e2->is_constant())
    {
        // FIXME: what about side effects of left hand side?
        if (e2->get_boolean_value())
            return e2;
        return e1;
    }
    return expression_logical_or_compile::create(e1, e2, &code);
}


expression::pointer
expression_logical_or_compile::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


void
expression_logical_or_compile::traversal(int)
    const
{
    code.value_traversal_for_logical_expression(this);
}


expression::pointer
expression_logical_or_compile::ord_odd_hack(void)
    const
{
    expression::pointer bw1 = get_lhs()->ord_odd_hack();
    if (!bw1)
        return bw1;
    expression::pointer bw2 = get_rhs()->ord_odd_hack();
    if (!bw2)
        return bw2;
    return expression_bitwise_or_compile::create(bw1, bw2, &code);
}


// vim: set ts=8 sw=4 et :
