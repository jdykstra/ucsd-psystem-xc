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

#include <lib/ac/stdio.h>
#include <libexplain/fflush.h>
#include <libexplain/fwrite.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <lib/output/stdout.h>
#include <lib/rcstring.h>


output_stdout::~output_stdout()
{
    //
    // Make sure all buffered data has been passed to our write_inner
    // method.
    //
    flush();
}


output_stdout::output_stdout() :
    bol(true),
    pos(0),
    width(80),
    height(24)
{
    struct winsize  sz;

    if (ioctl(fileno(stdout), TIOCGWINSZ, &sz) >= 0)
    {
        if (sz.ws_col >= 10)
            width = sz.ws_col;
        if (sz.ws_row >= 4)
            height = sz.ws_row;
    }
}


output_stdout::pointer
output_stdout::create()
{
    return pointer(new output_stdout());
}


rcstring
output_stdout::filename()
{
    return "standard output";
}


void
output_stdout::write_inner(const void *data, size_t len)
{
    explain_fwrite_or_die(data, 1, len, stdout);
    if (len > 0)
        bol = (((const char *)data)[len - 1] == '\n');
    pos += len;
}


void
output_stdout::flush_inner()
{
    explain_fflush_or_die(stdout);
}


void
output_stdout::utime(utimbuf&)
{
    // Do nothing.
}


unsigned
output_stdout::get_width(void)
    const
{
    return width;
}


unsigned
output_stdout::get_height(void)
    const
{
    return height;
}


// vim: set ts=8 sw=4 et :
