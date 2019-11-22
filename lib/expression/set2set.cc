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

#include <lib/expression/set2set.h>
#include <lib/type/set.h>


expression_set2set::~expression_set2set()
{
}


expression_set2set::expression_set2set(
    const expression_set2set &arg
) :
    expression(arg),
    to(arg.to),
    from(arg.from)
{
}


expression_set2set::expression_set2set(
    const type::pointer &a_to,
    const expression::pointer &a_from
) :
    expression(a_from->get_location()),
    to(a_to),
    from(a_from)
{
    assert(type_set::is_a(to));
    assert(type_set::is_a(from->get_type()));
}


void
expression_set2set::traversal(int pprec)
    const
{
    pre_order_traversal(pprec);
    from->traversal(pprec);
    post_order_traversal(pprec);
}


void
expression_set2set::pre_order_traversal(int)
    const
{
}


void
expression_set2set::post_order_traversal(int)
    const
{
}


void
expression_set2set::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(0);
}


int
expression_set2set::get_precedence()
    const
{
    return from->get_precedence();
}


type::pointer
expression_set2set::get_type()
    const
{
    return to;
}

bool
expression_set2set::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_set2set::has_side_effect()
    const
{
    return (side_effect_no + from->has_side_effect());
}


rcstring
expression_set2set::lisp_representation(void)
    const
{
    return
        (
            "(set2set {"
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
