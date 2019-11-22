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

#include <ucsdpsys_assemble/expression/addition/linksym.h>
#include <ucsdpsys_assemble/expression/linker_reference.h>


expression_addition_linksym::~expression_addition_linksym()
{
}


expression_addition_linksym::expression_addition_linksym(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression_addition(a_lhs, a_rhs)
{
}


expression_addition_linksym::pointer
expression_addition_linksym::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs)
{
    return pointer(new expression_addition_linksym(a_lhs, a_rhs));
}


expression::pointer
expression_addition_linksym::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();
    if (e1->is_constant() && e2->is_constant())
    {
        expression_linker_reference::pointer e1a =
            boost::dynamic_pointer_cast<expression_linker_reference>(e1);
        return e1a->addition_factory(e2->get_integer_value());
    }
    return create(e1, e2);
}


expression::pointer
expression_addition_linksym::clone(const location &where) const
{
    return create(get_lhs()->clone(where), get_rhs()->clone(where));
}


// vim: set ts=8 sw=4 et :
