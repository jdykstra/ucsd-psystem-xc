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

#include <lib/expression/addition.h>
#include <lib/type.h>


expression_addition::~expression_addition()
{
}


expression_addition::expression_addition(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs)
{
}


expression_addition::expression_addition(const expression_addition &arg) :
    expression(arg),
    lhs(arg.lhs),
    rhs(arg.rhs)
{
}


void
expression_addition::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    lhs->traversal(get_precedence());
    infix_order_traversal(pprec);
    rhs->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_addition::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_addition::infix_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_addition::post_order_traversal(int)
    const
{
    // Do nothing.
}


int
expression_addition::get_precedence()
    const
{
    return prec_add;
}


bool
expression_addition::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_addition::has_side_effect()
    const
{
    return (lhs->has_side_effect() + rhs->has_side_effect());
}


type::pointer
expression_addition::get_type()
    const
{
    // Both are supposed to be of the same type.
    if (rhs->is_error())
        return rhs->get_type();
    return lhs->get_type();
}


bool
expression_addition::is_constant()
    const
{
    return (lhs->is_constant() && rhs->is_constant());
}


rcstring
expression_addition::lisp_representation(void)
    const
{
    return
        (
            "(addition {"
        +
            get_type()->get_pascal_name()
        +
            "} ["
        +
            get_location().representation()
        +
            "] "
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


void
expression_addition::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(!"addition expressions are never boolean");
}


// vim: set ts=8 sw=4 et :
