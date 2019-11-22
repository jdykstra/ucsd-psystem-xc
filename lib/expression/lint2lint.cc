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

#include <lib/expression/lint2lint.h>


expression_lint2lint::~expression_lint2lint()
{
}


expression_lint2lint::expression_lint2lint(
    const type::pointer &a_to,
    const expression::pointer &a_from
) :
    expression(a_from->get_location()),
    to(a_to),
    from(a_from)
{
}


void
expression_lint2lint::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(!"long integers are never boolean");
}


type::pointer
expression_lint2lint::get_type(void)
    const
{
    return to;
}


bool
expression_lint2lint::is_lvalue(void)
    const
{
    return false;
}


int
expression_lint2lint::get_precedence(void)
    const
{
    return from->get_precedence();
}


expression::side_effect_t
expression_lint2lint::has_side_effect(void)
    const
{
    return side_effect_no + from->has_side_effect();
}


rcstring
expression_lint2lint::lisp_representation(void)
    const
{
    return
        (
            "(lin2lint {"
        +
            to->get_pascal_name()
        +
            "} "
        +
            from->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :
