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

#include <lib/statement/expression.h>
#include <lib/expression/empty.h>


statement_expression::~statement_expression()
{
}


statement_expression::statement_expression(const expression::pointer &a) :
    arg(a)
{
    set_location(arg->get_location());
}


statement_expression::pointer
statement_expression::create(const expression::pointer &a)
{
    return pointer(new statement_expression(a));
}


bool
statement_expression::execution_flows_out()
    const
{
    //
    // FIXME: eventually, we need to ask the expression whether
    // execution flows out.  E.g. Pascal procedures exit() and halt() do
    // not flow out.
    //
    return true;
}


void
statement_expression::traversal()
    const
{
    arg->traversal(0);

    // No need to pop anything, it is an already reported error if the
    // expression was not void.
    //
    // BUT... to pop one word, use "FJP 0"
    //        to pop one real, use "ADDI / FJP 0"
}


bool
statement_expression::empty(void)
    const
{
    return !!dynamic_cast<const expression_empty *>(arg.get());
}


// vim: set ts=8 sw=4 et :
