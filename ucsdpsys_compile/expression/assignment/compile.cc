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

#include <ucsdpsys_compile/expression/assignment/compile.h>


expression_assignment_compile::~expression_assignment_compile()
{
}


expression_assignment_compile::expression_assignment_compile(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_assignment(a_lhs, a_rhs),
    code(*cntxt)
{
}


expression_assignment_compile::pointer
expression_assignment_compile::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_assignment_compile(a_lhs, a_rhs, cntxt));
}


expression::pointer
expression_assignment_compile::optimize(void)
    const
{
    expression::pointer a1 = get_lhs();
    expression::pointer a2 = get_rhs();
    expression::pointer e1 = a1->optimize();
    expression::pointer e2 = a2->optimize();
    return expression_assignment_compile::create(e1, e2, &code);
}


expression::pointer
expression_assignment_compile::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :
