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
#include <ucsdpsys_compile/expression/addition/integer.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_addition_integer::~expression_addition_integer()
{
}


expression_addition_integer::expression_addition_integer(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_addition(a_lhs, a_rhs),
    code(*cntxt)
{
}


expression_addition_integer::pointer
expression_addition_integer::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_addition_integer(a_lhs, a_rhs, cntxt));
}


void
expression_addition_integer::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(ADI);
}


expression::pointer
expression_addition_integer::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


expression::pointer
expression_addition_integer::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();

    assert(e1->get_type()->get_size_in_words() == 1);
    assert(e2->get_type()->get_size_in_words() == 1);
    if (e1->is_constant() && e2->is_constant())
    {
        short v1 = e1->get_integer_value();
        short v2 = e2->get_integer_value();
        short value = v1 + v2;
        expression::pointer result =
            expression_integer_compile::create(get_location(), value, &code);
        return result;
    }
    if (e1->is_constant() && e1->get_integer_value() == 0)
        return e2;
    if (e2->is_constant() && e2->get_integer_value() == 0)
        return e1;

    return expression_addition_integer::create(e1, e2, &code);
}


void
expression_addition_integer::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"shouldn't happen, not boolean");
    traversal(pprec);
}


// vim: set ts=8 sw=4 et :
