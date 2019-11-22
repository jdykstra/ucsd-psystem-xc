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

#include <lib/bitmap/png.h>

#include <ucsdpsys_charset/sink/add_boot_logo.h>


sink_add_boot_logo::~sink_add_boot_logo()
{
}


sink_add_boot_logo::sink_add_boot_logo(
    const sink::pointer &a_deeper,
    const rcstring &a_png_file_name
) :
    deeper(a_deeper),
    png_file_name(a_png_file_name)
{
}


sink_add_boot_logo::pointer
sink_add_boot_logo::create(const sink::pointer &a_deeper,
    const rcstring &a_png_file_name)
{
    return pointer(new sink_add_boot_logo(a_deeper, a_png_file_name));
}


bool
sink_add_boot_logo::is_binary(void)
    const
{
    return deeper->is_binary();
}


void
sink_add_boot_logo::write_one_glyph(const glyph::pointer &)
{
    assert(!"should not be reachable");
}


bool
sink_add_boot_logo::put_boot_logo(const unsigned char *data)
{
    if (deeper->put_boot_logo(data))
        return true;

    //
    // write out the .png file
    // (we only support one image file format at present)
    //
    bitmap::pointer bmp = bitmap_png::create(png_file_name);
    bmp->write(64, 64, data);

    //
    // report success
    //
    return true;
}


// vim: set ts=8 sw=4 et :
