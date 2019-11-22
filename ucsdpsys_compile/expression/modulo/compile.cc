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

#include <lib/ac/assert.h>

#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>

#include <ucsdpsys_compile/expression/modulo/compile.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_modulo_compile::~expression_modulo_compile()
{
}


expression_modulo_compile::expression_modulo_compile(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_addition(a_lhs, a_rhs),
    code(*cntxt)
{
}


expression_modulo_compile::pointer
expression_modulo_compile::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_modulo_compile(a_lhs, a_rhs, cntxt));
}


void
expression_modulo_compile::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(MODI);
}


expression::pointer
expression_modulo_compile::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    assert(e1->get_type()->get_size_in_words() == 1);
    expression::pointer e2 = get_rhs()->optimize();
    assert(e2->get_type()->get_size_in_words() == 1);

    if (e2->is_constant())
    {
        int n2 = e2->get_integer_value();
        if (n2 == 0)
        {
            pascal_lex_error(e2->get_location(), "modulo by zero");
            return expression_error::create(e2->get_location());
        }
        if (e1->is_constant())
        {
            int n1 = e1->get_integer_value();
            int n = n1 % n2;
            return expression_integer_compile::create(get_location(), n, &code);
        }
    }
    else if (e1->is_constant() && e1->get_integer_value() == 0)
    {
        return e1;
    }

    return expression_modulo_compile::create(e1, e2, &code);
}


expression::pointer
expression_modulo_compile::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


void
expression_modulo_compile::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"shouldn't happen, not boolean");
    traversal(pprec);
}


// vim: set ts=8 sw=4 et :
