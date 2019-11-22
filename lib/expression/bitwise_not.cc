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

#include <lib/expression/bitwise_not.h>


expression_bitwise_not::~expression_bitwise_not()
{
}


expression_bitwise_not::expression_bitwise_not(
    const expression::pointer &a_operand
) :
    expression(a_operand->get_location()),
    operand(a_operand)
{
}


void
expression_bitwise_not::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    operand->traversal(prec_unary);
    post_order_traversal(pprec);
}


void
expression_bitwise_not::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_bitwise_not::post_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_bitwise_not::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"should not be possible");
}


int
expression_bitwise_not::get_precedence(void)
    const
{
    return prec_unary;
}


type::pointer
expression_bitwise_not::get_type(void)
    const
{
    return operand->get_type();
}


bool
expression_bitwise_not::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_bitwise_not::has_side_effect(void)
    const
{
    return side_effect_no + operand->has_side_effect();
}


rcstring
expression_bitwise_not::lisp_representation(void)
    const
{
    return
        (
            "(bitwise-not {"
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
