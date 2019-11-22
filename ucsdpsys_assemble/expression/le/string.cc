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

#include <ucsdpsys_assemble/expression/boolean/asm.h>
#include <ucsdpsys_assemble/expression/le/string.h>


expression_le_string::~expression_le_string()
{
}


expression_le_string::expression_le_string(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression_le(a_lhs, a_rhs)
{
}


expression_le_string::pointer
expression_le_string::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs)
{
    return pointer(new expression_le_string(a_lhs, a_rhs));
}


expression::pointer
expression_le_string::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();
    if (e1->is_constant() && e2->is_constant())
    {
        rcstring n1 = e1->get_string_value();
        rcstring n2 = e2->get_string_value();
        bool value = (n1 <= n2);
        return expression_boolean_asm::create(get_location(), value);
    }
    return create(e1, e2);
}


expression::pointer
expression_le_string::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn));
}


// vim: set ts=8 sw=4 et :
