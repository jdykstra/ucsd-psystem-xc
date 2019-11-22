//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#include <lib/ac/stdio.h>
#include <lib/ac/getopt.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <unistd.h>

#include <lib/codefile/file.h>
#include <lib/debug.h>
#include <lib/output.h>
#include <lib/p_machine.h>
#include <lib/rcstring.h>
#include <lib/segfault.h>
#include <lib/version.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <codefile>...\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    set_segfault_handler();
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    rcstring ofn = "-";
    int flags = 0;
    for (;;)
    {
        static const struct option options[] =
        {
            { "no-address", 0, 0, 'a' },
            { "comment", 0, 0, 'c' },
            { "debug", 0, 0, 'd' },
            { "extended", 0, 0, 'e' },
            { "output", 1, 0, 'o' },
            { "p-machine", 1, 0, 'P' },
            { "version", 0, 0, 'V' },
            { 0, 0, 0, 0 }
        };
        int c = getopt_long(argc, argv, "acdeo:P:V", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'a':
            //
            // Don't print the actual address.
            // This makes diff(1) more informative in automatic tests.
            //
            flags |= 1;
            break;

        case 'c':
            //
            // Include a comment describing each opcode.
            //
            flags |= 2;
            break;

        case 'd':
            ++debug_level;
            break;

        case 'e':
            //
            // Include the layout of the codefile itself in the listing.
            //
            flags |= 0x80;
            break;

        case 'o':
            ofn = optarg;
            break;

        case 'P':
            //
            // The p-machine release dictates the opcode set available,
            // it also affects the shape of the codefile.
            //
            p_machine_set(optarg);
            break;

        case 'V':
            version_print();
            return 0;

        default:
            usage();
        }
    }
    if (optind >= argc)
        usage();

    //
    // Open the output file.
    //
    output::pointer os = output::factory(ofn);

    //
    // Process each code file we are given on the command line.
    //
    while (optind < argc)
    {
        rcstring code_file_name = argv[optind++];
        codefile::pointer cfp = codefile_file::create(code_file_name);
        cfp->disassemble(os, flags);
    }

    //
    // Make sure everything is written out.
    //
    os.reset();
    return 0;
}


// vim: set ts=8 sw=4 et :
