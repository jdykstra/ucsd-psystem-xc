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


interval
interval::difference(const interval &left, const interval &right)
{
    // assert(left.valid());
    // assert(right.valid());
    interval result;
    size_t left_pos = 0;
    size_t right_pos = 0;
    int count = 0;
    for (;;)
    {
        int old_count = count;
        data_t place;
        if (left_pos < left.length)
        {
            if (right_pos < right.length)
            {
                long_data_t left_val = promote(left.data[left_pos], left_pos);
                long_data_t right_val =
                    promote(right.data[right_pos], right_pos);
                if (left_val < right_val)
                {
                    count += (left_pos & 1 ? -1 : 1);
                    place = left.data[left_pos++];
                }
                else
                {
                    count -= (right_pos & 1 ? -1 : 1);
                    place = right.data[right_pos++];
                }
            }
            else
            {
                count += (left_pos & 1 ? -1 : 1);
                place = left.data[left_pos++];
            }
        }
        else
        {
            if (right_pos < right.length)
            {
                count -= (right_pos & 1 ? -1 : 1);
                place = right.data[right_pos++];
            }
            else
                break;
        }
        if ((count >= 1) != (old_count >= 1))
            result.append(place);
    }
    if (result.length)
        result.data[result.length] = result.length;
    // assert(result.valid());
    return result;
}


// vim: set ts=8 sw=4 et :
