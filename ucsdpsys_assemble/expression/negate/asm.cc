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
#include <ucsdpsys_assemble/expression/negate/asm.h>


expression_negate_asm::~expression_negate_asm()
{
}


expression_negate_asm::expression_negate_asm(
    const expression::pointer &a_operand,
    assembler *a_context
) :
    expression_negate(a_operand),
    context(a_context)
{
}


expression_negate_asm::pointer
expression_negate_asm::create(const expression::pointer &a_operand,
    assembler *a_context)
{
    return pointer(new expression_negate_asm(a_operand, a_context));
}


expression::pointer
expression_negate_asm::optimize(void)
    const
{
    expression::pointer e = get_operand()->optimize();
    if (e->is_constant())
    {
        long n = e->get_integer_value();
        return expression_integer_asm::create(-n, get_location());
    }
    return create(e, context);
}


expression::pointer
expression_negate_asm::clone(const location &where)
    const
{
    expression::pointer e = get_operand()->clone(where);
    return create(e, context);
}


// vim: set ts=8 sw=4 et :
