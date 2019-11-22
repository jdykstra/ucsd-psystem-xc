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

#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type/integer.h>

#include <ucsdpsys_compile/expression/trunc/real.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_trunc_real::~expression_trunc_real()
{
}


expression_trunc_real::expression_trunc_real(
    const expression::pointer &a_operand,
    translator_compile *a_code
) :
    expression_trunc(a_operand),
    code(*a_code)
{
}


expression_trunc_real::pointer
expression_trunc_real::create(const expression::pointer &a_operand,
    translator_compile *a_code)
{
    return pointer(new expression_trunc_real(a_operand, a_code));
}


void
expression_trunc_real::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(CSP);
    code.emit_byte(CSP_TRUNC);
}


expression::pointer
expression_trunc_real::optimize(void)
    const
{
    expression::pointer ep = get_operand()->optimize();
    if (ep->is_constant())
    {
        double x = ep->get_real_value();
        x = trunc(x);
        if (x < -32678 || x >= 32768)
        {
            pascal_lex_error
            (
                ep->get_location(),
                "trunc produced a constant integer value (%g) "
                    "that is outside the range (-32768..32767)",
                x
            );
        }
        long i = x;
        return expression_integer_compile::create(ep->get_location(), i, &code);
    }

    return create(ep, &code);
}


expression::pointer
expression_trunc_real::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
