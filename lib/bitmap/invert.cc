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

#include <lib/bitmap/invert.h>


bitmap_invert::~bitmap_invert()
{
}


bitmap_invert::bitmap_invert(const pointer &a_deeper) :
    deeper(a_deeper)
{
    assert(deeper);
}


bitmap_invert::pointer
bitmap_invert::create(const pointer &a_deeper)
{
    return pointer(new bitmap_invert(a_deeper));
}


void
bitmap_invert::read(unsigned &width, unsigned &height, unsigned char *&data)
{
    deeper->read(width, height, data);

    // invert all the bits
    size_t row_bytes = (width + 7) / 8;
    size_t nbytes = row_bytes * height;
    unsigned char *p = data;
    while (nbytes-- > 0)
        *p++ ^= 0xFF;
}


void
bitmap_invert::write(unsigned width, unsigned height, const unsigned char *data)
{
    // We can't change data, so we have to work from a copy.
    size_t row_bytes = (width + 7) / 8;
    size_t data_size = row_bytes * height;

    unsigned char *inverted_data = new unsigned char [data_size];

    {
        const unsigned char *from = data;
        unsigned char *to = inverted_data;
        size_t nbytes = data_size;
        while (nbytes-- > 0)
            *to++ = ~*from++;
    }

    deeper->write(width, height, inverted_data);
    delete [] inverted_data;
}


void
bitmap_invert::set_size_hint(unsigned width, unsigned height)
{
    deeper->set_size_hint(width, height);
}


// vim: set ts=8 sw=4 et :
