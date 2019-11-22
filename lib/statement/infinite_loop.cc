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

#include <lib/label.h>
#include <lib/statement/infinite_loop.h>
#include <lib/translator.h>


statement_infinite_loop::~statement_infinite_loop()
{
}


statement_infinite_loop::statement_infinite_loop(
    const statement::pointer &a_body,
    translator *cntxt
) :
    xlat(*cntxt),
    body(a_body)
{
}


void
statement_infinite_loop::traversal()
    const
{
    pre_order_traversal();
    label::pointer repeat = xlat.label_factory();
    repeat->define_here();
    body->traversal();
    if (body->execution_flows_out())
        repeat->goto_from_here();
    post_order_traversal();
}


void
statement_infinite_loop::pre_order_traversal()
    const
{
    // Do nothing.
}


void
statement_infinite_loop::post_order_traversal()
    const
{
    // Do nothing.
}


bool
statement_infinite_loop::execution_flows_out()
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
