//
// UCSD p-System operating system
// Copyright (C) 2006, 2010-2012 Peter Miller
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
#include <libexplain/system.h>

#include <ucsdpsys/volume/file.h>


volume_file::~volume_file()
{
}


volume_file::volume_file(const rcstring &a_path, char a_mode) :
    path(a_path),
    mode(a_mode)
{
}


volume_file::pointer
volume_file::create(const rcstring &a_path, char a_mode)
{
    return pointer(new volume_file(a_path, a_mode));
}


void
volume_file::slurp(void)
{
    // Do nothing.
}


void
volume_file::unslurp(void)
{
    // Do nothing.
}


rcstring
volume_file::command_line_argument(void)
{
    char prefix[] = { '-', mode, ' ' };
    return (rcstring(prefix, 3) + path);
}


bool
volume_file::contains_system_pascal(void)
    const
{
    //
    // We don't actually open the volume and look inside it.  Maybe we could if
    // ucsd-psystem-fs exported a shared library for us to use, but it doesn't.
    // So instead we use the ucsdpsys_disk(1) command from the ucsd-psystem-fs
    // package, which knows how to grope volume images.
    //
    rcstring command =
        "ucsdpsys_disk --file " + path + " --list"
        "| grep -i system.pascal > /dev/null";
    int n = explain_system_or_die(command.c_str());
    // grep exit status: 0 found a match,
    //                   1 no match,
    //                   2 all other errors.
    return (n == 0);
}


bool
volume_file::contains_system_files(void)
{
    //
    // We don't actually open the volume and look inside it.  Maybe we could if
    // ucsd-psystem-fs exported a shared library for us to use, but it doesn't.
    // So instead we use the ucsdpsys_disk(1) command from the ucsd-psystem-fs
    // package, which knows how to grope volume images.
    //
    rcstring command = "ucsdpsys_disk --file " + path + " --system-volume";
    int n = explain_system_or_die(command.c_str());
    // exit status: 0 found a match (no output),
    //              1 no match (no output),
    //              1 all other errors (with output).
    return (n == 0);
}


// vim: set ts=8 sw=4 et :
