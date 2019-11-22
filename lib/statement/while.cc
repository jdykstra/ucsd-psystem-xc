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
#include <lib/statement/while.h>
#include <lib/translator.h>


statement_while::~statement_while()
{
}


statement_while::statement_while(const expression::pointer &a_condition,
        const statement::pointer &a_body, translator *a_xlat) :
    xlat(*a_xlat),
    condition(a_condition),
    body(a_body)
{
}


void
statement_while::traversal()
    const
{
    pre_order_traversal();
    label::pointer start_branch = xlat.label_factory();
    label::pointer end_branch = xlat.label_factory();
    label::pointer body_branch = xlat.label_factory();
    start_branch->define_here();
    condition->logical_traversal(0, body_branch, end_branch, true);
    body_branch->define_here();
    do_order_traversal();
    body->traversal();
    start_branch->goto_from_here();
    end_branch->define_here();
    post_order_traversal();
}


void
statement_while::pre_order_traversal()
    const
{
    // Do nothing.
}


void
statement_while::do_order_traversal()
    const
{
    // Do nothing.
}


void
statement_while::post_order_traversal()
    const
{
    // Do nothing.
}


bool
statement_while::execution_flows_out()
    const
{
    // Because of the processing done in translator::while_statement
    // this is always true.
    return true;
}


// vim: set ts=8 sw=4 et :
