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

#include <lib/config.h>
#include <libexplain/fclose.h>
#include <libexplain/output.h>

#include <ucsdpsys_depends/context.h>


context::~context()
{
    if (ifp)
    {
        explain_fclose_or_die(ifp);
        ifp = 0;
    }
}


context::context(const rcstring &a_ifn, FILE *a_ifp) :
    ifn(a_ifn),
    ifp(a_ifp),
    line_number(1)
{
}


context::pointer
context::create(const rcstring &a_ifn, FILE *a_ifp)
{
    return pointer(new context(a_ifn, a_ifp));
}


void
context::fatal_error(const char *str)
{
    explain_output_error_and_die
    (
        "%s: %d: %s",
        ifn.c_str(),
        line_number,
        str
    );
}


location
context::get_location(void)
    const
{
    return location(ifn, line_number);
}


void
context::ungetch(int c)
{
    switch (c)
    {
    case EOF:
        break;

    case '\n':
        --line_number;
        // fall through...

    default:
        ungetc(c, ifp);
        break;
    }
}


// vim: set ts=8 sw=4 et :
