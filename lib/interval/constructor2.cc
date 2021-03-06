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

#include <lib/interval.h>


interval::interval(data_t first, data_t last)
{
    length = 2;
    size = 8;
    data = new data_t[size + 1];
    scan_index = 0;
    scan_next_datum = 0;
    if (first <= promote(last, 1))
    {
        data[0] = first;
        data[1] = last;
    }
    else
    {
        data[0] = last;
        data[1] = first;
    }
    data[2] = 2;
    // assert(valid());
}


// vim: set ts=8 sw=4 et :
