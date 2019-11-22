//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#include <lib/ac/ctype.h>

#include <lib/tcontrol.h>


tcontrol::~tcontrol()
{
}


tcontrol::tcontrol()
{
}


tcontrol::tcontrol(const tcontrol &)
{
}


tcontrol &
tcontrol::operator=(const tcontrol &)
{
    return *this;
}


void
tcontrol::set(const rcstring &arg)
{
    if (isalpha((unsigned char)arg[0]) && ispunct((unsigned char)arg[1]))
    {
        // Cope with (*$U-*) and friends.
        rcstring_list args;
        args.push_back(arg.substring(0, 1));
        args.push_back(arg.substring(1, arg.size() - 1));
        set(args);
    }
    else
    {
        rcstring_list args;
        args.split(arg);
        set(args);
    }
}


// vim: set ts=8 sw=4 et :
