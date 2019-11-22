//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
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

#include <lib/ac/assert.h>
#include <lib/ac/ctype.h>
#include <lib/ac/errno.h>
#include <lib/ac/stdio.h>
#include <lib/ac/stdlib.h>
#include <fcntl.h>
#include <lib/ac/map>
#include <lib/ac/sys/stat.h>
#include <libexplain/close.h>
#include <libexplain/closedir.h>
#include <libexplain/open.h>
#include <libexplain/opendir.h>
#include <libexplain/output.h>
#include <libexplain/read.h>
#include <libexplain/readdir.h>
#include <libexplain/program_name.h>
#include <libexplain/stat.h>
#include <libexplain/system.h>
#include <libexplain/unlink.h>
#include <unistd.h>

#include <lib/read_whole_directory.h>

#include <ucsdpsys/volume/directory.h>


volume_directory::~volume_directory()
{
    if
    (
        !temporary_file_name.empty()
    &&
        unlink(temporary_file_name.c_str()) < 0
    &&
        errno != ENOENT
    )
    {
        explain_output_error_and_die
        (
            "%s",
            explain_unlink(temporary_file_name.c_str())
        );
    }
}


volume_directory::volume_directory(
    const rcstring &a_path,
    char a_mode,
    const rcstring &a_name
) :
    mode(a_mode),
    volume_name(a_name),
    slurped(false)
{
    directories.push_back(a_path);
}


volume_directory::pointer
volume_directory::create(const rcstring &a_path, char a_mode,
    const rcstring &a_name)
{
    return pointer(new volume_directory(a_path, a_mode, a_name));
}


volume_directory::volume_directory(
    const rcstring_list &a_path,
    char a_mode,
    const rcstring &a_name
) :
    directories(a_path),
    mode(a_mode),
    volume_name(a_name)
{
}


volume_directory::pointer
volume_directory::create(const rcstring_list &a_path, char a_mode,
    const rcstring &a_name)
{
    return pointer(new volume_directory(a_path, a_mode, a_name));
}


void
volume_directory::push_back(const rcstring &path)
{
    directories.push_back(path);
}


static rcstring
temp_volume_name(void)
{
    for (int j = 1; ; ++j)
    {
        rcstring path =
            rcstring::printf
            (
                "/tmp/%s-%d-%d.vol",
                explain_program_name_get(),
                getpid(),
                j
            );
#ifndef O_NOFOLLOW
#define O_NOFOLLOW 0
#endif
        // Be wary of symlink attacks.
        int flags = O_WRONLY + O_CREAT + O_EXCL + O_NOFOLLOW;
        int mode = 0600;
        int fd = open(path.c_str(), flags, mode);
        if (fd >= 0)
        {
            close(fd);
            return path;
        }
    }
}


const rcstring &
volume_directory::get_temporary_file_name(void)
{
    if (temporary_file_name.empty())
    {
        temporary_file_name = temp_volume_name();
    }
    return temporary_file_name;
}


static int
looks_like_text(const rcstring &path)
{
    int fd = explain_open_or_die(path.c_str(), O_RDONLY, 0666);
    unsigned char data[1024];
    int n = explain_read_or_die(fd, data, sizeof(data));
    explain_close_or_die(fd);
    if (n <= 0)
        return false;
    for (int j = 0; j < n; ++j)
    {
        unsigned char c = data[j];
        if (!isprint(c) && !isspace(c))
            return false;
    }
    return true;
}


static int
estimate_directory_size(const rcstring &path)
{
    int nblocks = 0;
    DIR *dp = explain_opendir_or_die(path.c_str());
    for (;;)
    {
        struct dirent *dep = explain_readdir_or_die(dp);
        if (!dep)
            break;
        rcstring name = dep->d_name;
        rcstring path2 = path + "/" + name;
        struct stat st;
        explain_stat_or_die(path2.c_str(), &st);
        if (S_ISREG(st.st_mode))
        {
            if (looks_like_text(path2))
                nblocks += 2 + (((st.st_size + 1023) >> 10) << 1);
            else
                nblocks += (st.st_size + 511) >> 9;
        }
    }
    explain_closedir_or_die(dp);
    return nblocks;
}


