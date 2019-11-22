//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 3 of the License, or
//      (at your option) any later version.
//
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//
//      You should have received a copy of the GNU General Public License
//      along with this program. If not, see
//      <http://www.gnu.org/licenses/>.
//

#include <lib/ac/string.h>
#include <lib/rcstring.h>


bool
rcstring::starts_with(const rcstring &rhs)
    const
{
    const char *haystack = c_str();
    size_t haystack_len = size();
    const char *needle = rhs.c_str();
    size_t needle_len = rhs.size();
    return
    (
        haystack_len >= needle_len
    &&
        0 == memcmp(haystack, needle, needle_len)
    );
}


// vim: set ts=8 sw=4 et :
