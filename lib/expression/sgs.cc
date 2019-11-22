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

#include <lib/expression/sgs.h>
#include <lib/set.h>
#include <lib/type/integer.h>
#include <lib/type/set/sized.h>
#include <lib/type/subrange.h>


expression_sgs::~expression_sgs()
{
}


expression_sgs::expression_sgs(
    const expression_sgs &arg
) :
    expression(arg),
    value(arg.value),
    tp(arg.tp)
{
}


expression_sgs::expression_sgs(
    const expression::pointer &a_value
) :
    expression(a_value->get_location()),
    value(a_value),
    tp(type_set_sized::create(a_value->get_type()))
{
}


type::pointer
expression_sgs::get_type()
    const
{
    return tp;
}


void
expression_sgs::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    value->traversal(get_precedence());
    post_order_traversal(pprec);
}


void
expression_sgs::pre_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_sgs::post_order_traversal(int)
    const
{
    // Do nothing.
}


void
expression_sgs::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(0);
}


int
expression_sgs::get_precedence()
    const
{
    return prec_paren;
}


bool
expression_sgs::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_sgs::has_side_effect()
    const
{
    return (side_effect_no + value->has_side_effect());
}


rcstring
expression_sgs::lisp_representation(void)
    const
{
    return
        (
            "(sgs "
        +
            value->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
