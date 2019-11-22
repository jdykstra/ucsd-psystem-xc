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

#include <lib/translator.h>
#include <lib/type/long_integer/sized.h>


expression::pointer
translator::integer_minus_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return
        subtraction_expression(real_from_integer_expression_factory(lhs), rhs);
}


expression::pointer
translator::real_minus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return
        subtraction_expression(lhs, real_from_integer_expression_factory(rhs));
}


expression::pointer
translator::subtraction_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    expression::pointer e1 = call_noparam_func_if_necessary(lhs);
    expression::pointer e2 = call_noparam_func_if_necessary(rhs);
    return op_subtraction.dispatch(e1, e2);
}


expression::pointer
translator::int_minus_lint_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return
        lint_minus_lint_expression_factory
        (
            lint_from_integer_expression_factory(lhs),
            rhs
        );
}


expression::pointer
translator::lint_minus_int_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return
        lint_minus_lint_expression_factory
        (
            lhs,
            lint_from_integer_expression_factory(rhs)
        );
}


// vim: set ts=8 sw=4 et :
