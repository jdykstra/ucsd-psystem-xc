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

#include <lib/path_join.h>


rcstring
path_join(const rcstring &lhs, const rcstring &rhs)
{
    if (lhs == "." || lhs.empty())
        return rhs;
    if (lhs[lhs.size() - 1] == '/')
        return (lhs + rhs);
    return (lhs + "/" + rhs);
}


rcstring
path_join(const rcstring &f1, const rcstring &f2, const rcstring &f3)
{
    return path_join(path_join(f1, f2), f3);
}


rcstring
path_join(const rcstring &f1, const rcstring &f2, const rcstring &f3,
    const rcstring &f4)
{
    return path_join(path_join(f1, f2), path_join(f3, f4));
}


// vim: set ts=8 sw=4 et :
