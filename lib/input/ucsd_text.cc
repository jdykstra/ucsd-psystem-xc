//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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

#include <lib/ac/ctype.h>
#include <fcntl.h>
#include <libexplain/close.h>
#include <libexplain/fstat.h>
#include <libexplain/open.h>
#include <libexplain/output.h>
#include <libexplain/read.h>

#include <lib/input/ucsd_text.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif


input_ucsd_text::~input_ucsd_text()
{
    if (fd >= 0)
    {
        explain_close_or_die(fd);
        fd = -1;
    }
}


static bool
is_text_block(const unsigned char *block, size_t size)
{
    // Blocks of text contain one or more whole lines.
    // If padding is needed, NUL characters are used.
    assert(size == 1024);
    for (;;)
    {
        if (size == 0)
            return false;
        if (block[size - 1] != 0)
            break;
        --size;
    }
    if (block[size - 1] != '\r')
        return false;

    size_t pos = 0;
    while (pos < size)
    {
        unsigned char c = block[pos++];
        if (c == 16)
        {
            if (pos >= size)
                return false;
            if (block[pos++] < ' ')
                return false;
            continue;
        }
        if (c == '\r')
            continue;
        if (!isprint(c))
            return false;
    }
    return true;
}


input_ucsd_text::input_ucsd_text(const rcstring &a_path) :
    path(a_path),
    fd(-1),
    pos(0),
    block_size(0),
    block_posn(0),
    dle_seen(0)
{
    fd = explain_open_or_die(path.c_str(), O_RDONLY | O_BINARY, 0);

    //
    // The first 1kB block is supposed to be a binary blob for the
    // editor's personal use.  Sometimes this header has been ripped
    // off, and we must cope with that.
    //
    block_size = explain_read_or_die(fd, block, sizeof(block));
    if (block_size == 0)
        return;
    if (block_size != sizeof(block))
    {
        format_error();
    }
    if (!is_text_block(block, block_size))
        block_size = 0;
    else
    {
        for (;;)
        {
            if (block[block_size - 1] == '\r')
                break;
            if (block[block_size - 1])
            {
                format_error();
            }
            --block_size;
            if (block_size == 0)
            {
                format_error();
            }
        }
    }
}


input_ucsd_text::pointer
input_ucsd_text::create(const rcstring &a_path)
{
    return pointer(new input_ucsd_text(a_path));
}


input_ucsd_text::pointer
input_ucsd_text::candidate(const rcstring &a_path)
{
    int fd = open(a_path.c_str(), O_RDONLY | O_BINARY, 0);
    if (fd < 0)
        return pointer();
    unsigned char block[1024];
    ssize_t n = explain_read_or_die(fd, block, sizeof(block));
    if (n != sizeof(block))
    {
        close(fd);
        return pointer();
    }

    // If the file header has been ripped off, the first block will be
    // in UCSD p-System text format.
    if (is_text_block(block, sizeof(block)))
    {
        close(fd);
        return create(a_path);
    }

    // Check the second block.
    n = explain_read_or_die(fd, block, sizeof(block));
    close(fd);
    if (n == 0)
    {
        // Looks like an empty UCSD p-System text file.
        return create(a_path);
    }
    if (n != sizeof(block))
        return pointer();
    if (!is_text_block(block, sizeof(block)))
        return pointer();

    // Looks good.
    return create(a_path);
}



long
input_ucsd_text::read_inner(void *vdata, size_t len)
{
    if (len == 0)
        return 0;
    assert(fd >= 0);

    unsigned char *data = (unsigned char *)vdata;
    unsigned char *data_start = data;
    unsigned char *data_end = data + len;
    while (data < data_end)
    {
        if (dle_seen > 0)
        {
            *data++ = ' ';
            --dle_seen;
            continue;
        }
        if (block_posn >= block_size)
        {
            block_posn = 0;
            block_size = explain_read_or_die(fd, block, sizeof(block));
            if (block_size == 0)
                goto done;
            if (block_size != sizeof(block))
            {
                format_error();
            }
            for (;;)
            {
                if (block[block_size - 1] == '\r')
                    break;
                if (block[block_size - 1])
                {
                    format_error();
                }
                --block_size;
                if (block_size == 0)
                {
                    format_error();
                }
            }
        }
        unsigned char c = block[block_posn++];
        switch (c)
        {
        case 16:
            if (block_posn >= block_size)
            {
                format_error();
            }
            c = block[block_posn++];
            if (c < ' ')
            {
                format_error();
            }
            dle_seen = c - ' ';
            break;

        case '\r':
            *data++ = '\n';
            break;

        default:
            *data++ = c;
            break;
        }
    }
    done:
    size_t result = data - data_start;
    pos += result;
    return result;
}


rcstring
input_ucsd_text::name()
{
    return path;
}


long
input_ucsd_text::length()
{
    assert(fd >= 0);
    struct stat st;
    explain_fstat_or_die(fd, &st);
    return st.st_size;
}


int
input_ucsd_text::fpathconf_name_max()
{
    return 15;
}


void
input_ucsd_text::fstat(struct stat &st)
{
    assert(fd >= 0);
    explain_fstat_or_die(fd, &st);
}


void
input_ucsd_text::format_error(void)
{
    explain_output_error_and_die
    (
        "%s: does not appear to be a valid UCSD p-System text file",
        path.c_str()
    );
}


// vim: set ts=8 sw=4 et :
