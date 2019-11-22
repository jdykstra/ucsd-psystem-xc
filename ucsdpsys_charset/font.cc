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

#include <lib/ac/assert.h>
#include <lib/ac/string.h>

#include <ucsdpsys_charset/font.h>


font::~font()
{
}


font::font(unsigned a_width, unsigned a_height) :
    width(a_width),
    height(a_height)
{
    assert(width > 0);
    assert(height > 0);
}


font::pointer
font::create(unsigned a_width, unsigned a_height)
{
    return pointer(new font(a_width, a_height));
}


glyph::pointer
font::get_glyph(unsigned char c)
    const
{
    glyph::pointer gp = glyphs[c];
    if (!gp)
    {
#if 0
        size_t row_bytes = (width + 7) / 8;
        size_t nbytes = row_bytes * height;
        unsigned char *data = new unsigned char [nbytes];
        memset(data, 0, nbytes);
        gp = glyph::create(c, width, height, data);
        glyphs[c] = gp;
        delete [] data;
#else
        return glyph::pointer();
#endif
    }
    assert(c == gp->get_number());
    return gp;
}


glyph::pointer
font::create_glyph(unsigned char c)
{
    size_t row_bytes = (width + 7) / 8;
    size_t nbytes = row_bytes * height;
    unsigned char *data = new unsigned char [nbytes];
    memset(data, 0, nbytes);
    glyph::pointer gp = glyph::create(c, width, height, data);
    delete [] data;

    glyphs[c] = gp;
    assert(c == gp->get_number());
    return gp;
}


void
font::set_glyph(const glyph::pointer &gp)
{
    unsigned char c = gp->get_number();
    glyphs[c] = gp;
    if (width < gp->get_width())
        width = gp->get_width();
    if (height < gp->get_height())
        height = gp->get_height();
}


void
font::clear_glyph(unsigned char c)
{
    glyphs[c].reset();

#if 0
    width = 1;
    height = 1;
    for (unsigned n = 0; n < 256; ++n)
    {
        glyph::pointer gp = glyphs[n];
        if (gp)
        {
            if (width < gp->get_width())
                width = gp->get_width();
            if (height < gp->get_height())
                height = gp->get_height();
        }
    }
#endif
}


void
font::read_whole_font_begin(void)
{
}


void
font::read_whole_font_end(void)
{
}


// vim: set ts=8 sw=4 et :
