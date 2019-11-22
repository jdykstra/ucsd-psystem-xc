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

#include <lib/ac/math.h>
#include <fcntl.h>
#include <libexplain/close.h>
#include <libexplain/creat.h>
#include <libexplain/fstat.h>
#include <libexplain/open.h>
#include <libexplain/output.h>
#include <libexplain/read.h>
#include <libexplain/write.h>

#include <lib/bitmap/raw.h>


bitmap_raw::~bitmap_raw()
{
}


bitmap_raw::bitmap_raw(const rcstring &a_filename) :
    filename(a_filename),
    hint_width(0),
    hint_height(0)
{
}


bitmap_raw::pointer
bitmap_raw::create(const rcstring &a_filename)
{
    return pointer(new bitmap_raw(a_filename));
}


void
bitmap_raw::read(unsigned &width, unsigned &height, unsigned char *&data)
{
    int ifd = explain_open_or_die(filename.c_str(), O_RDONLY, 0);
    struct stat st;
    explain_fstat_or_die(ifd, &st);
    bool size_warning = false;
    if (st.st_size >= 65536)
    {
        explain_output_error_and_die
        (
            "%s: there is no way a file of size %ld can fit into the "
                "memory of a UCSD Pascal system; aborting",
            filename.c_str(),
            (long)st.st_size
        );
    }
    if (st.st_size & 511)
    {
        explain_output_warning
        (
            "%s: UCSD Pascal bitmap image files are usually a multiple of 512 "
                "bytes in size, but this file is %ld bytes (r=%d), so "
                "it may not be a bitmap image",
            filename.c_str(),
            (long)st.st_size,
            (int)(st.st_size & 511)
        );
        size_warning = true;
    }

    //
    // check the image size
    //
    if (hint_width)
    {
        width = hint_width;
        height = hint_height;
        unsigned row_bytes = (width + 7) / 8;
        off_t expect = row_bytes * height;
        if (st.st_size < expect)
        {
            explain_output_error_and_die
            (
                "%s: an image of --size=%dx%d requires a bitmap file of at "
                   "least %ld bytes, but this file is only %ld bytes; aborting",
                filename.c_str(),
                width,
                height,
                expect,
                (long)st.st_size
            );
        }
        expect = (expect + 511) >> 9;
        expect <<= 9;
        if (st.st_size != expect && !size_warning)
        {
            explain_output_warning
            (
                "%s: an image of --size=%dx%d requires a file size of %d "
                    "whole 512-byte blocks, giving a size of %ld bytes, "
                    "but this file is %ld bytes",
                filename.c_str(),
                width,
                height,
                (int)(expect >> 9),
                expect,
                (long)st.st_size
            );
            size_warning = true;
        }
    }
    else
    {
        // the size if it was a 4:3 rectangle
        int z = ~1 & (int)sqrt(st.st_size * (8./12.));
        int r_width = z * 4;
        int r_height = z * 3;
        int r_error = st.st_size - r_width * r_height / 8;

        // the size if it was a square
        z = ~7 & (int)sqrt(st.st_size * 8.);
        int sq_width = z;
        int sq_height = z;
        int sq_error = st.st_size - sq_width * sq_height / 8;

        if (r_error < sq_error)
        {
            width = r_width;
            height = r_height;
        }
        else
        {
            width = sq_width;
            height = sq_height;
        }
        explain_output_error
        (
            "%s: guessing --size=%dx%d for this image",
            filename.c_str(),
            width,
            height
        );
    }

    //
    // read in the .foto data
    //
    data = new unsigned char [st.st_size];
    explain_read_or_die(ifd, data, st.st_size);
    explain_close_or_die(ifd);
}


void
bitmap_raw::write(unsigned width, unsigned height, const unsigned char *data)
{
    int fd = explain_creat_or_die(filename.c_str(), 0666);
    unsigned row_bytes = (width + 7) / 8;
    unsigned data_size = row_bytes * height;
    explain_write_or_die(fd, data, data_size);
    explain_close_or_die(fd);
}


void
bitmap_raw::set_size_hint(unsigned width, unsigned height)
{
    assert(width > 0);
    hint_width = width;
    assert(height > 0);
    hint_height = height;
}


// vim: set ts=8 sw=4 et :
