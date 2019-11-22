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

#include <ucsdpsys_opcodes/bitflip.h>


int bitflip;


unsigned
two_byte_decode(const unsigned char *data)
{
    if (bitflip)
    {
        // big-endian
        return (data[1] + (data[0] << 8));
    }
    else
    {
        // little-endian
        return (data[0] + (data[1] << 8));
    }
}


void
two_byte_encode(unsigned char *data, unsigned value)
{
    if (bitflip)
    {
        // big-endian
        data[0] = value >> 8;
        data[1] = value;
    }
    else
    {
        // little-endian
        data[0] = value;
        data[1] = value >> 8;
    }
}


// vim: set ts=8 sw=4 et :
