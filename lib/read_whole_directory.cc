//
// UCSD p-System cross compiler
// Copyright (C) 2011, 2012 Peter Miller
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

#include <lib/config.h>
#include <lib/ac/string.h>
#include <dirent.h>
#include <libexplain/opendir.h>
#include <libexplain/readdir.h>
#include <libexplain/closedir.h>

#include <lib/read_whole_directory.h>


void
read_whole_directory(const rcstring &path, rcstring_list &result)
{
    DIR *dp = explain_opendir_or_die(path.c_str());
    for (;;)
    {
        dirent *dep = explain_readdir_or_die(dp);
        if (!dep)
            break;
        if (strcmp(dep->d_name, ".") && strcmp(dep->d_name, ".."))
            result.push_back(dep->d_name);
    }
    explain_closedir_or_die(dp);
}


// vim: set ts=8 sw=4 et :
