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

#include <lib/expression/lint_from_integer.h>
#include <lib/type/long_integer/sized.h>


expression_lint_from_integer::~expression_lint_from_integer()
{
}


expression_lint_from_integer::expression_lint_from_integer(
    const expression::pointer &a_operand
) :
    expression(a_operand->get_location()),
    operand(a_operand)
{
}


void
expression_lint_from_integer::traversal(int pprec)
    const
{
    pre_order_traversal();
    operand->traversal(pprec);
    post_order_traversal();
}


void
expression_lint_from_integer::pre_order_traversal(void)
    const
{
}


void
expression_lint_from_integer::post_order_traversal(void)
    const
{
}


type::pointer
expression_lint_from_integer::get_type(void)
    const
{
    return type_long_integer_sized::create();
}


void
expression_lint_from_integer::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"INTEGER[*] is never boolean");
}


int
expression_lint_from_integer::get_precedence(void)
    const
{
    return operand->get_precedence();
}


bool
expression_lint_from_integer::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_lint_from_integer::has_side_effect(void)
    const
{
    return side_effect_no + operand->has_side_effect();
}


rcstring
expression_lint_from_integer::lisp_representation(void)
    const
{
    return
        (
            "(int2lint {"
        +
            get_type()->get_pascal_name()
        +
            "} "
        +
            operand->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
