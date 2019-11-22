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

#include <lib/type.h>
#include <lib/name_type.h>


name_type::~name_type()
{
}


name_type::name_type()
{
}


name_type::name_type(const variable_name_list &a_name,
        const type::pointer &a_type) :
    name(a_name),
    ptype(a_type)
{
}


name_type::name_type(const name_type &arg) :
    name(arg.name),
    ptype(arg.ptype)
{
}


name_type &
name_type::operator=(const name_type &arg)
{
    if (this != &arg)
    {
        name = arg.name;
        ptype = arg.ptype;
    }
    return *this;
}


// vim: set ts=8 sw=4 et :
