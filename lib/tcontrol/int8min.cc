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

#include <lib/ac/stdlib.h>

#include <lib/cardinal.h>
#include <lib/pascal/lex.h>
#include <lib/tcontrol/int8min.h>


tcontrol_int8min::~tcontrol_int8min()
{
}


tcontrol_int8min::tcontrol_int8min(int &a_cntrl) :
    cntrl(a_cntrl)
{
}


tcontrol_int8min::pointer
tcontrol_int8min::create(int &a_cntrl)
{
    return pointer(new tcontrol_int8min(a_cntrl));
}


void
tcontrol_int8min::set(const rcstring_list &args)
{
    switch (args.size())
    {
    case 0:
        pascal_grammar_error
        (
            "more parameters required"
        );
        break;

    case 1:
        pascal_lex_error
        (
            pascal_lex_location(),
            "%s: requires one parameter",
            args[0].c_str()
        );
        cntrl = 32767;
        break;

    case 2:
        {
            long n = strtol(args[1].c_str(), 0, 0);
            if (n < 8 || n >= 32768)
            {
                pascal_lex_error
                (
                    pascal_lex_location(),
                    "%s must be 8..32767, not %ld",
                    args[0].c_str(),
                    n
                );
            }
            else
                cntrl = n;
        }
        break;

    default:
        pascal_lex_error
        (
            pascal_lex_location(),
            "%s %s needs one parameter, not %s",
            args[0].c_str(),
            args[1].quote_c().c_str(),
            cardinal(args.size() - 1).c_str()
        );
        break;
    }
}


// vim: set ts=8 sw=4 et :
