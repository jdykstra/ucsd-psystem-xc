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

#include <lib/variable/name.h>
#include <lib/pascal/lex.h>


variable_name::~variable_name()
{
}


variable_name::variable_name()
{
}


variable_name::variable_name(const rcstring &a_name) :
    name(a_name),
    locn(pascal_lex_location())
{
}


variable_name::variable_name(const rcstring &a_name, const location &a_locn) :
    name(a_name),
    locn(a_locn)
{
}


variable_name::variable_name(const variable_name &arg) :
    name(arg.name),
    locn(arg.locn)
{
}


variable_name &
variable_name::operator=(const variable_name &arg)
{
    if (this != &arg)
    {
        name = arg.name;
        locn = arg.locn;
    }
    return *this;
}


// vim: set ts=8 sw=4 et :
