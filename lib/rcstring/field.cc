//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#include <lib/ac/string.h>
#include <lib/rcstring.h>


rcstring
rcstring::field(char sep, int fldnum)
    const
{
    const char *cp = c_str();
    while (fldnum > 0)
    {
        const char *sp = strchr(cp, sep);
        if (!sp)
            return rcstring("");
        cp = sp + 1;
        --fldnum;
    }
    const char *ep = strchr(cp, sep);
    if (ep)
        return rcstring(cp, ep - cp);
    return rcstring(cp);
}


// vim: set ts=8 sw=4 et :
