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
#include <lib/ac/sys/stat.h>
#include <libexplain/fclose.h>
#include <libexplain/fopen.h>
#include <libexplain/fstat.h>
#include <libexplain/getc.h>
#include <libexplain/output.h>
#include <libexplain/stat.h>

#include <lib/get_filename.h>

#include <ucsdpsys_charset/source/apple.h>


source_apple::~source_apple()
{
    explain_fclose_or_die(fp);
    fp = 0;
}


static off_t
filesize(FILE *fp)
{
    struct stat st;
    explain_fstat_or_die(fileno(fp), &st);
    return st.st_size;
}


source_apple::source_apple(const rcstring &a_filename) :
    filename(a_filename),
    fp(stdin)
{
    if (looks_like_a_stdin_synonym(filename))
        filename = filename_from_stream(fp);
    else
        fp = explain_fopen_or_die(filename.c_str(), "rb");
    assert(fp);

    off_t fs = filesize(fp);
    if (fs != 1024)
    {
        explain_output_error_and_die
        (
            "%s: the unlikely file size (%ld = 0x%04lX) does not appear to be "
                "consistent with an Apple Pascal system.charset file "
                "(size 1024)",
            filename.c_str(),
            (long)fs,
            (long)fs
        );
    }
}


source_apple::pointer
source_apple::create(const rcstring &a_filename)
{
    return pointer(new source_apple(a_filename));
}


static off_t
filesize(const rcstring &filename)
{
    struct stat st;
    explain_stat_or_die(filename.c_str(), &st);
    return st.st_size;
}


bool
source_apple::candidate(const rcstring &filename)
{
    return (filename.ends_with(".charset") && filesize(filename) == 1024);
}


void
source_apple::read_whole_font_begin(void)
{
    rewind(fp);
}


glyph::pointer
source_apple::read_one_glyph(void)
{
    off_t pos = ftell(fp);
    unsigned short addr = pos;
    unsigned c = pos / 8;
    if (c >= 128)
        return glyph::pointer();

    if (pos & 7)
    {
        explain_output_error_and_die
        (
            "%s: 0x%04X: format error, incorrect alignment",
            filename.c_str(),
            addr
        );
    }

    // the rows are bottom-to-top
    // but class glyph uses top-to-bottom
    //
    // The Apple Pascal documentation says that the glyphs are 7x8.
    // We will use 7x8 size, unless contradicted by an actual byte with
    // the high bit set, in which has we will use 8x8.
    unsigned char data[8];
    bool high_bit_on = false;
    for (unsigned y = 0; y < sizeof(data); ++y)
    {
        unsigned s1y = sizeof(data) - 1 - y;
        data[s1y] = explain_getc_or_die(fp);
        if (0x80 & data[s1y])
            high_bit_on = true;
    }

    unsigned width = high_bit_on ? 8 : 7;
    unsigned height = 8;
    glyph::pointer gp = glyph::create(c, width, height, data);
    gp->set_location(rcstring::printf("%s: 0x%04X", filename.c_str(), addr));
    return gp;
}


bool
source_apple::is_binary(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
