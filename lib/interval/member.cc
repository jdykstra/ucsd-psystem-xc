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


bool
interval::member(data_t datum)
    const
{
    if (length == 0)
        return false;
    // assert(valid());
    long min = 0;
    long max = length - 2;
    while (min <= max)
    {
        long mid = ((min + max) / 2) & ~1;
        data_t lo = data[mid];
        long_data_t hi = promote(data[mid + 1], mid + 1);
        if (lo <= datum && datum < hi)
            return true;
        if (lo < datum)
            min = mid + 2;
        else
            max = mid - 2;
    }
    return false;
}


// vim: set ts=8 sw=4 et :
