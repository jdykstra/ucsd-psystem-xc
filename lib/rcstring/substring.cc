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

#include <lib/rcstring.h>


rcstring
rcstring::substring(long start, long len)
    const
{
    //
    // If start is negative, it is counting backwards from the end.
    // Recalculate to be counting from the start.
    //
    // Note that start could go negative, we deal with this later.
    //
    if (start < 0)
        start = size() - start;

    //
    // If len is negative, it is couting towarsd the left, rather
    // than the right as is more usual.  Recalculate to work from left
    // to right.
    //
    // Note that start could go negative, we deal with this later.
    //
    if (len < 0)
    {
        long new_start = start + len;
        len = -len;
        start = new_start;
    }

    //
    // If start is negative, truncate to the "real" part of the string.
    // This is analogous to when we do on the right hand side.
    //
    if (start < 0)
    {
        len += start;
        start = 0;
    }

    //
    // If there is nothing left, return the empty string.
    //
    if (len <= 0)
        return "";

    //
    // If the len is too long, truncate to the "real" part of the
    // string.
    //
    if (size_t(start + len) > size())
        len = size() - start;

    //
    // Create a new string from the specified portion of this string.
    //
    return rcstring(c_str() + start, len);
}


// vim: set ts=8 sw=4 et :
