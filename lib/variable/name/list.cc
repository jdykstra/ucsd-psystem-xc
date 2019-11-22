//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2012 Peter Miller
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

#include <lib/variable/name/list.h>


variable_name_list::~variable_name_list()
{
    delete [] list;
}


variable_name_list::variable_name_list() :
    length(0),
    maximum(0),
    list(0)
{
}


variable_name_list::variable_name_list(const variable_name_list &arg) :
    length(0),
    maximum(0),
    list(0)
{
    push_back(arg);
}


variable_name_list &
variable_name_list::operator=(const variable_name_list &arg)
{
    if (this != &arg)
    {
        length = 0;
        push_back(arg);
    }
    return *this;
}


void
variable_name_list::push_back(const variable_name &arg)
{
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 8;
        variable_name *new_list = new variable_name[new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_list[j] = list[j];
        delete [] list;
        list = new_list;
        maximum = new_maximum;
    }
    list[length++] = arg;
}


void
variable_name_list::push_back(const variable_name_list &arg)
{
    for (size_t j = 0; j < arg.size(); ++j)
        push_back(arg[j]);
}


// vim: set ts=8 sw=4 et :
