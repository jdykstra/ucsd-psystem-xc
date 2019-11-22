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

#include <ucsdpsys_compile/expression/bitwise_not/compile.h>
#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/expression/logical_not/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_logical_not_compile::~expression_logical_not_compile()
{
}


expression_logical_not_compile::expression_logical_not_compile(
    const expression::pointer &a_operand,
    translator_compile *cntxt
) :
    expression_logical_not(a_operand, cntxt),
    code(*cntxt)
{
}


expression_logical_not_compile::pointer
expression_logical_not_compile::create(const expression::pointer &a_operand,
    translator_compile *cntxt)
{
    return pointer(new expression_logical_not_compile(a_operand, cntxt));
}


void
expression_logical_not_compile::traversal(int)
    const
{
    code.value_traversal_for_logical_expression(this);
}


void
expression_logical_not_compile::logical_traversal(int pprec,
    label::pointer &true_branch, label::pointer &false_branch,
    bool fall_through_preference) const
{
    get_operand()->logical_traversal
    (
        pprec,
        false_branch,
        true_branch,
        !fall_through_preference
    );
}


expression::pointer
expression_logical_not_compile::optimize()
    const
{
    expression::pointer cp1 = get_operand();
    expression::pointer ep1 = cp1->optimize();
    if (ep1->is_constant())
    {
        bool value = ep1->get_boolean_value();
        value = !value;
        expression::pointer ep2 =
            expression_boolean_compile::create(get_location(), value, &code);
        return ep2;
    }
    return create(ep1, &code);
}


expression::pointer
expression_logical_not_compile::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


expression::pointer
expression_logical_not_compile::ord_odd_hack(void)
    const
{
    expression::pointer o1 = get_operand()->ord_odd_hack();
    if (!o1)
        return o1;
    return expression_bitwise_not_compile::create(o1, &code);
}


// vim: set ts=8 sw=4 et :
