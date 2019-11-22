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

#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type.h>

#include <ucsdpsys_compile/expression/integer_division/compile.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_integer_division_compile::~expression_integer_division_compile()
{
}


expression_integer_division_compile::expression_integer_division_compile(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_integer_division(a_lhs, a_rhs),
    code(*cntxt)
{
}


expression_integer_division_compile::pointer
expression_integer_division_compile::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntx)
{
    return pointer(new expression_integer_division_compile(a_lhs, a_rhs, cntx));
}


void
expression_integer_division_compile::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(DVI);
}


expression::pointer
expression_integer_division_compile::optimize(void)
    const
{
    expression::pointer c1 = get_lhs();
    expression::pointer c2 = get_rhs();
    expression::pointer e1 = c1->optimize();
    expression::pointer e2 = c2->optimize();

    assert(e1->get_type() == e2->get_type());
    if (e1->is_constant() && e2->is_constant())
    {
        long n1 = e1->get_integer_value();
        long n2 = e2->get_integer_value();
        if (n2 == 0)
        {
            pascal_lex_error(e2->get_location(), "divide by zero");
            return expression_error::create(get_location());
        }
        long n = n1 / n2;
        return expression_integer_compile::create(get_location(), n, &code);
    }

    return expression_integer_division_compile::create(e1, e2, &code);
}


expression::pointer
expression_integer_division_compile::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
