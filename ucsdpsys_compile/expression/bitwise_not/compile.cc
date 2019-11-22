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

#include <ucsdpsys_compile/expression/bitwise_not/compile.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_bitwise_not_compile::~expression_bitwise_not_compile()
{
}


expression_bitwise_not_compile::expression_bitwise_not_compile(
    const expression::pointer &a_operand,
    translator_compile *a_code
) :
    expression_bitwise_not(a_operand),
    code(*a_code)
{
}


expression_bitwise_not_compile::pointer
expression_bitwise_not_compile::create(const expression::pointer &a_operand,
    translator_compile *a_code)
{
    return pointer(new expression_bitwise_not_compile(a_operand, a_code));
}


void
expression_bitwise_not_compile::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(LNOT);
}


expression::pointer
expression_bitwise_not_compile::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


expression::pointer
expression_bitwise_not_compile::optimize(void)
    const
{
    expression::pointer ep = get_operand()->optimize();
    if (ep->is_constant())
    {
        int value = ep->get_integer_value();
        value = 0xFFFF & ~value;
        return expression_integer_compile::create(get_location(), value, &code);
    }
    return create(ep, &code);
}


// vim: set ts=8 sw=4 et :
