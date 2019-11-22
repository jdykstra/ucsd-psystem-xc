//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <lib/ac/ctype.h>

#include <lib/rcstring/accumulator.h>


rcstring
rcstring::downcase()
    const
{
    static rcstring_accumulator tmp;
    tmp.clear();
    const char *cp = c_str();
    for (;;)
    {
        unsigned char c = *cp++;
        if (!c)
            break;
        if (isupper((unsigned char)c))
            c = tolower(c);
        tmp.push_back(c);
    }
    return tmp.mkstr();
}


// vim: set ts=8 sw=4 et :
