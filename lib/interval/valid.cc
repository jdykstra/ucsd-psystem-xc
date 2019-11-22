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
interval::valid()
    const
{
    if (length > size)
        return false;
    if (length & 1)
        return false;
    if ((size == 0) != (data == 0))
        return false;
    if (length == 0)
        return true;
    if (data[length] != length)
        return false;

    //
    // As a special case, an upper bound of zero means
    // positive infinity.  It has to be the last one.
    //
    size_t max = length;
    if (data[max - 1] == 0)
        --max;

    for (size_t j = 1; j < max; ++j)
        if (data[j - 1] >= data[j])
            return false;
    return true;
}


// vim: set ts=8 sw=4 et :
