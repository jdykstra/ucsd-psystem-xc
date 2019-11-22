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

#include <lib/expression/enum_tag.h>
#include <lib/variable/name.h>


expression_enum_tag::~expression_enum_tag()
{
}


expression_enum_tag::expression_enum_tag(
    const variable_name &a_name,
    const type::pointer &a_typ,
    int a_value
) :
    expression(a_name.get_location()),
    name(a_name.get_name()),
    typ(a_typ),
    value(a_value)
{
    assert(value >= 0);
    assert(value < 32768);
}


type::pointer
expression_enum_tag::get_type(void)
    const
{
    return typ;
}


long
expression_enum_tag::get_integer_value(void)
    const
{
    return value;
}


void
expression_enum_tag::logical_traversal(int pprec, label::pointer &true_lbl,
    label::pointer &false_lbl, bool fall_thru_pref) const
{
    traversal(pprec);
    if (fall_thru_pref)
        false_lbl->branch_from_here_if_false();
    else
        true_lbl->branch_from_here_if_true();
}


int
expression_enum_tag::get_precedence(void)
    const
{
    return prec_id;
}


bool
expression_enum_tag::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_enum_tag::has_side_effect(void)
    const
{
    return side_effect_no;
}


rcstring
expression_enum_tag::lisp_representation(void)
    const
{
    return "(enum-tag " + name.quote_c() + ")";
}


bool
expression_enum_tag::is_constant(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
