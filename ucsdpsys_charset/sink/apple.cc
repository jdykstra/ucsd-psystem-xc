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
#include <lib/ac/sys/stat.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/fputc.h>
#include <libexplain/fseek.h>
#include <libexplain/fstat.h>
#include <libexplain/putc.h>

#include <ucsdpsys_charset/sink/apple.h>


sink_apple::~sink_apple()
{
    explain_fclose_or_die(fp);
    fp = 0;
}


sink_apple::sink_apple(FILE *a_fp) :
    fp(a_fp)
{
    assert(fp);
}


bool
sink_apple::candidate(const rcstring &filename)
{
    return (filename.ends_with_nc(".charset"));
}


sink_apple::pointer
sink_apple::create(const rcstring &filename)
{
    FILE *fp =
        (
            looks_like_a_stdout_synonym(filename)
        ?
            stdout
        :
            explain_fopen_or_die(filename.c_str(), "wb")
        );
    return pointer(new sink_apple(fp));
}


static size_t
filesize(FILE *fp)
{
    int fd = fileno(fp);
    struct stat st;
    explain_fstat_or_die(fd, &st);
    ssize_t big = ((size_t)1 << 15);
    return (st.st_size < big ? st.st_size : big);
}


void
sink_apple::write_whole_font_begin(void)
{
    rewind(fp);
    if (filesize(fp) < 1024)
    {
        // establish the file length.
        // all the skipped bytes will be 0x00.
        explain_fseek_or_die(fp, 1023, SEEK_SET);
        explain_fputc_or_die(0x00, fp);
        rewind(fp);
    }
}


void
sink_apple::write_one_glyph(const glyph::pointer &gp0)
{
    unsigned c = gp0->get_number();

    // no support for high-bit-on characters
    // silently ignore these ones.
    if (c >= 128)
        return;

    enum { apple_width = 8, apple_height = 8 };
    glyph::pointer gp = gp0->crop(apple_width, apple_height);

    // data stored on disk bottom-to-top
    gp = gp->vflip();

    explain_fseek_or_die(fp, c * apple_height, SEEK_SET);
    for (unsigned y = 0; y < apple_height; ++y)
    {
        const unsigned char *row = gp->get_row(y);
        explain_putc_or_die(*row, fp);
    }
}


void
sink_apple::write_whole_font_end(void)
{
    explain_fflush_or_die(fp);
}


// vim: set ts=8 sw=4 et :
