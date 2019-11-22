//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#include <lib/get_filename.h>


rcstring
filename_from_stream(FILE *fp)
{
    char buffer[2000];
    if (explain_filename_from_stream(fp, buffer, sizeof(buffer)) < 0)
    {
        if (fp == stdin)
            return "the standard input";
        if (fp == stdout)
            return "the standard output";
        if (fp == stderr)
            return "the standard error";
        return "anonymous file";
    }
    rcstring absolute(buffer);
    if (absolute[0] != '/')
    {
        // sockets, pipes, etc
        return absolute;
    }

    // grab the current directory
    explain_getcwd_or_die(buffer, sizeof(buffer));
    rcstring dot(buffer);

    // is it the current directory?
    if (dot == absolute)
        return ".";

    // is it below the current directory?
    if (absolute.starts_with(dot) && absolute[dot.size()] == '/')
    {
        return
            absolute.substring
            (
                dot.size() + 1,
                absolute.size() - dot.size() - 1
            );
    }

    // just use the sobsolute path
    return absolute;
}


// vim: set ts=8 sw=4 et :
