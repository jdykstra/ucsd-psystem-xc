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

#include <ucsdpsys_assemble/expression/addition/segrel.h>
#include <ucsdpsys_assemble/expression/segment_relative.h>
#include <ucsdpsys_assemble/type/segment_relative.h>


expression_addition_segrel::~expression_addition_segrel()
{
}


expression_addition_segrel::expression_addition_segrel(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression_addition(a_lhs, a_rhs)
{
}


expression_addition_segrel::pointer
expression_addition_segrel::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs)
{
    assert(type_segment_relative::is_a(a_lhs));
    assert(type_integer::is_a(a_rhs));
    return pointer(new expression_addition_segrel(a_lhs, a_rhs));
}


type::pointer
expression_addition_segrel::get_type(void)
    const
{
    return type_segment_relative::create();
}


expression::pointer
expression_addition_segrel::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();
    if (e1->is_constant() && e2->is_constant())
    {
        long n1 = e1->get_integer_value();
        long n2 = e2->get_integer_value();
        long n = n1 + n2;
        return expression_segment_relative::create(get_location(), n);
    }
    return create(e1, e2);
}


expression::pointer
expression_addition_segrel::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn));
}


// vim: set ts=8 sw=4 et :
