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

#include <lib/expression/logical_not.h>


expression_logical_not::~expression_logical_not()
{
}


expression_logical_not::expression_logical_not(
    const expression::pointer &a_operand,
    translator *cntxt
) :
    expression(a_operand->get_location()),
    xlat(*cntxt),
    operand(a_operand)
{
}


void
expression_logical_not::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    operand->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_logical_not::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_logical_not::post_order_traversal(int)
    const
{
    // Do nothing.
}


int
expression_logical_not::get_precedence()
    const
{
    return prec_unary;
}


type::pointer
expression_logical_not::get_type()
    const
{
    return operand->get_type();
}


bool
expression_logical_not::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_logical_not::has_side_effect()
    const
{
    return operand->has_side_effect();
}


void
expression_logical_not::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


rcstring
expression_logical_not::lisp_representation(void)
    const
{
    return
        (
            "(logical-not "
        +
            operand->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
