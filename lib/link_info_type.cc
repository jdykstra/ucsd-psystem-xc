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

#include <lib/link_info_type.h>


rcstring
link_info_type_name(link_info_type_t x)
{
    switch (x)
    {
    case EOFMARK:
        return "EOFMARK";

    case UNITREF:
        return "UNITREF";

    case GLOBREF:
        return "GLOBREF";

    case PUBLREF:
        return "PUBLREF";

    case PRIVREF:
        return "PRIVREF";

    case CONSTREF:
        return "CONSTREF";

    case GLOBDEF:
        return "GLOBDEF";

    case PUBLDEF:
        return "PUBLDEF";

    case CONSTDEF:
        return "CONSTDEF";

    case EXTPROC:
        return "EXTPROC";

    case EXTFUNC:
        return "EXTFUNC";

    case SEPPROC:
        return "SEPPROC";

    case SEPFUNC:
        return "SEPFUNC";

    case SEPPREF:
        return "SEPPREF";

    case SEPFREF:
        return "SEPFREF";
    }
    return rcstring::printf("%d", (int)x);
};


// vim: set ts=8 sw=4 et :
