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

#include <lib/dislabel.h>


dislabel::~dislabel()
{
}


dislabel::dislabel(const rcstring &a_prefix) :
    prefix(a_prefix),
    ordering(0)
{
}


static rcstring
string_from_number(unsigned address)
{
    return rcstring::printf("%04X", address);
}


void
dislabel::add(unsigned address)
{
    rcstring key = string_from_number(address);
    if (name_from_address.query(key))
        return;
    int num = ordering++;
    rcstring value = rcstring::printf("%s_%d", prefix.c_str(), num);
    name_from_address.assign(key, value);
}


void
dislabel::add(unsigned address, const rcstring &suffix)
{
    rcstring key = string_from_number(address);
    if (name_from_address.query(key))
        return;
    rcstring value = rcstring::printf("%s_%s", prefix.c_str(), suffix.c_str());
    name_from_address.assign(key, value);
}


rcstring
dislabel::get_name(unsigned address)
    const
{
    rcstring key = string_from_number(address);
    rcstring *value = name_from_address.query(key);
    if (!value)
        return "";
    return *value;
}


// vim: set ts=8 sw=4 et :
