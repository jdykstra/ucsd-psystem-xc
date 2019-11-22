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

#include <lib/expression/logical_and.h>
#include <lib/label.h>
#include <lib/translator.h>


expression_logical_and::~expression_logical_and()
{
}


expression_logical_and::expression_logical_and(
    const expression_logical_and &arg
) :
    expression(arg),
    xlat(arg.xlat),
    lhs(arg.lhs),
    rhs(arg.rhs)
{
}


expression_logical_and::expression_logical_and(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator *cntxt
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    xlat(*cntxt),
    lhs(a_lhs),
    rhs(a_rhs)
{
}


void
expression_logical_and::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    lhs->traversal(get_precedence());
    infix_order_traversal(pprec);
    rhs->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_logical_and::pre_order_traversal(int)
    const
{
}


void
expression_logical_and::infix_order_traversal(int)
    const
{
}


void
expression_logical_and::post_order_traversal(int)
    const
{
}


void
expression_logical_and::logical_traversal(int pprec,
    label::pointer &true_br, label::pointer &false_br, bool fall_thru) const
{
    pre_order_traversal(pprec);
    label::pointer partial_br = xlat.label_factory();
    lhs->logical_traversal(get_precedence(), partial_br, false_br, true);
    infix_order_traversal(pprec);
    partial_br->define_here();
    rhs->logical_traversal(get_precedence(), true_br, false_br, fall_thru);
    post_order_traversal(pprec);
}


int
expression_logical_and::get_precedence()
    const
{
    return prec_mul;
}


type::pointer
expression_logical_and::get_type()
    const
{
    return lhs->get_type();
}


bool
expression_logical_and::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_logical_and::has_side_effect()
    const
{
    return (lhs->has_side_effect() + rhs->has_side_effect());
}


rcstring
expression_logical_and::lisp_representation(void)
    const
{
    return
        (
            "(logical-and "
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
