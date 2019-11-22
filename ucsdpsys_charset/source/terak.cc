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
#include <libexplain/fread.h>
#include <libexplain/fseek.h>
#include <libexplain/fstat.h>
#include <libexplain/output.h>
#include <libexplain/stat.h>

#include <lib/get_filename.h>

#include <ucsdpsys_charset/source/terak.h>


source_terak::~source_terak()
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


source_terak::source_terak(const rcstring &a_filename) :
    filename(a_filename),
    fp(stdin)
{
    if (looks_like_a_stdin_synonym(filename))
        filename = filename_from_stream(stdin);
    else
        fp = explain_fopen_or_die(filename.c_str(), "rb");
    assert(fp);

    off_t fs = filesize(fp);
    unsigned nblocks = fs >> 9;
    if ((fs & 511) != 0 || nblocks < 4 || nblocks > 6)
    {
        explain_output_error_and_die
        (
            "%s: the unlikely file size (%ld = 0x%04lX) does not appear to be "
                "consistent a terak system.charset file",
            filename.c_str(),
            (long)fs,
            (long)fs
        );
    }
}


source_terak::pointer
source_terak::create(const rcstring &a_filename)
{
    return pointer(new source_terak(a_filename));
}


static off_t
filesize(const rcstring &filename)
{
    struct stat st;
    explain_stat_or_die(filename.c_str(), &st);
    return st.st_size;
}


bool
source_terak::candidate(const rcstring &filename)
{
    if (!filename.ends_with(".charset"))
        return false;
    off_t fs = filesize(filename);
    return ((fs & 0x03FF) == 0 && fs >= 0x0400 * 2 && fs <= 0x0400 * 3);
}


void
source_terak::read_whole_font_begin(void)
{
    rewind(fp);
}


glyph::pointer
source_terak::read_one_glyph(void)
{
    for (;;)
    {
        off_t pos = ftell(fp);
        unsigned short addr = pos;
        unsigned page = pos / 0x0400;
        if (page >= 2)
        {
            // logical end-of-file
            return glyph::pointer();
        }
        pos &= 0x3FF;
        if (pos < 960)
        {
            if (pos % 10)
            {
                explain_output_error_and_die
                (
                    "%s: 0x%04X: format error, incorrect alignment",
                    filename.c_str(),
                    addr
                );
            }
            unsigned c = (page * 128) + ' ' + (pos / 10);
            assert(c < 256);

            unsigned char data[10];
            explain_fread_or_die(data, 1, sizeof(data), fp);
            glyph::pointer gp = glyph::create(c, 8, sizeof(data), data);
            gp->set_location
            (
                rcstring::printf("%s: 0x%04X", filename.c_str(), addr)
            );
            return gp;
        }
        explain_fseek_or_die(fp, (page + 1) * 0x0400, SEEK_SET);
    }
}


bool
source_terak::is_binary(void)
    const
{
    return true;
}


bool
source_terak::get_boot_logo(unsigned char *data)
{
    if (filesize(fp) < 0xA00)
        return false;
    explain_fseek_or_die(fp, 0x800, SEEK_SET);
    explain_fread_or_die(data, 1, 512, fp);
    return true;
}


// vim: set ts=8 sw=4 et :
