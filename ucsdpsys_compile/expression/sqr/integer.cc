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

#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/sqr/integer.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_sqr_integer::~expression_sqr_integer()
{
}


expression_sqr_integer::expression_sqr_integer(
    const expression::pointer &a_operand,
    translator_compile *a_code
) :
    expression_sqr(a_operand),
    code(*a_code)
{
}


expression_sqr_integer::pointer
expression_sqr_integer::create(const expression::pointer &a_operand,
    translator_compile *a_code)
{
    return pointer(new expression_sqr_integer(a_operand, a_code));
}


expression::pointer
expression_sqr_integer::optimize(void)
    const
{
    expression::pointer ep = get_operand()->optimize();
    if (ep->is_constant())
    {
        long n = ep->get_integer_value();
        n = n * n;
        return expression_integer_compile::create(get_location(), n, &code);
    }
    return expression_sqr_integer::create(ep, &code);
}


expression::pointer
expression_sqr_integer::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


void
expression_sqr_integer::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(SQI);
}


// vim: set ts=8 sw=4 et :
