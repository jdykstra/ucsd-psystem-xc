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

#include <lib/statement/label.h>


statement_label::~statement_label()
{
}


statement_label::statement_label(
    const symbol::pointer &a_sym,
    const statement::pointer &a_body
) :
    sym(boost::dynamic_pointer_cast<symbol_label>(a_sym)),
    body(a_body)
{
    assert(sym);
}


void
statement_label::traversal(void)
    const
{
    pre_order_traversal(sym);

    // tell the symbol we used it
    sym->define_here();

    body->traversal();
    post_order_traversal();
}


void
statement_label::pre_order_traversal(const symbol_label::pointer &)
    const
{
    // Do nothing.
}


void
statement_label::post_order_traversal(void)
    const
{
    // Do nothing.
}


bool
statement_label::execution_flows_out(void)
    const
{
    return body->execution_flows_out();
}


bool
statement_label::execution_flows_in(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
