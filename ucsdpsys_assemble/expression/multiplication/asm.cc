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

#include <lib/type/integer.h>

#include <ucsdpsys_assemble/expression/integer/asm.h>
#include <ucsdpsys_assemble/expression/multiplication/asm.h>


expression_multiplication_asm::~expression_multiplication_asm()
{
}


expression_multiplication_asm::expression_multiplication_asm(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression_multiplication(a_lhs, a_rhs)
{
}


expression_multiplication_asm::pointer
expression_multiplication_asm::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs)
{
    return pointer(new expression_multiplication_asm(a_lhs, a_rhs));
}


type::pointer
expression_multiplication_asm::get_type(void)
    const
{
    return type_integer::create();
}


expression::pointer
expression_multiplication_asm::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();
    if (e1->is_constant() && e2->is_constant())
    {
        long n1 = e1->get_integer_value();
        long n2 = e2->get_integer_value();
        long n = n1 * n2;
        return expression_integer_asm::create(n, get_location());
    }
    return create(e1, e2);
}


expression::pointer
expression_multiplication_asm::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn));
}


// vim: set ts=8 sw=4 et :
