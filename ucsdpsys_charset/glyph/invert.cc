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
glyph::invert(void)
    const
{
    return invert(number);
}


glyph::pointer
glyph::invert(unsigned char c)
    const
{
    glyph::pointer gp = create(c, width, height);
    for (unsigned y = 0; y < height; ++y)
    {
        for (unsigned x = 0; x < width; ++x)
        {
            bool b = get_pixel(x, y);
            gp->set_pixel(x, y, !b);
        }
    }
    gp->set_location(location);
    return gp;
}


// vim: set ts=8 sw=4 et :
