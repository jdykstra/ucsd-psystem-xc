//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/ac/errno.h>
#include <fcntl.h>
#include <libexplain/open.h>
#include <libexplain/utime.h>
#include <libexplain/write.h>
#include <unistd.h>
#include <utime.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <lib/output/file.h>
#include <lib/output/null.h>
#include <lib/output/stdout.h>


output_file::~output_file()
{
    //
    // Make sure all buffered data has been passed to our write_inner
    // method.
    //
    flush();

    ::close(fd);
    fd = -1;
    pos = 0;
}


output_file::output_file(const rcstring &fn, bool binary) :
    file_name(fn),
    fd(-1),
    bol(true),
    pos(0),
    width(80),
    height(66)
{
    {
        int mode = O_WRONLY | O_CREAT | O_TRUNC;
#ifdef O_BINARY
        if (binary)
            mode |= O_BINARY;
#else
        (void)binary;
#endif
        fd = explain_open_or_die(fn.c_str(), mode, 0666);
    }

    {
        struct winsize  sz;

        if (ioctl(fd, TIOCGWINSZ, &sz) >= 0)
        {
            if (sz.ws_col >= 10)
                width = sz.ws_col;
            if (sz.ws_row >= 4)
                height = sz.ws_row;
        }
    }
}


output_file::pointer
output_file::create(const rcstring &fn, bool binary)
{
    if (fn == "/dev/null")
        return output_null::create();
    if (fn == "-")
        return output_stdout::create();
    return pointer(new output_file(fn, binary));
}


rcstring
output_file::filename()
{
    return file_name;
}


void
output_file::write_inner(const void *data, size_t len)
{
    explain_write_or_die(fd, data, len);
    if (len > 0)
        bol = (((char *)data)[len - 1] == '\n');
    pos += len;
}


void
output_file::flush_inner()
{
}


void
output_file::utime(struct utimbuf &utb)
{
    explain_utime_or_die(file_name.c_str(), &utb);
}


unsigned
output_file::get_width(void)
    const
{
    return width;
}


unsigned
output_file::get_height(void)
    const
{
    return height;
}


// vim: set ts=8 sw=4 et :
