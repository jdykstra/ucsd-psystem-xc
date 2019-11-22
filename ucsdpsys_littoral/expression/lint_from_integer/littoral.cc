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

#include <ucsdpsys_littoral/expression/lint_from_integer/littoral.h>


expression_lint_from_integer_littoral::~expression_lint_from_integer_littoral()
{
}


expression_lint_from_integer_littoral::expression_lint_from_integer_littoral(
    const expression::pointer &a_operand
) :
    expression_lint_from_integer(a_operand)
{
}


expression_lint_from_integer_littoral::pointer
expression_lint_from_integer_littoral::create(
    const expression::pointer &a_operand)
{
    return pointer(new expression_lint_from_integer_littoral(a_operand));
}


expression::pointer
expression_lint_from_integer_littoral::optimize(void)
    const
{
    return create(get_operand());
}


expression::pointer
expression_lint_from_integer_littoral::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn));
}


bool
expression_lint_from_integer_littoral::is_constant(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
