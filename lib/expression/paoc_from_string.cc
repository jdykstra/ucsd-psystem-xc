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

#include <lib/expression/paoc_from_string.h>


expression_paoc_from_string::~expression_paoc_from_string()
{
}


expression_paoc_from_string::expression_paoc_from_string(
    const expression::pointer &a_rhs,
    const type::pointer &a_tp
) :
    expression(a_rhs->get_location()),
    rhs(a_rhs),
    tp(a_tp)
{
}


type::pointer
expression_paoc_from_string::get_type(void)
    const
{
    return tp;
}


bool
expression_paoc_from_string::is_constant(void)
    const
{
    return rhs->is_constant();
}


int
expression_paoc_from_string::get_precedence(void)
    const
{
    return rhs->get_precedence();
}


bool
expression_paoc_from_string::is_lvalue(void)
    const
{
    return true;
}


expression::side_effect_t
expression_paoc_from_string::has_side_effect(void)
    const
{
    return rhs->has_side_effect();
}


rcstring
expression_paoc_from_string::lisp_representation(void)
    const
{
    return
        (
            "(paoc-from-string {"
        +
            tp->get_pascal_name()
        +
            "} "
        +
            rhs->lisp_representation()
        +
            ")"
        );
}


void
expression_paoc_from_string::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"constant strings are never boolean");
}


// vim: set ts=8 sw=4 et :
