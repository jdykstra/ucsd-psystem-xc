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

#include <lib/pcode.h>
#include <lib/symbol/variable.h>
#include <lib/type/nothing.h>

#include <ucsdpsys_compile/expression/store.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_store::~expression_store()
{
}


expression_store::expression_store(
    const expression_store &arg
) :
    expression(arg),
    from(arg.from)
{
}


expression_store::expression_store(
    const expression::pointer &a_from
) :
    expression(a_from->get_location()),
    from(a_from)
{
}


void
expression_store::traversal(int)
    const
{
    pre_order_traversal();
    from->traversal(prec_assign);
    post_order_traversal();
}


void
expression_store::pre_order_traversal()
    const
{
    // Do nothing.
}


void
expression_store::post_order_traversal()
    const
{
    // Do nothing.
}


int
expression_store::get_precedence()
    const
{
    return 0;
}


type::pointer
expression_store::get_type()
    const
{
    // Unlike C, you can't chain assignments.
    return type_nothing::create();
}


bool
expression_store::is_lvalue()
    const
{
    // Unlike C, you can't chain assignments.
    return false;
}


expression::side_effect_t
expression_store::has_side_effect()
    const
{
    return side_effect_yes;
}


void
expression_store::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"store is never bool");
}


// vim: set ts=8 sw=4 et :
