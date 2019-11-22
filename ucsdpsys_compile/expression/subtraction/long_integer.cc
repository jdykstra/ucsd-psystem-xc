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
#include <lib/type/long_integer/sized.h>

#include <ucsdpsys_compile/expression/lint2lint/compile.h>
#include <ucsdpsys_compile/expression/long_integer/compile.h>
#include <ucsdpsys_compile/expression/subtraction/long_integer.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_subtraction_long_integer::~expression_subtraction_long_integer()
{
}


expression_subtraction_long_integer::expression_subtraction_long_integer(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *a_code
) :
    expression_subtraction(a_lhs, a_rhs),
    code(*a_code)
{
    code.need_segment(DECOPS_SEG_NUM);
}


expression_subtraction_long_integer::pointer
expression_subtraction_long_integer::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *a_code)
{
    type::pointer tp = type_long_integer_sized::create();
    expression::pointer e1 =
        expression_lint2lint_compile::create(tp, a_lhs, a_code);
    expression::pointer e2 =
        expression_lint2lint_compile::create(tp, a_rhs, a_code);
    return pointer(new expression_subtraction_long_integer(e1, e2, a_code));
}


expression::pointer
expression_subtraction_long_integer::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();

    if (code.feature_long_integer_constants())
    {
        expression_long_integer::pointer k1 =
            boost::dynamic_pointer_cast<expression_long_integer>(e1);
        if (k1)
        {
            expression_long_integer::pointer k2 =
                boost::dynamic_pointer_cast<expression_long_integer>(e2);
            if (k2)
            {
                long_integer n = k1->get_value() - k2->get_value();
                return
                    expression_long_integer_compile::create
                    (
                        get_location(),
                        get_type(),
                        n,
                        &code
                    );
            }
        }
    }

    return create(e1, e2, &code);
}


expression::pointer
expression_subtraction_long_integer::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


void
expression_subtraction_long_integer::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_ldci(DECOPS_SUBTRACT);
    code.emit_byte(CXP);
    code.emit_byte(DECOPS_SEG_NUM);
    code.emit_byte(DECOPS_PROC_NUM);
}


// vim: set ts=8 sw=4 et :
