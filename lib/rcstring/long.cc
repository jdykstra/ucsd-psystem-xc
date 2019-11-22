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

#include <lib/ac/stdio.h>

#include <lib/rcstring.h>


static rcstring_gizzards *
long_to_gizzards(long n)
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%ld", n);
    return rcstring_gizzards_from_c(buffer);
}


rcstring::rcstring(long arg) :
    ref(long_to_gizzards(arg))
{
}


// vim: set ts=8 sw=4 et :
