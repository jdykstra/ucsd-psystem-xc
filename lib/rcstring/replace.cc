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
#include <lib/rcstring/accumulator.h>


rcstring
rcstring::replace(const rcstring &lhs, const rcstring &rhs, int how_many_times)
    const
{
    //
    // Deal with some trivial cases.
    //
    if (lhs.empty() || how_many_times == 0)
        return *this;

    //
    // Default the number of times, if necessary.
    //
    if (how_many_times < 0)
        how_many_times = length() + 1;

    //
    // Walk along the string replacing things.
    //
    const char *ip = c_str();
    const char *ip_end = ip + size();
    static rcstring_accumulator sa;
    sa.clear();
    while (ip < ip_end && (size_t)(ip_end - ip) >= lhs.size())
    {
        if (0 == memcmp(ip, lhs.c_str(), lhs.size()))
        {
            sa.push_back(rhs);
            ip += lhs.size();
            if (--how_many_times <= 0)
                break;
        }
        else
        {
            char c = *ip++;
            sa.push_back(c);
        }
    }

    //
    // Collect the tail-end of the input.
    //
    sa.push_back(ip, ip_end - ip);

    //
    // Build ther answer.
    //
    return sa.mkstr();
}


// vim: set ts=8 sw=4 et :
