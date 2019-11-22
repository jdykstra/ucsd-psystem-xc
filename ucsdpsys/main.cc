//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2009-2012 Peter Miller
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

#include <lib/ac/ctype.h>
#include <lib/ac/stdio.h>
#include <lib/ac/stdlib.h>
#include <lib/ac/sys/stat.h>
#include <lib/ac/getopt.h>
#include <libexplain/getcwd.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/system.h>
#include <unistd.h>

#include <lib/libdir.h>
#include <lib/mtype.h>
#include <lib/path_join.h>
#include <lib/version.h>
#include <ucsdpsys/volume/directory.h>
#include <ucsdpsys/volume/file.h>
#include <ucsdpsys/volume/list.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ]\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


static void
needs_string(const char *opt)
{
    explain_output_error_and_die
    (
        "the %s option needs a non-empty string parameter",
        opt
    );
}


static bool
is_a_directory(const rcstring &path)
{
    struct stat st;
    return (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode));
}


static rcstring
sanitise(const rcstring text)
{
    char buffer[8];
    char *bp = buffer;
    const char *cp = text.c_str();
    for (;;)
    {
        unsigned char c = *cp++;
        if (!c)
            break;
        if (bp == buffer ? isalpha(c) : isalnum(c))
        {
            *bp++ = c;
            if (bp >= buffer + 7)
                break;
        }
    }
    return rcstring(buffer, bp - buffer);
}


