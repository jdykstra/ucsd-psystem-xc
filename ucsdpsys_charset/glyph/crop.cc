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

#include <ucsdpsys_charset/glyph.h>


glyph::pointer
glyph::crop(unsigned new_width, unsigned new_height)
    const
{
    return crop(0, 0, new_width, new_height);
}


glyph::pointer
glyph::crop(unsigned ox, unsigned oy, unsigned new_width, unsigned new_height)
    const
{
    glyph::pointer gp = create(get_number(), new_width, new_height);
    for (unsigned y = 0; y < new_height; ++y)
    {
        for (unsigned x = 0; x < new_width; ++x)
        {
            bool b = get_pixel(ox + x, oy + y);
            gp->set_pixel(x, y, b);
        }
    }
    gp->set_location(location);
    return gp;
}


// vim: set ts=8 sw=4 et :
