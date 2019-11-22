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

#include <ucsdpsys_assemble/expression/integer/asm.h>
#include <ucsdpsys_assemble/expression/xor/asm.h>


expression_xor_asm::~expression_xor_asm()
{
}


expression_xor_asm::expression_xor_asm(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    assembler *a_context
) :
    expression_xor(a_lhs, a_rhs),
    context(a_context)
{
}


expression_xor_asm::pointer
expression_xor_asm::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, assembler *a_context)
{
    return pointer(new expression_xor_asm(a_lhs, a_rhs, a_context));
}


expression::pointer
expression_xor_asm::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();
    if (e1->is_constant() && e2->is_constant())
    {
        long v1 = e1->get_integer_value();
        long v2 = e2->get_integer_value();
        long v = v1 ^ v2;
        return expression_integer_asm::create(v, get_location());
    }
    return create(e1, e2, context);
}


expression::pointer
expression_xor_asm::clone(const location &locn)
    const
{
    expression::pointer e1 = get_lhs()->clone(locn);
    expression::pointer e2 = get_rhs()->clone(locn);
    return create(e1, e2, context);
}


// vim: set ts=8 sw=4 et :