static rcstring
name_from_dir(const rcstring &arg)
{
    bool used_dot = false;
    rcstring path = arg;
    for (;;)
    {
        if (path == "/")
            return "work";
        if (path == "." || path.empty())
        {
            if (used_dot)
                return "work";
            used_dot = true;
            char buf[2000];
            explain_getcwd_or_die(buf, sizeof(buf));
            path = buf;
            continue;
        }
        rcstring result = sanitise(path.basename());
        if (!result.empty())
            return result;
        path = path.dirname();
    }
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    rcstring_list system_volume_dirs;
    rcstring host = "klebsch";
    rcstring architecture;
    rcstring_list command;
    volume_list::pointer volumes = volume_list::create();
    bool build_system_disk_image = true;
    for (;;)
    {
        static const struct option options[] =
        {
            { "apple", 0, 0, 'a' },
            { "architecture", 1, 0, 'A' },
            { "batch", 1, 0, 'b' },
            { "debug", 0, 0, 'g' },
            { "forget", 1, 0, 'f' },
            { "host", 1, 0, 'H' },
            { "machine", 1, 0, 'A' },
            { "name", 1, 0, 'n' },
            { "no-system", 0, 0, 'N' },
            { "p-machine", 1, 0, 'P' },
            { "read-only", 1, 0, 'r' },
            { "system-directory", 1, 0, 'S' },
            { "trace", 0, 0, 'T' },
            { "trace-file", 1, 0, 't' },
            { "trace-max", 0, 0, 'g' },
            { "version", 0, 0, 'V' },
            { "write", 1, 0, 'w' },
            { "xterm", 0, 0, 'x' },
            { 0, 0, 0, 0 }
        };
        int c =
            getopt_long(argc, argv, "A:ab:f:gH:NnP:r:S:Tt:w:xV", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'A':
            architecture = optarg;
            break;

        case 'a':
            // apple compatibility
            command.push_back("-a");
            break;

        case 'b':
            // batch
            if (!*optarg)
                needs_string("batch (-b)");
            command.push_back("-b" + rcstring(optarg).quote_shell());
            break;

        case 'f':
            goto volume;

        case 'g':
            // debug
            command.push_back("-g");
            break;

        case 'H':
            host = rcstring(optarg).downcase();
            break;

        case 'N':
            build_system_disk_image = false;
            break;

        case 'n':
            // batch
            if (!*optarg)
                needs_string("system name (-n)");
            command.push_back("-n" + rcstring(optarg).quote_shell());
            break;

        case 'P':
            command.push_back("--p-machine=" + rcstring(optarg).quote_shell());
            break;

        case 'r':
            goto volume;

        case 'S':
            // System volume directory
            if (!*optarg)
                needs_string("system directory (-S)");
            system_volume_dirs.push_back(optarg);
            break;

        case 'T':
            // trace
            command.push_back("-T");
            break;

        case 't':
            // trace file
            if (!*optarg)
                needs_string("trace file (-t)");
            command.push_back("-t" + rcstring(optarg).quote_shell());
            break;

        case 'V':
            // version
            version_print();
            return 0;

        case 'w':
            // volume to mount
            volume:
            if (!*optarg)
                needs_string("mount volume");
            if (is_a_directory(optarg))
            {
                rcstring name = name_from_dir(optarg);
                volume::pointer vp = volume_directory::create(optarg, c, name);
                volumes->push_back(vp);
            }
            else
            {
                volume::pointer vp = volume_file::create(optarg, c);
                volumes->push_back(vp);
            }
            break;

        case 'x':
            // use the X11 interface
            command.push_back("-x");
            break;

        default:
            usage();
            // NOTREACHED
        }
    }
    if (optind != argc)
        usage();

    // patch the architecture, if necessary
    if (architecture.empty())
    {
        mtype_t m = mtype_from_host(host);
        if (m == mtype_undefined)
        {
            explain_output_error_and_die
            (
                "host %s unknown",
                host.quote_c().c_str()
            );
        }
        architecture = mtype_name(m).downcase();
    }

    //
    // If they do not name any directories at all, make a temporary
    // forget-mode volume from the contents in the current directory.
    //
    if (volumes->empty())
    {
        explain_output_warning
        (
            "no volumes named on the command line; assuming you meant "
            "\"-f .\" which uses the contents of the current directory "
            "but forgets all changes on exit"
        );
        rcstring dir = ".";
        volume::pointer vp =
            volume_directory::create(dir, 'f', name_from_dir(dir));
        volumes->push_back(vp);
    }

    if (build_system_disk_image && !volumes->contains_system_files())
    {
        //
        // Default the system volume directories.
        //
        if (system_volume_dirs.empty())
        {
            rcstring_list seed;
            seed.push_back_unique(configured_libdir());
            seed.push_back_unique(configured_datadir());
            seed.push_back_unique("/usr/share");
            seed.push_back_unique("/usr/lib");
            seed.push_back_unique("/usr/local/share");
            seed.push_back_unique("/usr/local/lib");

            //
            // These are from the ucsd-psystem-os project.
            // The one with the operating system sources.
            //
            for (size_t j = 0; j < seed.size(); ++j)
            {
                rcstring dir = seed[j];
                if (is_a_directory(dir))
                {
                    // There could be some from ucsd-psystem-os
                    rcstring osdir = dir + "/ucsd-psystem-os";
                    if (is_a_directory(osdir))
                        system_volume_dirs.push_back_unique(osdir);

                    // There could be some from ucsd-psystem-vm
                    // (for the turtle graphics unit, etc)
                    rcstring vmdir = dir + "/ucsd-psystem-vm";
                    if (is_a_directory(vmdir))
                        system_volume_dirs.push_back_unique(vmdir);
                }
            }
        }

        //
        // Expand the directory list, to hunt for architecture specific
        // files, and system files, in the locations that are used to
        // instal the the ucsd-psystem-os project.
        //
        rcstring_list path;
        for (size_t j = 0; j < system_volume_dirs.size(); ++j)
        {
            rcstring dir = system_volume_dirs[j];
            if (is_a_directory(dir))
            {
                // Search architecture specific directories before
                // architecture neutral directories.
                {
                    rcstring d1 = path_join(dir, "system", architecture);
                    if (is_a_directory(d1))
                        path.push_back(d1);
                }

                {
                    rcstring d2 = path_join(dir, "system", host);
                    if (is_a_directory(d2))
                        path.push_back(d2);
                }

                {
                    rcstring d2 = path_join(dir, architecture);
                    if (is_a_directory(d2))
                        path.push_back(d2);
                }
                {
                    rcstring d2 = path_join(dir, host);
                    if (is_a_directory(d2))
                        path.push_back(d2);
                }

                {
                    rcstring d3 = path_join(dir, "system");
                    if (is_a_directory(d3))
                        path.push_back(d3);
                }

                path.push_back(dir);
            }
        }
        if (path.empty())
        {
            explain_output_error_and_die
            (
                "unable to locate a directory containing system files, "
                    "looked in %s",
                system_volume_dirs.unsplit(", ").c_str()
            );
        }

        // we always want this as volume #5:
        volume::pointer svp = volume_directory::create(path, 'r', "system");
        volumes->insert_second(svp);

        // make sure our implied system volume actually contains a
        // "system.pascal" file, otherwise we can't boot.
        if (!svp->contains_system_pascal())
        {
            explain_output_error_and_die
            (
                "none of the system directories contain a \"system.pascal\" "
                    "file, looked in %s",
                path.unsplit(", ").c_str()
            );
        }
    }

    //
    // Now add all of the volumes to the end of the command.
    //
    command.push_back(volumes->command_line_argument());

    //
    // slurp the temporary volumes
    //
    volumes->slurp();

    //
    // Take a quick look at all the volumes, to be sure that
    // "system.pascal" is on at least one of them.
    //
    if (!volumes->contains_system_pascal())
    {
        explain_output_error_and_die
        (
            "none of the supplied volumes contain a \"system.pascal\" file"
        );
    }

    //
    // Run the sub-command
    //
    command.push_front("ucsdpsys_vm");
    rcstring s = command.unsplit();
    explain_output_message(s.c_str());
    explain_system_success_or_die(s.c_str());

    //
    // unslurp the temporary volumes
    // (This removes any temporary files, too.)
    //
    volumes->unslurp();
    volumes.reset();

    //
    // Report success.
    //
    return 0;
}


// vim: set ts=8 sw=4 et :
