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


interval &
interval::operator=(const interval &arg)
{
    if (this != &arg)
    {
        if (data)
        {
            delete [] data;
            data = 0;
        }
        // assert(arg.valid());
        length = arg.length;
        size = length;
        scan_index = 0;
        scan_next_datum = 0;
        if (size)
        {
            data = new data_t[size + 1];
            for (size_t j = 0; j <= length; ++j)
                data[j] = arg.data[j];
        }
        else
            data = 0;
        // assert(valid());
    }
    return *this;
}


// vim: set ts=8 sw=4 et :
