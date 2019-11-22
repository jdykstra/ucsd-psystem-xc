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

#include <lib/statement/compound.h>


statement_compound::~statement_compound()
{
}


statement_compound::statement_compound()
{
}


statement_compound::pointer
statement_compound::create(void)
{
    return pointer(new statement_compound());
}


statement_compound::statement_compound(const statement_list &sl) :
    content(sl)
{
}


statement_compound::pointer
statement_compound::create(const statement_list &sl)
{
    return pointer(new statement_compound(sl));
}


statement_compound::statement_compound(const statement_compound &arg) :
    statement(arg),
    content(arg.content)
{
}


statement_compound &
statement_compound::operator=(const statement_compound &arg)
{
    if (this != &arg)
    {
        statement::operator=(arg);
        content = arg.content;
    }
    return *this;
}


void
statement_compound::traversal(void)
    const
{
    traversal_begin();
    for (size_t j = 0; j < content.size(); ++j)
    {
        if (j)
            traversal_semicolon();
        content[j]->traversal();
    }
    traversal_end();
}


void
statement_compound::traversal_begin(void)
    const
{
    // Do nothing.
}


void
statement_compound::traversal_semicolon(void)
    const
{
    // Do nothing.
}


void
statement_compound::traversal_end(void)
    const
{
    // Do nothing.
}


bool
statement_compound::execution_flows_out(void)
    const
{
    return content.execution_flows_out();
}


bool
statement_compound::empty(void)
    const
{
    if (content.empty())
        return true;
    for (size_t j = 0; j < content.size(); ++j)
        if (!content[j]->empty())
            return false;
    return true;
}


bool
statement_compound::is_compound(void)
    const
{
    return true;
}


bool
statement_compound::execution_flows_in(void)
    const
{
    return content.execution_flows_in();
}


// vim: set ts=8 sw=4 et :
