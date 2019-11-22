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
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/expression/negate/real.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_negate_real::~expression_negate_real()
{
}


expression_negate_real::expression_negate_real(
    const expression::pointer &arg,
    translator_compile *cntxt
) :
    expression_negate(arg),
    code(*cntxt)
{
}


expression_negate_real::pointer
expression_negate_real::create(const expression::pointer &arg,
    translator_compile *cntxt)
{
    return pointer(new expression_negate_real(arg, cntxt));
}


void
expression_negate_real::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(NGR);
}


expression::pointer
expression_negate_real::optimize(void)
    const
{
    //
    // evaluate the operand
    //
    expression::pointer ap = get_operand();
    expression::pointer ep = ap->optimize();

    if (ep->is_constant())
    {
        //
        // If the evaluated result is a real constant, build a new
        // expression tree with the negated constant value.
        //
        double n = ep->get_real_value();
        return expression_real_compile::create(get_location(), -n, &code);
    }

    //
    // Otherwise, build a new expression tree, negating the evaluated
    // operand.  It had deeper nodes that could be folded, even if this
    // node could not.
    //
    return expression_negate_real::create(ep, &code);
}


expression::pointer
expression_negate_real::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