static int
estimate_directory_size(const rcstring_list &directories)
{
    int nblocks = 0;
    for (size_t j = 0; j < directories.size(); ++j)
        nblocks += estimate_directory_size(directories[j]);
    if (nblocks < 512)
        nblocks = 512;
    else if (nblocks > 32768 - 8)
        nblocks = 32768 - 8;
    else
        nblocks = (nblocks + 127) & ~127;
    return nblocks;
}


static bool
is_a_regular_file(const rcstring &path)
{
    struct stat st;
    explain_stat_or_die(path.c_str(), &st);
    return S_ISREG(st.st_mode);
}


void
volume_directory::slurp(void)
{
    if (slurped)
        return;
    slurped = true;

    if (volume_name.empty())
    {
        assert(!directories.empty());
        volume_name = directories[0].basename().identifier();
    }

    int nblocks = estimate_directory_size(directories);

    //
    // Build the new volume_directory.
    //
    rcstring command = "ucsdpsys_mkfs -B"
        + rcstring::printf("%d", nblocks / 2)
        + " --label="
        + volume_name.quote_shell() + " "
        + get_temporary_file_name().quote_shell();
    explain_output_message(command.c_str());
    explain_system_success_or_die(command.c_str());

    //
    // Read the directories, using the files present therein, except
    // we only use the first file found for each unique basename.
    //
    rcstring_list contents;
    {
        typedef std::map<rcstring, rcstring> sources_t;
        sources_t sources;
        for (size_t j = 0; j < directories.size(); ++j)
        {
            rcstring dir = directories[j];
            rcstring_list names;
            read_whole_directory(dir, names);
            for (size_t k = 0; k < names.size(); ++k)
            {
                rcstring name = names[k];
                rcstring name_lc = name.downcase();
                if (sources.find(name_lc) == sources.end())
                {
                    rcstring path = dir + "/" + name;
                    if (is_a_regular_file(path))
                    {
                        sources[name_lc] = path;
                        contents.push_back(path);
                    }
                }
            }
        }
    }

    //
    // Transfer the contents into in the new volume_directory.
    //
    command = "ucsdpsys_disk --file " + get_temporary_file_name().quote_shell()
        + " --put " + contents.quote_shell().unsplit();
    explain_output_message(command.c_str());
    explain_system_success_or_die(command.c_str());
}


void
volume_directory::unslurp(void)
{
    if (mode == 'w')
    {
        //
        // Transfer the contents out of the new volume_directory.
        // (We only do this for the first directory.)
        //
        rcstring command = "ucsdpsys_disk -f "
            + get_temporary_file_name().quote_shell() + " -g "
            + directories[0].quote_shell();
        explain_output_message(command.c_str());
        explain_system_success_or_die(command.c_str());
    }

    //
    // Get rid of the temporary file.
    //
    explain_unlink_or_die(get_temporary_file_name().c_str());
}


rcstring
volume_directory::command_line_argument(void)
{
    char opt[] = { ' ', '-', mode, ' ' };
    return (rcstring(opt, 4) + get_temporary_file_name().quote_shell());
}


void
volume_directory::set_volume_name(const rcstring &name)
{
    volume_name = name;
}


bool
volume_directory::contains_system_pascal(void)
    const
{
    for (size_t j = 0; j < directories.size(); ++j)
    {
        rcstring dir = directories[j];
        rcstring_list names;
        read_whole_directory(dir, names);
        for (size_t k = 0; k < names.size(); ++k)
        {
            rcstring name = names[k];
            rcstring name_lc = name.downcase();
            if (name_lc == "system.pascal")
            {
                rcstring path = dir + "/" + name;
                if (is_a_regular_file(path))
                    return true;
            }
        }
    }
    return false;
}


bool
volume_directory::contains_system_files(void)
{
    slurp();
    //
    // We don't actually open the volume and look inside it.  Maybe we could if
    // ucsd-psystem-fs exported a shared library for us to use, but it doesn't.
    // So instead we use the ucsdpsys_disk(1) command from the ucsd-psystem-fs
    // package, which knows how to grope volume images.
    //
    rcstring command =
        "ucsdpsys_disk --file " + get_temporary_file_name().quote_shell() +
        " --system-volume";
    int n = explain_system_or_die(command.c_str());
    // exit status: 0 found a match (no output),
    //              1 no match (no output),
    //              1 all other errors (with output).
    return (n == 0);
}


// vim: set ts=8 sw=4 et :
