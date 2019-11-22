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

#include <lib/expression/real_from_integer.h>
#include <lib/type/real.h>


expression_real_from_integer::~expression_real_from_integer()
{
}


expression_real_from_integer::expression_real_from_integer(
    const expression::pointer &a_operand
) :
    expression(a_operand->get_location()),
    operand(a_operand)
{
}


void
expression_real_from_integer::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    operand->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_real_from_integer::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_real_from_integer::post_order_traversal(int)
    const
{
    // Do nothing.
}


int
expression_real_from_integer::get_precedence()
    const
{
    return operand->get_precedence();
}


type::pointer
expression_real_from_integer::get_type()
    const
{
    return type_real::create();
}


bool
expression_real_from_integer::is_lvalue()
    const
{
    // If it was before, it isn't now.
    return false;
}


expression::side_effect_t
expression_real_from_integer::has_side_effect()
    const
{
    switch (operand->has_side_effect())
    {
    case side_effect_no:
        break;

    case side_effect_partial:
    case side_effect_yes:
        return side_effect_partial;
    }
    return side_effect_no;
}


bool
expression_real_from_integer::is_constant()
    const
{
    return operand->is_constant();
}


void
expression_real_from_integer::logical_traversal(int pprec,
    label::pointer &, label::pointer &, bool) const
{
    assert(0);
    traversal(pprec);
}


rcstring
expression_real_from_integer::lisp_representation(void)
    const
{
    return
        (
            "(real-from-integer "
        +
            operand->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
