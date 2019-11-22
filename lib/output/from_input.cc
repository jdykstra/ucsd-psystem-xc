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

#include <lib/debug.h>
#include <lib/input.h>
#include <lib/output.h>


output::pointer &
operator<<(output::pointer &os, input::pointer &is)
{
    for (;;)
    {
        unsigned char buffer[4096];
        DEBUG(3, "is->read(%p, 0x%lX);", buffer, (long)sizeof(buffer));
        long n = is->read(buffer, sizeof(buffer));
        if (!n)
            break;
        DEBUG(3, "os->write(%p, 0x%lX);", buffer, n);
        os->write(buffer, n);
    }
    return os;
}


// vim: set ts=8 sw=4 et :
