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

#include <lib/ac/assert.h>

#include <lib/debug.h>
#include <lib/pcode.h>
#include <lib/type.h>
#include <ucsdpsys_compile/expression/negate/real.h>
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/expression/subtraction/real.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_subtraction_real::~expression_subtraction_real()
{
}


expression_subtraction_real::expression_subtraction_real(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_subtraction(a_lhs, a_rhs),
    code(*cntxt)
{
}


expression_subtraction_real::pointer
expression_subtraction_real::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_subtraction_real(a_lhs, a_rhs, cntxt));
}


void
expression_subtraction_real::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(SBR);
}


expression::pointer
expression_subtraction_real::optimize(void)
    const
{
    expression::pointer c1 = get_lhs();
    DEBUG(2, "c1 = %s", c1->lisp_representation().c_str());
    expression::pointer c2 = get_rhs();
    DEBUG(2, "c2 = %s", c2->lisp_representation().c_str());
    expression::pointer e1 = c1->optimize();
    DEBUG(2, "e1 = %s", e1->lisp_representation().c_str());
    expression::pointer e2 = c2->optimize();
    DEBUG(2, "e2 = %s", e2->lisp_representation().c_str());

    if (e1->is_constant() && e2->is_constant())
    {
        expression::pointer result =
            expression_real_compile::create
            (
                get_location(),
                e1->get_real_value() - e2->get_real_value(),
                &code
            );
        DEBUG(2, "result = %s", result->lisp_representation().c_str());
        return result;
    }
    if (e1->is_constant() && e1->get_real_value() == 0)
        return expression_negate_real::create(e2, &code);
    if (e2->is_constant() && e2->get_real_value() == 0)
        return e1;

    return expression_subtraction_real::create(e1, e2, &code);
}


expression::pointer
expression_subtraction_real::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
