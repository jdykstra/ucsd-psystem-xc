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
interval::append(data_t datum)
{
    //
    // should always be increasing
    //
    // assert(length < 1 || promote(datum, length) >=
    //  promote(data[length - 1], length - 1));

    //
    // make it larger if necessary
    //
    if (length >= size)
    {
        size = size * 2 + 8;
        data_t *tmp = new data_t[size + 1];
        if (data)
        {
            for (size_t k = 0; k < length; ++k)
                tmp[k] = data[k];
            delete [] data;
        }
        data = tmp;
    }

    //
    // remeber the datum
    //
    data[length++] = datum;

    //
    // elide empty sequences
    //
    // See the comment for the "data" instance variable; it is a
    // series of [lo, hi) pairs.
    //
    // There are two cases here
    //   length is odd:   [a, b) [b, ???)  -->   [a, ???)
    //   length is even:  [a, a)           -->   {}
    // Either way, discard the last two elements.
    //
    if (length >= 2 && data[length - 1] == data[length - 2])
        length -= 2;
}


// vim: set ts=8 sw=4 et :
