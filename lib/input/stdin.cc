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
#include <lib/ac/stdio.h>
#include <lib/ac/sys/stat.h>
#include <libexplain/fstat.h>
#include <libexplain/read.h>
#include <unistd.h>

#include <lib/input/stdin.h>


input_stdin::~input_stdin()
{
}


input_stdin::input_stdin() :
    pos(0),
    unbuffered(false)
{
}


input_stdin::pointer
input_stdin::create()
{
    return pointer(new input_stdin());
}


input_stdin::pointer
input_stdin::candidate(const rcstring &path)
{
    if (path.empty() || path == "-")
        return create();
    return pointer();
}


long
input_stdin::read_inner(void *data, size_t len)
{
    if (len <= 0)
        return 0;
    if (unbuffered)
        len = 1;
    int fd = fileno(stdin);
    long result = explain_read_or_die(fd, data, len);
    pos += result;
    return result;
}


rcstring
input_stdin::name()
{
    return "standard input";
}


long
input_stdin::length()
{
    struct stat st;
    if (::fstat(fileno(stdin), &st) < 0)
        return -errno;
    if (!S_ISREG(st.st_mode))
        return -EINVAL;
    return st.st_size;
}


int
input_stdin::fpathconf_name_max()
{
    long n = fpathconf(fileno(stdin), _PC_NAME_MAX);
    if (n < 0)
        return -errno;
    return n;
}


void
input_stdin::fstat(struct stat &st)
{
    explain_fstat_or_die(fileno(stdin), &st);
}


// vim: set ts=8 sw=4 et :
