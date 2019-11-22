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

#include <lib/tcontrol/bool/ignore.h>


tcontrol_bool_ignore::~tcontrol_bool_ignore()
{
}


tcontrol_bool_ignore::tcontrol_bool_ignore()
{
}


tcontrol_bool_ignore::pointer
tcontrol_bool_ignore::create()
{
    return pointer(new tcontrol_bool_ignore());
}


void
tcontrol_bool_ignore::set(const rcstring_list &args)
{
    check_value(args, true);
}


// vim: set ts=8 sw=4 et :
