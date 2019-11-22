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

#include <lib/ac/ctype.h>
#include <lib/ac/stdio.h> // for snprintf

#include <lib/hexdump.h>
#include <lib/rcstring/accumulator.h>


rcstring
hexdump(const void *base, size_t len)
{
    if (len == 0)
        return "/* empty */";
    const unsigned char *cp = (const unsigned char *)base;
    static rcstring_accumulator acc;
    acc.clear();
    for (size_t j = 0; j < len; j += 16)
    {
        if (j)
            acc.push_back('\n');
        char buf[20];
        snprintf(buf, sizeof(buf), "%4.4lX:", (long)j);
        acc.push_back(buf);
        for (size_t k = 0; k < 16; ++k)
        {
            size_t n = j + k;
            if (n < len)
            {
                snprintf(buf, sizeof(buf), "%3.2X", cp[n]);
                acc.push_back(buf);
            }
            else
                acc.push_back("   ");
        }
        acc.push_back("  ");
        for (size_t k = 0; k < 16 && j + k < len; ++k)
        {
            unsigned char c = cp[j + k];
            if (!isprint(c))
                c = '.';
            acc.push_back((char)c);
        }
    }
    return acc.mkstr();
}


// vim: set ts=8 sw=4 et :
