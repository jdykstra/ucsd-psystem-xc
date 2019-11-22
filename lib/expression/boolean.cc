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

#include <lib/expression/boolean.h>
#include <lib/label.h>
#include <lib/pascal/lex.h>
#include <lib/type/boolean.h>


expression_boolean::~expression_boolean()
{
}


expression_boolean::expression_boolean(const location &where, bool a_value) :
    expression(where),
    value(a_value)
{
}


bool
expression_boolean::is_constant(void)
    const
{
    return true;
}


int
expression_boolean::get_precedence(void)
    const
{
    return prec_id;
}


type::pointer
expression_boolean::get_type(void)
    const
{
    return type_boolean::create();
}


bool
expression_boolean::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_boolean::has_side_effect(void)
    const
{
    return side_effect_no;
}


void
expression_boolean::logical_traversal(int, label::pointer &true_branch,
    label::pointer &false_branch, bool fall_through_preference) const
{
    if (value)
    {
        if (!fall_through_preference)
            true_branch->goto_from_here();
    }
    else
    {
        if (fall_through_preference)
            false_branch->goto_from_here();
    }
}


bool
expression_boolean::get_boolean_value(void)
    const
{
    return value;
}


long
expression_boolean::get_integer_value(void)
    const
{
    return value;
}


rcstring
expression_boolean::lisp_representation(void)
    const
{
    return (value ? "true" : "false");
}


rcstring
expression_boolean::get_printable_value(void)
    const
{
    return lisp_representation();
}


// vim: set ts=8 sw=4 et :
