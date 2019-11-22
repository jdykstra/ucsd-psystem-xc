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

#include <lib/cardinal.h>
#include <lib/pascal/lex.h>
#include <lib/tcontrol/noarg.h>


tcontrol_noarg::~tcontrol_noarg()
{
}


tcontrol_noarg::tcontrol_noarg()
{
}


tcontrol_noarg::tcontrol_noarg(const tcontrol_noarg &arg) :
    tcontrol(arg)
{
}


tcontrol_noarg &
tcontrol_noarg::operator=(const tcontrol_noarg &arg)
{
    if (this != &arg)
    {
        tcontrol::operator=(arg);
    }
    return *this;
}


bool
tcontrol_noarg::check_value(const rcstring_list &args)
{
    switch (args.size())
    {
    case 0:
        pascal_grammar_error
        (
            "no idea what you are trying to say"
        );
        return false;

    case 1:
        return true;

    default:
        pascal_lex_error
        (
            pascal_lex_location(),
            "setting the %s control requires zero parameters, not %s",
            args[0].quote_c().c_str(),
            cardinal(args.size() - 1).c_str()
        );
        return false;
    }
}


// vim: set ts=8 sw=4 et :
