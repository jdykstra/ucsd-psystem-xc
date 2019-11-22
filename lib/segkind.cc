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

#include <lib/segkind.h>


rcstring
segkind_name(segkind_t x)
{
    switch (x)
    {
    case LINKED:
        return "LINKED";

    case HOSTSEG:
        return "HOSTSEG";

    case SEGPROC:
        return "SEGPROC";

    case UNITSEG:
        return "UNITSEG";

    case SEPRTSEG:
        return "SEPRTSEG";

    case UNLINKED_INTRINS:
        return "UNLINKED_INTRINS";

    case LINKED_INTRINS:
        return "LINKED_INTRINS";

    case DATASEG:
        return "DATASEG";
    }
    return rcstring::printf("unknown (%d)", x);
}


// vim: set ts=8 sw=4 et :
