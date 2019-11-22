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

#include <lib/pcode.h>

#include <ucsdpsys_compile/expression/bitwise_and/compile.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_bitwise_and_compile::~expression_bitwise_and_compile()
{
}


expression_bitwise_and_compile::expression_bitwise_and_compile(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *a_code
) :
    expression_bitwise_and(a_lhs, a_rhs),
    code(*a_code)
{
}


expression_bitwise_and_compile::pointer
expression_bitwise_and_compile::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *a_code)
{
    return pointer(new expression_bitwise_and_compile(a_lhs, a_rhs, a_code));
}


void
expression_bitwise_and_compile::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(LAND);
}


expression::pointer
expression_bitwise_and_compile::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


expression::pointer
expression_bitwise_and_compile::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();
    if (e1->is_constant() && e2->is_constant())
    {
        int value = e1->get_integer_value() & e2->get_integer_value();
        return expression_integer_compile::create(get_location(), value, &code);
    }
    if (e1->is_constant() && e1->get_integer_value() == 0)
    {
        return expression_integer_compile::create(get_location(), 0, &code);
    }
    if (e2->is_constant() && e2->get_integer_value() == 0)
    {
        return expression_integer_compile::create(get_location(), 0, &code);
    }
    return create(e1, e2, &code);
}


// vim: set ts=8 sw=4 et :
