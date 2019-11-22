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


rcstring
rcstring::basename(const rcstring &extension)
    const
{
    if (empty())
        return ".";

    const char *begin = c_str();
    const char *end = begin + size();

    //
    // Ignore trailing '/' characters.
    //
    while (end > begin + 1 && end[-1] == '/')
        --end;

    //
    // If all that is left is a single slash,
    // the return root ("/").
    //
    if (end - begin == 1 && *begin == '/')
        return "/";

    //
    // Now we have to find the last slash.
    //
    const char *cp = end;
    while (cp > begin)
    {
        //
        // If path does not contain a slash,
        // dirname returns the original path.
        //
        if (cp == begin)
            return rcstring(begin, end - begin);
        if (cp[-1] == '/')
            break;
        --cp;
    }

    //
    // Remove the extension (suffix)
    //
    if
    (
        size_t(end - cp) >= extension.size()
    &&
        0 == memcmp(end - extension.size(), extension.c_str(), extension.size())
    )
        end -= extension.size();

    //
    // Build the result.
    //
    return rcstring(cp, end - cp);
}


// vim: set ts=8 sw=4 et :
