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

#include <lib/input/file.h>
#include <lib/input/stdin.h>
#include <lib/input/ucsd_text.h>


input::pointer
input::factory(const rcstring &path)
{
    input::pointer ip = input_stdin::candidate(path);
    if (ip)
        return ip;
    ip = input_ucsd_text::candidate(path);
    if (ip)
        return ip;
    return input_file::create(path);
}


// vim: set ts=8 sw=4 et :
