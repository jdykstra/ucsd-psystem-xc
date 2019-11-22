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
#include <lib/statement/until.h>
#include <lib/translator.h>


statement_until::~statement_until()
{
}


statement_until::statement_until(
    const statement_list &a_body,
    const expression::pointer &a_condition,
    translator *a_xlat
) :
    xlat(*a_xlat),
    body(a_body),
    condition(a_condition)
{
}


void
statement_until::traversal()
    const
{
    pre_order_traversal();
    label::pointer start_branch = xlat.label_factory();
    label::pointer end_branch = xlat.label_factory();
    start_branch->define_here();
    body.traversal();
    until_order_traversal();
    condition->logical_traversal(0, end_branch, start_branch, true);
    end_branch->define_here();
    post_order_traversal();
}


void
statement_until::pre_order_traversal()
    const
{
    // Do nothing.
}


void
statement_until::until_order_traversal()
    const
{
    // Do nothing.
}


void
statement_until::post_order_traversal()
    const
{
    // Do nothing.
}


bool
statement_until::execution_flows_out()
    const
{
    return body.execution_flows_out() && condition->execution_flows_out();
}


bool
statement_until::execution_flows_in()
    const
{
    return body.execution_flows_in();
}


// vim: set ts=8 sw=4 et :
