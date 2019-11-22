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

#include <lib/expression.h>
#include <lib/label.h>
#include <lib/statement/if.h>
#include <lib/translator.h>


statement_if::~statement_if()
{
}


statement_if::statement_if(const expression::pointer &a_cond,
        const statement::pointer &a_then, const statement::pointer &a_else,
        translator *a_xlat) :
    xlat(*a_xlat),
    condition(a_cond),
    then_clause(a_then),
    else_clause(a_else)
{
    if (else_clause && else_clause->empty())
        else_clause.reset();
}


void
statement_if::traversal()
    const
{
    label::pointer true_branch = xlat.label_factory();
    label::pointer false_branch = xlat.label_factory();
    pre_order_traversal();
    condition->logical_traversal(0, true_branch, false_branch, true);
    then_order_traversal();
    true_branch->define_here();
    then_clause->traversal();
    if (!else_clause)
    {
        false_branch->define_here();
    }
    else if (!then_clause->execution_flows_out())
    {
        false_branch->define_here();
        else_order_traversal();
        else_clause->traversal();
    }
    else
    {
        label::pointer finish_branch = xlat.label_factory();
        finish_branch->goto_from_here();
        false_branch->define_here();
        else_order_traversal();
        else_clause->traversal();
        finish_branch->define_here();
    }
    post_order_traversal();
}


void
statement_if::pre_order_traversal()
    const
{
    // Do nothing.
}


void
statement_if::then_order_traversal()
    const
{
    // Do nothing.
}


void
statement_if::else_order_traversal()
    const
{
    // Do nothing.
}


void
statement_if::post_order_traversal()
    const
{
    // Do nothing.
}


bool
statement_if::empty()
    const
{
    return
        (
            condition->has_side_effect() == expression::side_effect_no
        &&
            then_clause->empty()
        &&
            !else_clause
        );
}


bool
statement_if::execution_flows_out()
    const
{
    return
        (
            then_clause->execution_flows_out()
        ||
            (!else_clause || else_clause->execution_flows_out())
        );
}


// vim: set ts=8 sw=4 et :
