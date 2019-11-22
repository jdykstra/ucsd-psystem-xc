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
#include <lib/type/integer.h>

#include <ucsdpsys_compile/expression/abs/integer.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_abs_integer::~expression_abs_integer()
{
}


expression_abs_integer::expression_abs_integer(
    const expression::pointer &a_operand,
    translator_compile *a_code
) :
    expression_abs(a_operand),
    code(*a_code)
{
}


expression_abs_integer::pointer
expression_abs_integer::create(const expression::pointer &a_operand,
    translator_compile *a_code)
{
    return pointer(new expression_abs_integer(a_operand, a_code));
}


void
expression_abs_integer::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(ABI);
}


expression::pointer
expression_abs_integer::optimize(void)
    const
{
    expression::pointer ep = get_operand()->optimize();
    assert(type_integer::is_a(ep));
    if (ep->is_constant())
    {
        long n = ep->get_integer_value();
        if (n < 0)
            n = -n;
        return expression_integer_compile::create(ep->get_location(), n, &code);
    }
    return create(ep, &code);
}


expression::pointer
expression_abs_integer::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
