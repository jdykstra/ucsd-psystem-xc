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

#include <lib/ac/sys/stat.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/fputc.h>
#include <libexplain/fseek.h>
#include <libexplain/fstat.h>
#include <libexplain/fwrite.h>

#include <ucsdpsys_charset/sink/terak.h>


sink_terak::~sink_terak()
{
    explain_fclose_or_die(fp);
    fp = 0;
}


sink_terak::sink_terak(FILE *a_fp) :
    fp(a_fp)
{
    assert(fp);
}


bool
sink_terak::candidate(const rcstring &filename)
{
    return (filename.ends_with_nc(".charset"));
}


sink_terak::pointer
sink_terak::create(const rcstring &filename)
{
    FILE *fp =
        (
            looks_like_a_stdout_synonym(filename)
        ?
            stdout
        :
            explain_fopen_or_die(filename.c_str(), "wb")
        );
    return pointer(new sink_terak(fp));
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
sink_terak::write_whole_font_begin(void)
{
    rewind(fp);
    if (filesize(fp) < 2048)
    {
        // establish the file length.
        // all the skipped bytes will be 0x00.
        explain_fseek_or_die(fp, 2047, SEEK_SET);
        explain_fputc_or_die(0x00, fp);
        rewind(fp);
    }
}


void
sink_terak::write_one_glyph(const glyph::pointer &gp0)
{
    glyph::pointer gp = gp0->crop(8, 10);
    unsigned c = gp->get_number();

    unsigned page = c >> 7;
    c &= 0x7F;
    if (c < ' ')
    {
        // no support for control characters
        // simply ignore these ones.
        return;
    }
    off_t pos = page * 0x400 + (c - ' ') * 10;

    assert(gp->get_width() == 8);
    assert(gp->get_height() == 10);
    explain_fseek_or_die(fp, pos, SEEK_SET);

    explain_fwrite_or_die
    (
        gp->get_row(0),
        gp->get_row_bytes(),
        gp->get_height(),
        fp
    );
}


void
sink_terak::write_whole_font_end(void)
{
    explain_fflush_or_die(fp);
}


bool
sink_terak::put_boot_logo(const unsigned char *data)
{
    explain_fseek_or_die(fp, 0x800, SEEK_SET);
    explain_fwrite_or_die(data, 1, 512, fp);
    explain_fflush_or_die(fp);
    return true;
}


// vim: set ts=8 sw=4 et :
