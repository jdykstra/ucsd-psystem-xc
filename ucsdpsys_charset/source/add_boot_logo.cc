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
#include <libexplain/output.h>

#include <lib/bitmap/png.h>

#include <ucsdpsys_charset/source/add_boot_logo.h>


source_add_boot_logo::~source_add_boot_logo()
{
}


source_add_boot_logo::source_add_boot_logo(
    const source::pointer &a_deeper,
    const rcstring &a_png_file_name
) :
    deeper(a_deeper),
    png_file_name(a_png_file_name)
{
}


source_add_boot_logo::pointer
source_add_boot_logo::create(const source::pointer &a_deeper,
    const rcstring &a_png_file_name)
{
    return pointer(new source_add_boot_logo(a_deeper, a_png_file_name));
}


bool
source_add_boot_logo::is_binary(void)
    const
{
    return deeper->is_binary();
}


font::pointer
source_add_boot_logo::read_whole_font(void)
{
    return deeper->read_whole_font();
}


glyph::pointer
source_add_boot_logo::read_one_glyph(void)
{
    assert(!"should not be able to reach here");
    return glyph::pointer();
}


bool
source_add_boot_logo::get_boot_logo(unsigned char *data)
{
    //
    // If the deeper source *has* a boot image,
    // use that image, and ignore the PNG file.
    //
    if (deeper->get_boot_logo(data))
        return true;

    //
    // load the PNG file
    //
    bitmap::pointer bmp = bitmap_png::create(png_file_name);
    unsigned width = 0;
    unsigned height = 0;
    unsigned char *d = 0;
    bmp->read(width, height, d);

    if (width != 64 || height != 64)
    {
        explain_output_error_and_die
        (
            "%s: the image --size=%dx%d is incorrect for a UCSD Pascal system "
                "boot logo (should be --size=64x64)",
            png_file_name.c_str(),
            width,
            height
        );
    }

    // copy the data across
    unsigned row_bytes = (width + 7) / 8;
    size_t nbytes = row_bytes * height;
    memcpy(data, d, nbytes);
    delete [] d;

    //
    // report success
    //
    return true;
}


// vim: set ts=8 sw=4 et :
