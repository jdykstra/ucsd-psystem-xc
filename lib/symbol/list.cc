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

#include <lib/symbol/list.h>


symbol_list::~symbol_list()
{
    delete [] item;
}


symbol_list::symbol_list() :
    length(0),
    maximum(0),
    item(0)
{
}


void
symbol_list::push_back(const symbol::pointer &arg)
{
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 16;
        symbol::pointer *new_item = new symbol::pointer [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_item[j] = item[j];
        delete [] item;
        item = new_item;
        maximum = new_maximum;
    }
    item[length++] = arg;
}


symbol::pointer
symbol_list::get(int n)
    const
{
    if (n < 0 || (size_t)n >= length)
        return symbol::pointer();
    return item[n];
}


// vim: set ts=8 sw=4 et :
