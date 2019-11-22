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

#include <lib/tcontrol/bool/ambiguous.h>


tcontrol_bool_ambiguous::~tcontrol_bool_ambiguous()
{
}


tcontrol_bool_ambiguous::tcontrol_bool_ambiguous(
    const tcontrol::pointer &a_bool_case,
    const tcontrol::pointer &a_non_bool_case
) :
    bool_case(a_bool_case),
    non_bool_case(a_non_bool_case)
{
}


tcontrol_bool_ambiguous::pointer
tcontrol_bool_ambiguous::create(const tcontrol::pointer &a_bool_case,
    const tcontrol::pointer &a_non_bool_case)
{
    return pointer(new tcontrol_bool_ambiguous(a_bool_case, a_non_bool_case));
}


void
tcontrol_bool_ambiguous::set(const rcstring_list &args)
{
    if (args.size() >= 2 && looks_boolean(args[1]))
        bool_case->set(args);
    else
        non_bool_case->set(args);
}


// vim: set ts=8 sw=4 et :
