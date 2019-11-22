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

#include <lib/expression/assignment.h>
#include <lib/type/nothing.h>


expression_assignment::~expression_assignment()
{
}


expression_assignment::expression_assignment(
    const expression_assignment &arg
) :
    expression(arg),
    lhs(arg.lhs),
    rhs(arg.rhs)
{
}


expression_assignment::expression_assignment(
    const expression::pointer &arg1,
    const expression::pointer &arg2
) :
    expression(arg1->get_location() + arg2->get_location()),
    lhs(arg1),
    rhs(arg2)
{
}


void
expression_assignment::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    lhs->traversal(get_precedence() + 1); // right assoc
    infix_order_traversal(pprec);
    rhs->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_assignment::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_assignment::infix_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_assignment::post_order_traversal(int)
    const
{
    // Do nothing.
}


int
expression_assignment::get_precedence()
    const
{
    return prec_assign;
}


bool
expression_assignment::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_assignment::has_side_effect()
    const
{
    return side_effect_yes;
}


void
expression_assignment::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


type::pointer
expression_assignment::get_type()
    const
{
    return type_nothing::create();
}


rcstring
expression_assignment::lisp_representation(void)
    const
{
    return
        (
            "(assignment "
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
