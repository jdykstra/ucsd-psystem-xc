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

#include <ucsdpsys_compile/expression/lint_from_integer/compile.h>
#include <ucsdpsys_compile/expression/long_integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_lint_from_integer_compile::~expression_lint_from_integer_compile()
{
}


expression_lint_from_integer_compile::expression_lint_from_integer_compile(
    const expression::pointer &a_operand,
    translator_compile *a_code
) :
    expression_lint_from_integer(a_operand),
    code(*a_code)
{
    code.need_segment(DECOPS_SEG_NUM);
}


expression_lint_from_integer_compile::pointer
expression_lint_from_integer_compile::create(
    const expression::pointer &a_operand, translator_compile *a_code)
{
    return pointer(new expression_lint_from_integer_compile(a_operand, a_code));
}


void
expression_lint_from_integer_compile::post_order_traversal(void)
    const
{
    code.breakpoint(get_location());
    code.emit_ldci(DECOPS_CONVERT_TOS);
    code.emit_byte(CXP);
    code.emit_byte(DECOPS_SEG_NUM);
    code.emit_byte(DECOPS_PROC_NUM);
}


expression::pointer
expression_lint_from_integer_compile::optimize(void)
    const
{
    expression::pointer e1 = get_operand()->optimize();
    if (code.feature_long_integer_constants() && e1->is_constant())
    {
        return
            expression_long_integer_compile::create
            (
                get_location(),
                get_type(),
                e1->get_integer_value(),
                &code
            );
    }
    return create(e1, &code);
}


expression::pointer
expression_lint_from_integer_compile::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
