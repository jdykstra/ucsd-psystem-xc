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

#include <lib/expression/long_integer.h>


expression_long_integer::~expression_long_integer()
{
}


expression_long_integer::expression_long_integer(
    const location &locn,
    const type::pointer &a_tp,
    const long_integer &a_value
) :
    expression(locn),
    tp(a_tp),
    value(a_value)
{
}


type::pointer
expression_long_integer::get_type(void)
    const
{
    return tp;
}


bool
expression_long_integer::is_constant(void)
    const
{
    return true;
}


bool
expression_long_integer::is_lvalue(void)
    const
{
    return false;
}


void
expression_long_integer::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"long integer constants are never boolean");
}


int
expression_long_integer::get_precedence(void)
    const
{
    return prec_id;
}


expression::side_effect_t
expression_long_integer::has_side_effect(void)
    const
{
    return side_effect_no;
}


rcstring
expression_long_integer::lisp_representation(void)
    const
{
    return
        (
            "(long-int {"
        +
            get_type()->get_name()
        +
            "} "
        +
            value.to_string()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
