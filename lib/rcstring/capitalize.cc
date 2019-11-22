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

#include <lib/rcstring/accumulator.h>


rcstring
rcstring::capitalize()
    const
{
    static rcstring_accumulator sa;
    bool prev_was_alpha = false;
    const char *cp = c_str();
    sa.clear();
    for (;;)
    {
        unsigned char c = (unsigned char)*cp++;
        if (!c)
            break;
        if (islower(c))
        {
            if (!prev_was_alpha)
                c = toupper(c);
            prev_was_alpha = true;
        }
        else if (isupper(c))
        {
            if (prev_was_alpha)
                c = tolower(c);
            prev_was_alpha = true;
        }
        else
            prev_was_alpha = false;
        sa.push_back(c);
    }
    return sa.mkstr();
}


// vim: set ts=8 sw=4 et :
