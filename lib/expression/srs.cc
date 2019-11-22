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

#include <lib/ac/assert.h>

#include <lib/expression/srs.h>
#include <lib/type/set/sized.h>


expression_srs::~expression_srs()
{
}


expression_srs::expression_srs(
    const expression_srs &arg
) :
    expression(arg),
    lhs(arg.lhs),
    rhs(arg.rhs),
    tp(arg.tp)
{
}


expression_srs::expression_srs(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs
) :
    expression(a_lhs->get_location() + a_rhs->get_location()),
    lhs(a_lhs),
    rhs(a_rhs),
    tp(type_set_sized::create(a_lhs->get_type()))
{
}


void
expression_srs::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    lhs->traversal(get_precedence());
    infix_order_traversal(pprec);
    rhs->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_srs::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_srs::infix_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_srs::post_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_srs::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(0);
}


int
expression_srs::get_precedence()
    const
{
    return prec_paren;
}


type::pointer
expression_srs::get_type()
    const
{
    return tp;
}


bool
expression_srs::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_srs::has_side_effect()
    const
{
    return (side_effect_no + lhs->has_side_effect() + rhs->has_side_effect());
}


rcstring
expression_srs::lisp_representation(void)
    const
{
    return
        (
            "(srs "
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
