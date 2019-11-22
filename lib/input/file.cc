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

#include <lib/ac/assert.h>
#include <lib/ac/errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <lib/ac/sys/stat.h>
#include <libexplain/open.h>
#include <libexplain/read.h>
#include <libexplain/fstat.h>

#include <lib/input/file.h>


input_file::~input_file()
{
    if (fd >= 0)
    {
        ::close(fd);
        fd = -1;
    }
}


input_file::input_file(const rcstring &a_path) :
    path(a_path),
    fd(-1),
    pos(0)
{
    int mode = O_RDONLY;
#if defined(__CYGWIN__) || defined(__CYGWIN32__)
    // I'm not sure whether MacOsX uses \r or \n in its native text
    // files, so I'm reluctant to always use the O_BINARY mode bit.
    mode |= O_BINARY;
#endif
    fd = explain_open_or_die(path.c_str(), mode, 0666);
}


input_file::pointer
input_file::create(const rcstring &a_path)
{
    return pointer(new input_file(a_path));
}


long
input_file::read_inner(void *data, size_t len)
{
    if (len == 0)
        return 0;
    if (fd < 0)
        return -EBADF;
    ssize_t result = explain_read_or_die(fd, data, len);
    pos += result;
    return result;
}


rcstring
input_file::name()
{
    return path;
}


long
input_file::length()
{
    if (fd < 0)
        return -EBADF;
    struct stat st;
    explain_fstat_or_die(fd, &st);
    return st.st_size;
}


int
input_file::fpathconf_name_max()
{
    if (fd < 0)
        return -EBADF;
    long n = fpathconf(fd, _PC_NAME_MAX);
    if (n < 0)
        return -errno;
    return n;
}


void
input_file::fstat(struct stat &st)
{
    assert(fd >= 0);
    explain_fstat_or_die(fd, &st);
}


// vim: set ts=8 sw=4 et :
