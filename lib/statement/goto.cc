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

#include <lib/statement/goto.h>


statement_goto::~statement_goto()
{
}


statement_goto::statement_goto(const symbol::pointer &asym) :
    sym(boost::dynamic_pointer_cast<symbol_label>(asym))
{
    assert(sym);
}


void
statement_goto::traversal(void)
    const
{
    // tell the symbol we used it
    sym->goto_from_here();

    post_order_traversal(sym);
}


void
statement_goto::post_order_traversal(const symbol::pointer &)
    const
{
    // Do nothing.
}


bool
statement_goto::execution_flows_out(void)
    const
{
    return false;
}


label::pointer
statement_goto::get_label(void)
    const
{
    return sym->get_label();
}


// vim: set ts=8 sw=4 et :
