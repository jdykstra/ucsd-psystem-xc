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

#include <lib/name_type/list.h>


name_type_list::~name_type_list()
{
    delete [] list;
}


name_type_list::name_type_list() :
    length(0),
    maximum(0),
    list(0)
{
}


name_type_list::name_type_list(const name_type_list &rhs) :
    length(0),
    maximum(0),
    list(0)
{
    for (size_t j = 0; j < rhs.length; ++j)
        push_back(rhs.list[j]);
}


name_type_list &
name_type_list::operator=(const name_type_list &rhs)
{
    if (this != &rhs)
    {
        length = 0;
        for (size_t j = 0; j < rhs.length; ++j)
            push_back(rhs.list[j]);
    }
    return *this;
}


void
name_type_list::push_back(const name_type &arg)
{
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 8;
        name_type *new_list = new name_type [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_list[j] = list[j];
        delete [] list;
        list = new_list;
        maximum = new_maximum;
    }
    list[length++] = arg;
}


void
name_type_list::flatten_into(name_type_list &result)
    const
{
    for (size_t j = 0; j < length; ++j)
    {
        const name_type &nt = list[j];
        for (size_t k = 0; k < nt.size(); ++k)
        {
            const variable_name &vn = nt.get_name(k);
            variable_name_list tmp;
            tmp.push_back(vn);
            result.push_back(name_type(tmp, nt.get_type()));
        }
    }
}


// vim: set ts=8 sw=4 et :
