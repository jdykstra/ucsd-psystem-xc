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
#include <lib/ac/string.h>

#include <lib/statement.h>
#include <lib/statement/list.h>


statement_list::~statement_list()
{
    delete [] item;
}


statement_list::statement_list() :
    length(0),
    maximum(0),
    item(0)
{
}


statement_list::statement_list(const statement_list &arg) :
    length(0),
    maximum(0),
    item(0)
{
    for (size_t j = 0; j < arg.length; ++j)
        push_back(arg.item[j]);
}


statement_list &
statement_list::operator=(const statement_list &arg)
{
    if (this != &arg)
    {
        length = 0;
        for (size_t j = 0; j < arg.length; ++j)
            push_back(arg.item[j]);
    }
    return *this;
}


void
statement_list::push_back(const statement::pointer &ssp)
{
    if (ssp->empty())
        return;
    assert(!!ssp);
    if (length >= maximum)
    {
        size_t new_maximum = 2 * maximum + 16;
        statement::pointer *new_item = new statement::pointer [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_item[j] = item[j];
        delete [] item;
        item = new_item;
        maximum = new_maximum;
    }
    item[length++] = ssp;
}


void
statement_list::traversal(void)
    const
{
    size_t len = length;
    while (len > 0 && item[len - 1]->empty())
        --len;

    bool prev_was_empty = true;
    for (size_t j = 0; j < len; ++j)
    {
        statement::pointer sp = item[j];
        bool is_empty = sp->empty();
        if (prev_was_empty && is_empty)
        {
            // Do nothing.
        }
        else
        {
            sp->traversal();
            prev_was_empty = is_empty;
        }
    }
}


void
statement_list::trim(void)
{
    //
    // Get rid of the trailing empty statements first,
    // it makes the next loop much simpler to write.
    //
    while (length > 0)
    {
        statement::pointer sp = item[length - 1];
        if (!sp->empty())
            break;
        --length;
        item[length].reset();
    }

    //
    // Walk the list, tossing redundant blank lines.
    // Leading blanks are considered redundant, too.
    //
    bool prev_was_empty = true;
    size_t k = 0;
    for (size_t j = 0; j < length; ++j)
    {
        statement::pointer &sp = item[j];
        if (prev_was_empty && sp->empty())
            sp.reset();
        else
        {
            item[k++] = sp;
            prev_was_empty = sp->empty();
        }
    }
    length = k;
}


location
statement_list::get_location(void)
    const
{
    location result;
    for (size_t j = 0; j < length; ++j)
        result += item[j]->get_location();
    return result;
}


bool
statement_list::execution_flows_out(void)
    const
{
    for (size_t j = 0; j < length; ++j)
        if (!item[j]->execution_flows_out())
            return false;
    return true;
}


bool
statement_list::execution_flows_in(void)
    const
{
    for (size_t j = 0; j < length; ++j)
    {
        statement::pointer sp = item[j];
        if (!sp->empty())
            return sp->execution_flows_in();
    }
    return false;
}


statement::pointer
statement_list::get(size_t n)
    const
{
    return (n < length ? item[n] : statement::pointer());
}


// vim: set ts=8 sw=4 et :
