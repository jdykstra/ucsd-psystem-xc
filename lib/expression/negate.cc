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

#include <lib/expression/negate.h>
#include <lib/pascal/lex.h>
#include <lib/type/error.h>
#include <lib/type/reference.h>


expression_negate::~expression_negate()
{
}


expression_negate::expression_negate(
    const expression_negate &rhs
) :
    expression(rhs),
    operand(rhs.operand),
    ptype(rhs.ptype)
{
}


expression_negate::expression_negate(
    const expression::pointer &arg
) :
    expression(arg->get_location()),
    operand(arg),
    ptype(operand->get_type())
{
}


int
expression_negate::get_precedence()
    const
{
    return prec_unary;
}


type::pointer
expression_negate::get_type()
    const
{
    return ptype;
}


void
expression_negate::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    operand->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_negate::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_negate::post_order_traversal(int)
    const
{
    // Do nothing.
}


expression::side_effect_t
expression_negate::has_side_effect()
    const
{
    return operand->has_side_effect();
}


bool
expression_negate::is_lvalue()
    const
{
    return false;
}


bool
expression_negate::is_constant()
    const
{
    return operand->is_constant();
}


rcstring
expression_negate::lisp_representation(void)
    const
{
    return
        (
            "(negate "
        +
            operand->lisp_representation()
        +
            ")"
        );
}


void
expression_negate::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(!"the result of a unary minus expression is never boolean");
}


// vim: set ts=8 sw=4 et :
