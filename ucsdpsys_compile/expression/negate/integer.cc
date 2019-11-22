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
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/negate/integer.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_negate_integer::~expression_negate_integer()
{
}


expression_negate_integer::expression_negate_integer(
    const expression::pointer &arg,
    translator_compile *cntxt
) :
    expression_negate(arg),
    code(*cntxt)
{
}


expression_negate_integer::pointer
expression_negate_integer::create(const expression::pointer &arg,
    translator_compile *cntxt)
{
    return pointer(new expression_negate_integer(arg, cntxt));
}


void
expression_negate_integer::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(NGI);
}


expression::pointer
expression_negate_integer::optimize(void)
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
        // If the evaluated result is an integer constant, build a new
        // expression tree with the negated constant value.
        //
        long n = ep->get_integer_value();
        n = -n;
        return expression_integer_compile::create(get_location(), n, &code);
    }

    //
    // Otherwise, build a new expression tree, negating the evaluaged
    // operand.  It had deeper nodes that could be folded, even if this
    // node could not.
    //
    return expression_negate_integer::create(ep, &code);
}


expression::pointer
expression_negate_integer::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
