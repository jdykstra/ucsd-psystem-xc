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
#include <lib/rcstring/accumulator.h>
#include <lib/rcstring/list.h>


rcstring
rcstring_list::unsplit(const char *sep)
    const
{
    return unsplit(0, size(), sep);
}


rcstring
rcstring_list::unsplit(size_t start, size_t stop, const char *sep)
    const
{
    if (!sep)
        sep = " ";
    size_t seplen = strlen(sep);

    static rcstring_accumulator tmp;
    tmp.clear();
    for (size_t j = start; j <= stop && j < nstrings; j++)
    {
        const rcstring &s = string[j];
        if (!s.empty())
        {
            if (!tmp.empty())
                tmp.push_back(sep, seplen);
            tmp.push_back(s);
        }
    }

    return tmp.mkstr();
}


// vim: set ts=8 sw=4 et :
