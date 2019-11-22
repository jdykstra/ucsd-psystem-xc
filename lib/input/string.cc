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

#include <lib/ac/time.h>
#include <lib/ac/string.h>
#include <lib/ac/sys/stat.h>

#include <lib/debug.h>
#include <lib/input/string.h>


input_string::~input_string()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


input_string::input_string(const rcstring &a_text, const rcstring &a_filename) :
    text(a_text),
    filename(a_filename),
    pos(0)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


input_string::pointer
input_string::create(const rcstring &a_text, const rcstring &a_filename)
{
    return pointer(new input_string(a_text, a_filename));
}


rcstring
input_string::name()
{
    return filename;
}


long
input_string::length()
{
    return text.size();
}


int
input_string::fpathconf_name_max()
{
    return 15;
}


void
input_string::fstat(struct stat &st)
{
    memset(&st, 0, sizeof(st));
    st.st_mode = S_IFREG | 0644;
    st.st_nlink = 1;
    st.st_size = text.size();
    st.st_blksize = 512;
    st.st_blocks = (text.size() + st.st_blksize - 1) >> st.st_blksize;
    time(&st.st_atime);
    st.st_mtime = st.st_atime;
    st.st_ctime = st.st_atime;
}


long
input_string::read_inner(void *data, size_t nbytes)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    size_t number_of_bytes = nbytes;
    if (pos + number_of_bytes > text.size())
        number_of_bytes = text.size() - pos;
    memcpy(data, text.c_str() + pos, number_of_bytes);
    pos += number_of_bytes;
    DEBUG(2, "return %d %s", (int)number_of_bytes,
        rcstring((const char *)data, number_of_bytes).quote_c().c_str());
    DEBUG(1, "}");
    return number_of_bytes;
}


// vim: set ts=8 sw=4 et :
