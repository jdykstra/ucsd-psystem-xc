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

#include <lib/type/list.h>


type_list::~type_list()
{
}


type_list::type_list()
{
}


type_list::type_list(const type_list &arg) :
    content(arg.content)
{
}


type_list &
type_list::operator=(const type_list &arg)
{
    if (this != &arg)
    {
        content = arg.content;
    }
    return *this;
}


void
type_list::push_back(const type::pointer &tp)
{
    assert(tp);
    content.push_back(tp);
}


// vim: set ts=8 sw=4 et :
