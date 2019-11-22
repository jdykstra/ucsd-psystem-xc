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

#include <lib/pcode.h>
#include <ucsdpsys_compile/expression/addition/real.h>
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_addition_real::~expression_addition_real()
{
}


expression_addition_real::expression_addition_real(
        const expression::pointer &a_lhs,
        const expression::pointer &a_rhs, translator_compile *cntxt) :
    expression_addition(a_lhs, a_rhs),
    code(*cntxt)
{
}


expression_addition_real::pointer
expression_addition_real::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_addition_real(a_lhs, a_rhs, cntxt));
}


void
expression_addition_real::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(ADR);
}


expression::pointer
expression_addition_real::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


expression::pointer
expression_addition_real::optimize()
    const
{
    expression::pointer c1 = get_lhs();
    expression::pointer c2 = get_rhs();
    expression::pointer e1 = c1->optimize();
    expression::pointer e2 = c2->optimize();

    assert(e1->get_type() == e2->get_type());
    if (e1->is_constant() && e2->is_constant())
    {
        expression::pointer result =
            expression_real_compile::create
            (
                get_location(),
                e1->get_real_value() + e2->get_real_value(),
                &code
            );
        return result;
    }
    if (e1->is_constant() && e1->get_real_value() == 0)
        return e2;
    if (e2->is_constant() && e2->get_real_value() == 0)
        return e1;

    return expression_addition_real::create(e1, e2, &code);
}


void
expression_addition_real::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"shouln't happen, not boolean");
    traversal(pprec);
}


// vim: set ts=8 sw=4 et :
