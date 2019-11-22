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

#include <lib/config.h>
#include <png.h>
#include <libexplain/fclose.h>
#include <libexplain/fopen.h>
#include <libexplain/output.h>

#include <lib/bitmap/png.h>
#include <lib/bitrev.h>


bitmap_png::~bitmap_png()
{
}


bitmap_png::bitmap_png(const rcstring &a_filename) :
    filename(a_filename)
{
}


bitmap_png::pointer
bitmap_png::create(const rcstring &a_filename)
{
    return pointer(new bitmap_png(a_filename));
}


static rcstring
string_from_color_type(int x)
{
    switch (x)
    {
    case PNG_COLOR_TYPE_GRAY:
        return "PNG_COLOR_TYPE_GRAY";

    case PNG_COLOR_TYPE_PALETTE:
        return "PNG_COLOR_TYPE_PALETTE";

    case PNG_COLOR_TYPE_RGB:
        return "PNG_COLOR_TYPE_RGB";

    case PNG_COLOR_TYPE_RGB_ALPHA:
        return "PNG_COLOR_TYPE_RGB_ALPHA";

    case PNG_COLOR_TYPE_GRAY_ALPHA:
        return "PNG_COLOR_TYPE_GRAY_ALPHA";
    }
    return rcstring::printf("%d", x);
}


void
bitmap_png::read(unsigned &width, unsigned &height, unsigned char *&data)
{
    FILE *ifp = explain_fopen_or_die(filename.c_str(), "rb");
    png_structp png_ptr =
        png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png_ptr)
    {
        explain_output_error_and_die
        (
            "%s: png_create_read_struct failed",
            filename.c_str()
        );
    }
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        explain_output_error_and_die
        (
            "%s: png_create_info_struct failed",
            filename.c_str()
        );
    }
    png_init_io(png_ptr, ifp);
    png_read_png(png_ptr, info_ptr, 0, NULL);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    unsigned bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    unsigned color_type = png_get_color_type(png_ptr, info_ptr);
    if
    (
        bit_depth != 1
    ||
        (
            color_type != PNG_COLOR_TYPE_GRAY
        &&
            color_type != PNG_COLOR_TYPE_PALETTE
        )
    )
    {
        explain_output_error_and_die
        (
            "%s: the image must be a 1-bit monochrome image, not %d-bit %s",
            filename.c_str(),
            bit_depth,
            string_from_color_type(color_type).c_str()
        );
    }

    //
    // If the PNG file uses a color palette (we don't, but Gimp does)
    // we have to grope the palette to figure out the invert.
    //
    bool invert = false;
    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_colorp palette[PNG_MAX_PALETTE_LENGTH];
        for (size_t j = 0; j < PNG_MAX_PALETTE_LENGTH; ++j)
            palette[j] = 0;
        int num_palette = PNG_MAX_PALETTE_LENGTH;
        // png_get_PLTE returns non-zero if the data has been read, or
        // zero if it is missing.
        int n = png_get_PLTE(png_ptr, info_ptr, palette, &num_palette);
        if (n == 0)
        {
            explain_output_error_and_die
            (
                "%s: png_get_PLTE failed",
                filename.c_str()
            );
        }
        if (num_palette != 2)
        {
            explain_output_error_and_die
            (
                "%s: weird palette size (expected 2, observed %d)",
                filename.c_str(),
                num_palette
            );
        }

        // FIXME: get fussier about the colors in the palette?
#if 0
        for (int j = 0; j < PNG_MAX_PALETTE_LENGTH; ++j)
        {
            if (palette[j])
            {
                fprintf(stderr, "%s: %d: palette[%d] = %p\n", __FILE__,
                    __LINE__, j, palette[j]);
                fprintf(stderr, "%s: %d: palette[%d][0] = %d,%d,%d\n", __FILE__,
                    __LINE__, j, palette[j][0].red, palette[j][0].green,
                    palette[j][0].blue);
                fprintf(stderr, "%s: %d: palette[%d][1] = %d,%d,%d\n", __FILE__,
                    __LINE__, j, palette[j][1].red, palette[j][1].green,
                    palette[j][1].blue);
            }
        }
#endif
        invert = (palette[0]->red != 0);
    }

    png_byte **row_pointers = png_get_rows(png_ptr, info_ptr);

    const unsigned row_bytes = (width + 7) / 8;
    size_t nbytes = row_bytes * height;
    data = new unsigned char [nbytes];

    // this memset should be redundant
    memset(data, 0, nbytes);

    for (unsigned y = 0; y < height; ++y)
    {
        const png_byte *from = row_pointers[y];
        unsigned char *to = data + y * row_bytes;
        for (unsigned x = 0; x < row_bytes; ++x)
        {
            // The bitrev8 is for the PNG, because in PNG the left-most pixel
            // is stored in the high bit of the byte.  But apple did it the
            // other way around, and I wrote the apple stuff first.
            unsigned char byte = bitrev8(from[x]);
            if (invert)
                byte = ~byte;
            to[x] = byte;
        }
    }
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    explain_fclose_or_die(ifp);
}


void
bitmap_png::write(unsigned width, unsigned height, const unsigned char *data)
{
    FILE *ofp = explain_fopen_or_die(filename.c_str(), "wb");
    png_struct *png_ptr =
        png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    if (!png_ptr)
    {
        explain_output_error_and_die
        (
            "%s: png_create_write_struct failed",
            filename.c_str()
        );
    }
    png_info *info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        explain_output_error_and_die
        (
            "%s: png_create_info_struct failed",
            filename.c_str()
        );
    }
    png_init_io(png_ptr, ofp);
    png_set_compression_level(png_ptr, Z_BEST_COMPRESSION);
    png_set_IHDR
    (
        png_ptr,
        info_ptr,
        width,
        height,
        1,
        PNG_COLOR_TYPE_GRAY,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE,
        PNG_FILTER_TYPE_BASE
    );

    unsigned row_bytes = (width + 7) /8;
    unsigned char *rdat = new unsigned char [row_bytes];
    png_write_info(png_ptr, info_ptr);
    for (unsigned y = 0; y < height; ++y)
    {
        const unsigned char *p = data + y * row_bytes;
        for (unsigned x = 0; x < row_bytes; ++x)
        {
            // The bitrev8 is for the PNG, because in PNG the left-most pixel
            // is stored in the high bit of the byte.  But apple did it the
            // other way around, and I wrote the apple stuff first.
            rdat[x] = bitrev8(p[x]);
        }
        png_write_row(png_ptr, rdat);
    }
    delete [] rdat;
    png_write_end(png_ptr, NULL);
    explain_fclose_or_die(ofp);
}



void
bitmap_png::set_size_hint(unsigned, unsigned)
{
    // PNG files have real meta-data, so we can ignore the size hint.
}


// vim: set ts=8 sw=4 et :
