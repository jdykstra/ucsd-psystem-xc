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

#include <lib/expression/scan.h>
#include <lib/type/integer.h>


expression_scan::~expression_scan()
{
}


expression_scan::expression_scan(
    const expression::pointer &a_maxdisp,
    int a_mask,
    const expression::pointer &a_value,
    const expression::pointer &a_start
) :
    expression(a_maxdisp->get_location() + a_value->get_location()
        + a_start->get_location()),
    maxdisp(a_maxdisp),
    mask(a_mask),
    value(a_value),
    start(a_start)
{
}


void
expression_scan::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(!"never boolean");
}


int
expression_scan::get_precedence(void)
    const
{
    return prec_paren;
}


type::pointer
expression_scan::get_type(void)
    const
{
    return type_integer::create();
}


bool
expression_scan::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_scan::has_side_effect(void)
    const
{
    return side_effect_no;
}


rcstring
expression_scan::lisp_representation(void)
    const
{
    return
        (
            "(scan "
        +
            maxdisp->lisp_representation()
        +
            rcstring::printf(" %d ", mask)
        +
            value->lisp_representation()
        +
            " "
        +
            start->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
