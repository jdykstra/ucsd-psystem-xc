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


static rcstring
to_string(interval::data_t x)
{
    int width = 4;
    if (x >= 0x10000)
        width += 2;
    if (x >= 0x1000000)
        width += 2;
    return rcstring::printf("0x%0*lX", width, (unsigned long)x);
}


rcstring
interval::representation()
    const
{
    rcstring result;
    result += "(";
    for (size_t j = 0; j < length; j += 2)
    {
        if (j)
            result += ", ";
        result += to_string(data[j]);
        if (data[j] + 2 == data[j + 1])
        {
            result += ", ";
            result += to_string(data[j] + 1);
        }
        else if (data[j] + 1 != data[j + 1])
        {
            result += " - ";
            result += to_string(data[j + 1] - 1);
        }
    }
    result += ")";
    return result;
}


// vim: set ts=8 sw=4 et :
