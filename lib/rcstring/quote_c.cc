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

#include <lib/ac/ctype.h>

#include <lib/rcstring.h>
#include <lib/rcstring/accumulator.h>


rcstring
rcstring::quote_c()
    const
{
    static rcstring_accumulator ac;
    ac.clear();
    const char *cp = c_str();
    ac.push_back('"');
    for (;;)
    {
        unsigned char c = *cp++;
        switch (c)
        {
        case 0:
            ac.push_back('"');
            return ac.mkstr();

        case '"':
        case '\\':
            ac.push_back('\\');
            ac.push_back(c);
            break;

        case '\a': ac.push_back('\\'); ac.push_back('a'); break;
        case '\b': ac.push_back('\\'); ac.push_back('b'); break;
        case '\f': ac.push_back('\\'); ac.push_back('f'); break;
        case '\n': ac.push_back('\\'); ac.push_back('n'); break;
        case '\r': ac.push_back('\\'); ac.push_back('r'); break;
        case '\t': ac.push_back('\\'); ac.push_back('t'); break;
        case '\v': ac.push_back('\\'); ac.push_back('v'); break;

        default:
            if (isprint(c))
                ac.push_back(c);
            else
                ac.printf("\\%03o", c);
            break;
        }
    }
}


// vim: set ts=8 sw=4 et :
