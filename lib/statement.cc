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

#include <lib/statement.h>
#include <lib/pascal/lex.h>


statement::~statement()
{
}


statement::statement() :
    locn(pascal_lex_location())
{
}


statement::statement(const statement &arg) :
    locn(arg.locn)
{
}


statement &
statement::operator=(const statement &arg)
{
    if (this != &arg)
    {
        locn = arg.locn;
    }
    return *this;
}


void
statement::traversal()
    const
{
    // Do nothing.
}


bool
statement::empty()
    const
{
    return false;
}


void
statement::set_location(const location &arg)
{
    if (!arg.empty())
        locn = arg;
}


bool
statement::is_compound(void)
    const
{
    return false;
}


bool
statement::execution_flows_in(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
