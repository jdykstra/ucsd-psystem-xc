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

#include <lib/expression/set.h>
#include <lib/type/set.h>


expression_set::~expression_set()
{
}


expression_set::expression_set(
    const expression_set &arg
) :
    expression(arg),
    tp(arg.tp),
    value(arg.value)
{
}


expression_set::expression_set(
    const location &a_locn,
    const type::pointer &a_tp,
    const set &a_value
) :
    expression(a_locn),
    tp(a_tp),
    value(a_value)
{
    assert(type_set::is_a(tp));
}


void
expression_set::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(0);
}


int
expression_set::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_set::get_type()
    const
{
    return tp;
}


bool
expression_set::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_set::has_side_effect()
    const
{
    return side_effect_no;
}


bool
expression_set::is_constant()
    const
{
    return true;
}


rcstring
expression_set::lisp_representation(void)
    const
{
    return
        (
            "(set-constant {"
        +
            get_type()->get_pascal_name()
        +
            "} "
        +
            value.representation()
        +
            ")"
        );
}


bool
expression_set::is_empty_set(void)
    const
{
    return value.empty();
}


// vim: set ts=8 sw=4 et :
