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

#include <lib/ac/string.h>

#include <ucsdpsys_charset/glyph.h>
#include <ucsdpsys_charset/pretty_char_name.h>


glyph::~glyph()
{
    delete [] data;
    data = 0;
}


glyph::glyph(
    unsigned char a_number,
    unsigned a_width,
    unsigned a_height,
    const unsigned char *a_data
) :
    number(a_number),
    width(a_width),
    row_bytes((a_width + 7) / 8),
    height(a_height),
    data(0)
{
    assert(width > 0);
    assert(row_bytes > 0);
    assert(height > 0);
    size_t nbytes = row_bytes * height;
    data = new unsigned char [nbytes];
    memcpy(data, a_data, nbytes);
}


glyph::pointer
glyph::create(unsigned char a_number, unsigned a_width, unsigned a_height,
    const unsigned char *a_data)
{
    return pointer(new glyph(a_number, a_width, a_height, a_data));
}


glyph::glyph(
    unsigned char a_number,
    unsigned a_width,
    unsigned a_height
) :
    number(a_number),
    width(a_width),
    row_bytes((a_width + 7) / 8),
    height(a_height),
    data(0)
{
    assert(width > 0);
    assert(row_bytes > 0);
    assert(height > 0);
    size_t nbytes = row_bytes * height;
    data = new unsigned char [nbytes];
    memset(data, 0, nbytes);
}


glyph::pointer
glyph::create(unsigned char a_number, unsigned a_width, unsigned a_height)
{
    return pointer(new glyph(a_number, a_width, a_height));
}


void
glyph::set_pixel(unsigned x, unsigned y, bool on_off)
{
    if (on_off)
        set_pixel(x, y);
    else
        clear_pixel(x, y);
}


void
glyph::set_pixel(unsigned x, unsigned y)
{
    if (y >= height)
    {
        unsigned new_height = y + 1;
        unsigned char *new_data = new unsigned char [new_height * row_bytes];
        memcpy(new_data, data, height * row_bytes);
        memset(new_data + y * row_bytes, 0, (new_height - height) * row_bytes);
        delete [] data;
        data = new_data;
        height = new_height;
    }
    if (x >= width)
    {
        if ((x >> 3) <= row_bytes)
            width = x + 1;
        else
        {
            unsigned new_width = x;
            unsigned new_row_bytes = (x + 7) >> 3;
            unsigned char *new_data =
                new unsigned char [height * new_row_bytes];
            for (unsigned row = 0; row < height; ++row)
            {
                memcpy
                (
                    new_data + row * new_row_bytes,
                    data + row * row_bytes,
                    row_bytes
                );
                memset
                (
                    new_data + row * new_row_bytes,
                    0,
                    new_row_bytes
                );
            }
            delete [] data;
            data = new_data;
            width = new_width;
            row_bytes = new_row_bytes;
        }
    }

    // set the pixel
    data[y * row_bytes + (x >> 3)] |= 1 << (x & 7);
}


void
glyph::clear_pixel(unsigned x, unsigned y)
{
    if (x >= width || y >= height)
        return;
    data[y * row_bytes + (x >> 3)] &= ~(1 << (x & 7));
}


rcstring
glyph::get_number_pretty(void)
    const
{
    return pretty_char_name(number);
}


rcstring
glyph::get_location(void)
    const
{
    return location;
}


void
glyph::set_location(const rcstring &locn)
{
    if (locn.empty())
    {
        location = locn;
    }
    else
    {
        rcstring text(locn);
        if (text.ends_with(":"))
            text +=  " ";
        if (!text.ends_with(": "))
            text += ": ";
        location = text;
    }
}


bool
glyph::get_pixel(unsigned x, unsigned y)
    const
{
    if (x >= width)
        return false;
    if (y >= height)
        return false;
    return (0 != (data[y * row_bytes + (x >> 3)] & (1 << (x & 7))));
}


bool
glyph::all_zero(void)
    const
{
    const unsigned char *p = data;
    size_t nbytes = row_bytes * height;
    while (nbytes-- > 0)
        if (*p++)
            return false;
    return true;
}


// vim: set ts=8 sw=4 et :
