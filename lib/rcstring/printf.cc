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

#include <lib/ac/stdio.h> // for vsnprintf

#include <lib/rcstring.h>


rcstring
rcstring::printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    rcstring s = vprintf(fmt, ap);
    va_end(ap);
    return s;
}


rcstring
rcstring::vprintf(const char *fmt, va_list ap)
{
    char buffer[1 << 11];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    return buffer;
}


// vim: set ts=8 sw=4 et :
