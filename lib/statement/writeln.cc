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

#include <lib/expression/list.h>
#include <lib/statement/writeln.h>


statement_writeln::~statement_writeln()
{
}


statement_writeln::statement_writeln(const expression_list &a_args) :
    args(a_args)
{
}


void
statement_writeln::traversal()
    const
{
    pre_order_traversal();
    for (size_t j = 0; j < args.size(); ++j)
    {
        expression::pointer ep = args.get(j);
        ep->traversal(0);
        infix_order_traversal(ep, j + 1 >= args.size());
    }
    post_order_traversal();
}


void
statement_writeln::pre_order_traversal()
    const
{
    // Do nothing.
}


void
statement_writeln::infix_order_traversal(const expression::pointer &, bool)
    const
{
    // Do nothing.
}


void
statement_writeln::post_order_traversal()
    const
{
    // Do nothing.
}

bool
statement_writeln::execution_flows_out()
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
