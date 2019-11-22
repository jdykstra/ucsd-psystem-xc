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

#include <lib/rcstring/list.h>


void
rcstring_list::push_front(const rcstring_list &arg)
{
    if (nstrings + arg.size() > nstrings_max)
    {
        //
        // always 8 less than a power of 2, which is
        // most efficient for many memory allocators
        //
        size_t new_nstrings_max = nstrings_max;
        for (;;)
        {
            new_nstrings_max = new_nstrings_max * 2 + 8;
            if (nstrings + nstrings <= new_nstrings_max)
                break;
        }
        rcstring *new_string = new rcstring[new_nstrings_max];
        for (size_t j = 0; j < nstrings; ++j)
            new_string[j + arg.size()] = string[j];
        delete [] string;
        string = new_string;
        nstrings_max = new_nstrings_max;
    }
    else
    {
        for (size_t k = nstrings; k > 0; --k)
            string[k + arg.size() - 1] = string[k - 1];
    }
    for (size_t j = 0; j < arg.size(); ++j)
        string[j] = arg[j];
    nstrings += arg.size();
}


// vim: set ts=8 sw=4 et :
