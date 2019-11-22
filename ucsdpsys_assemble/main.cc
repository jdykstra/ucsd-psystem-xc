//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#include <lib/ac/stdio.h>
#include <lib/ac/getopt.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <unistd.h>

#include <lib/debug.h>
#include <lib/output/null.h>
#include <lib/output/file.h>
#include <lib/output/filter/heading.h>
#include <lib/p_machine.h>
#include <lib/segfault.h>
#include <lib/version.h>

#include <ucsdpsys_assemble/assembler.h>
#include <ucsdpsys_assemble/lex.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <filename>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    set_segfault_handler();
    rcstring code_file;
    const char *architecture = 0;
    const char *host = 0;
    output::pointer list_file;
    for (;;)
    {
        static const struct option options[] =
        {
            { "architecture", 1, 0, 'A' },
            { "debug", 0, 0, 'd' },
            { "grammar-debug", 0, 0, 'y' },
            { "host", 1, 0, 'H' },
            { "include", 1, 0, 'I' },
            { "listing", 1, 0, 'L' },
            { "view-path", 1, 0, 'J' },
            { "machine", 1, 0, 'A' },
            { "output", 1, 0, 'o' },
            { "p-machine", 1, 0, 'P' },
            { "version", 0, 0, 'V' },
            { 0, 0, 0, 0 }
        };
        int c = getopt_long(argc, argv, "A:dH:I:J:L:o:P:Vy", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'A':
            architecture = optarg;
            break;

        case 'd':
            ++debug_level;
            break;

        case 'H':
            host = optarg;
            break;

        case 'I':
            lex_include_path(optarg);
            break;

        case 'J':
            lex_view_path(optarg);
            break;

        case 'L':
            list_file = output_file::create(optarg);
            list_file = output_filter_heading::create(list_file);
            break;

        case 'o':
            code_file = optarg;
            break;

        case 'P':
            //
            // The release of the p-machine changes the shape of the codefile.
            //
            p_machine_set(optarg);
            break;

        case 'V':
            version_print();
            return 0;

        case 'y':
            grammar_debug = true;
            break;

        default:
            usage();
        }
    }
    if (optind + 1 != argc)
        usage();
    rcstring source_file(argv[optind]);
    if (!list_file)
        list_file = output_null::create();
    list_file->set_heading1("Assembler Listing: " + source_file);

    // Default the code file's name if the user did not specify one,
    // using the UCSD p-system's file name convention.
    if (code_file.empty())
        code_file = source_file.trim_suffix() + ".code";

    assembler manuel(code_file, list_file);
    if (host)
        manuel.set_host(host);
    if (architecture)
        manuel.set_architecture(architecture);
    manuel.parse(source_file);
    manuel.write_code_file();

    return 0;
}


// vim: set ts=8 sw=4 et :
