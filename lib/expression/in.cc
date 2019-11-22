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

#include <lib/debug.h>
#include <lib/expression/in.h>
#include <lib/type/boolean.h>


expression_in::~expression_in()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in::expression_in(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_in::expression_in(const expression_in &arg) :
    expression(arg),
    lhs(arg.lhs),
    rhs(arg.rhs)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
expression_in::traversal(int pprec)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    pre_order_traversal(pprec);
    lhs->traversal(get_precedence());
    infix_order_traversal(pprec);
    rhs->traversal(get_precedence());
    post_order_traversal(pprec);
    DEBUG(1, "}");
}


void
expression_in::pre_order_traversal(int)
    const
{
    // Do nothing.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
expression_in::infix_order_traversal(int)
    const
{
    // Do nothing.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
expression_in::post_order_traversal(int)
    const
{
    // Do nothing.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


int
expression_in::get_precedence()
    const
{
    return prec_rel;
}


bool
expression_in::is_lvalue()
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return false;
}


expression::side_effect_t
expression_in::has_side_effect()
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return (lhs->has_side_effect() + rhs->has_side_effect());
}


type::pointer
expression_in::get_type()
    const
{
    return type_boolean::create();
}


bool
expression_in::is_constant()
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return (lhs->is_constant() && rhs->is_constant());
}


rcstring
expression_in::lisp_representation(void)
    const
{
    return
        (
            "(in "
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
