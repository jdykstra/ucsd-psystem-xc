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

#include <lib/expression/bitwise_or.h>


expression_bitwise_or::~expression_bitwise_or()
{
}


expression_bitwise_or::expression_bitwise_or(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs)
{
}


void
expression_bitwise_or::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    lhs->traversal(prec_add);
    infix_order_traversal(pprec);
    rhs->traversal(prec_add);
    post_order_traversal(pprec);
}


void
expression_bitwise_or::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_bitwise_or::infix_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_bitwise_or::post_order_traversal(int)
    const
{
    // Do nothing.
}


int
expression_bitwise_or::get_precedence(void)
    const
{
    return prec_add;
}


type::pointer
expression_bitwise_or::get_type(void)
    const
{
    return lhs->get_type();
}


bool
expression_bitwise_or::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_bitwise_or::has_side_effect(void)
    const
{
    return (lhs->has_side_effect() + rhs->has_side_effect());
}


rcstring
expression_bitwise_or::lisp_representation(void)
    const
{
    return
        (
            "(bitwise-or {"
        +
            get_type()->get_pascal_name()
        +
            "} "
        +
            lhs->lisp_representation()
        +
            " "
        +
            rhs->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
