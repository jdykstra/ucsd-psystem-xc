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

#include <lib/expression/eq.h>
#include <lib/type/boolean.h>


expression_eq::~expression_eq()
{
}


expression_eq::expression_eq(
    const expression_eq &arg
) :
    expression(arg),
    lhs(arg.lhs),
    rhs(arg.rhs)
{
}


expression_eq::expression_eq(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs)
{
}


void
expression_eq::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    lhs->traversal(get_precedence());
    infix_order_traversal(pprec);
    rhs->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_eq::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_eq::infix_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_eq::post_order_traversal(int)
    const
{
    // Do nothing.
}


int
expression_eq::get_precedence()
    const
{
    return prec_eq;
}


bool
expression_eq::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_eq::has_side_effect()
    const
{
    return (lhs->has_side_effect() + rhs->has_side_effect());
}


type::pointer
expression_eq::get_type()
    const
{
    return type_boolean::create();
}


bool
expression_eq::is_constant()
    const
{
    return (lhs->is_constant() && rhs->is_constant());
}


rcstring
expression_eq::lisp_representation(void)
    const
{
    return
        (
            "(eq "
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
