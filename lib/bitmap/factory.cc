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

#include <lib/ac/stdio.h>
#include <lib/ac/string.h>
#include <fcntl.h>
#include <libexplain/close.h>
#include <libexplain/open.h>
#include <libexplain/output.h>
#include <libexplain/read.h>

#include <lib/bitmap/png.h>
#include <lib/bitmap/raw.h>


static bool
looks_like_text(const rcstring &filename)
{
    int fd = explain_open_or_die(filename.c_str(), O_RDONLY, 0666);
    char data[512];
    ssize_t n = explain_read_or_die(fd, data, sizeof(data));
    explain_close_or_die(fd);
    if (n == 0)
        return true;

    // Binary files have NUL bytes.
    // Ascii text files do not have NUL bytes.
    // International text files do not have NUL bytes.
    return (0 == memchr(data, 0, n));
}


bitmap::pointer
bitmap::factory(const rcstring &filename)
{
    rcstring lc_fn = filename.downcase();
    if (lc_fn.ends_with(".foto"))
        return bitmap_raw::create(filename);
    if (lc_fn.ends_with(".png"))
        return bitmap_png::create(filename);

    if (looks_like_text(filename))
    {
        explain_output_error_and_die
        (
            "the %s file looks like it is text, but need a binary image; "
                "aborting",
            filename.quote_c().c_str()
        );
    }

    // this is the default.  it sucks.
    return bitmap_raw::create(filename);
}


// vim: set ts=8 sw=4 et :
