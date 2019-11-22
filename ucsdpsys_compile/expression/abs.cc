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

#include <ucsdpsys_compile/expression/abs.h>


expression_abs::~expression_abs()
{
}


expression_abs::expression_abs(const expression::pointer &a_operand) :
    expression(a_operand->get_location()),
    operand(a_operand)
{
}


void
expression_abs::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    operand->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_abs::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_abs::post_order_traversal(int)
    const
{
    // Do nothing.
}


type::pointer
expression_abs::get_type(void)
    const
{
    return operand->get_type();
}


void
expression_abs::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(!"the result of an abs call is never boolean");
}


rcstring
expression_abs::lisp_representation(void)
    const
{
    return
        (
            "(abs {"
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


int
expression_abs::get_precedence(void)
    const
{
    return prec_id;
}


expression::side_effect_t
expression_abs::has_side_effect(void)
    const
{
    return side_effect_no + operand->has_side_effect();
}


bool
expression_abs::is_constant(void)
    const
{
    return operand->is_constant();
}


bool
expression_abs::is_lvalue(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
