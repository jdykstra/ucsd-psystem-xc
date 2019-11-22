//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#include <lib/bitrev.h>


uint8_t
bitrev8(uint8_t x)
{
    uint8_t result = 0;
    uint8_t b1 = 1;
    uint8_t b2 = 0x80;
    while (b1)
    {
        if (x & b1)
            result |= b2;
        b1 <<= 1;
        b2 >>= 1;
    }
    return result;
}


uint16_t
bitrev16(uint16_t x)
{
    uint16_t result = 0;
    uint16_t b1 = 1;
    uint16_t b2 = 0x8000;
    while (b1)
    {
        if (x & b1)
            result |= b2;
        b1 <<= 1;
        b2 >>= 1;
    }
    return result;
}


uint32_t
bitrev32(uint32_t x)
{
    uint32_t result = 0;
    uint32_t b1 = 1;
    uint32_t b2 = 0x80000000;
    while (b1)
    {
        if (x & b1)
            result |= b2;
        b1 <<= 1;
        b2 >>= 1;
    }
    return result;
}


// vim: set ts=8 sw=4 et :
