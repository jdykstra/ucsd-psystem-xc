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

#include <lib/ac/math.h>

#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type/integer.h>
#include <lib/type/long_integer/sized.h>

#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/lint2lint/compile.h>
#include <ucsdpsys_compile/expression/long_integer/compile.h>
#include <ucsdpsys_compile/expression/trunc/long_integer.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_trunc_long_integer::~expression_trunc_long_integer()
{
}


expression_trunc_long_integer::expression_trunc_long_integer(
    const expression::pointer &a_operand,
    translator_compile *a_code
) :
    expression_trunc(a_operand),
    code(*a_code)
{
    code.need_segment(DECOPS_SEG_NUM);
}


expression_trunc_long_integer::pointer
expression_trunc_long_integer::create(const expression::pointer &ep,
    translator_compile *a_code)
{
    type::pointer tp = type_long_integer_sized::create();
    expression::pointer a_operand =
        expression_lint2lint_compile::create(tp, ep, a_code);
    return pointer(new expression_trunc_long_integer(a_operand, a_code));
}


void
expression_trunc_long_integer::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(DECOPS_TRUNC);
    code.emit_byte(CXP);
    code.emit_byte(DECOPS_SEG_NUM);
    code.emit_byte(DECOPS_PROC_NUM);
}


expression::pointer
expression_trunc_long_integer::optimize(void)
    const
{
    expression::pointer ep = get_operand()->optimize();

    if (code.feature_long_integer_constants())
    {
        expression_long_integer::pointer k1 =
            boost::dynamic_pointer_cast<expression_long_integer>(ep);
        if (k1)
        {
            long_integer x = k1->get_value();
            if (x < -32678 || x >= 32768)
            {
                pascal_lex_error
                (
                    ep->get_location(),
                    "trunc produced a constant long integer value (%s) "
                        "that is outside the range (-32768..32767)",
                    x.to_string().c_str()
                );
                return expression_error::create(get_location());
            }
            return
                expression_integer_compile::create
                (
                    ep->get_location(),
                    x.trunc(),
                    &code
                );
        }
    }

    return create(ep, &code);
}


expression::pointer
expression_trunc_long_integer::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
