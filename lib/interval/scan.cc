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


void
interval::scan_begin()
{
    // assert(valid());
    // assert(!scan_index);
    scan_index = 1;
    if (length)
        scan_next_datum = data[0];
    else
        scan_next_datum = 0;
}


bool
interval::scan_next(data_t &datum)
{
    // assert(valid());
    // assert(scan_index & 1);
    if (scan_index >= length)
        return false;
    if (scan_next_datum >= promote(data[scan_index], scan_index))
    {
        scan_index += 2;
        if (scan_index >= length)
            return false;
        scan_next_datum = data[scan_index - 1];
    }
    datum = scan_next_datum++;
    return true;
}


void
interval::scan_end()
{
    // assert(valid());
    // assert(scan_index & 1);
    scan_index = 0;
    scan_next_datum = 0;
}


// vim: set ts=8 sw=4 et :
