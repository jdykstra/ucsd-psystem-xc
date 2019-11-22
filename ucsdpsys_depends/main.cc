//
// UCSD p-System operating system
// Copyright (C) 2006, 2010, 2012 Peter Miller
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
#include <lib/ac/stdlib.h>
#include <lib/ac/getopt.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <unistd.h>

#include <lib/rcstring.h>
#include <lib/version.h>

#include <ucsdpsys_depends/language/assembler.h>
#include <ucsdpsys_depends/language/pascal.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ -o <outfile> ] <infile>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    rcstring outfile = "-";
    bool assembler = false;
    for (;;)
    {
        static const struct option options[] =
        {
            { "assembler", 0, 0, 'a' },
            { "include", 1, 0, 'I' },
            { "one-per-line", 0, 0, 'L' },
            { "output", 1, 0, 'o' },
            { "prefix", 1, 0, 'P' },
            { "recursive", 0, 0, 'r' },
            { "suffix", 1, 0, 'S' },
            { "version", 0, 0, 'V' },
            { "view-path", 1, 0, 'J' },
            { 0, 0, 0, 0 }
        };
        int c = getopt_long(argc, argv, "aI:J:Lo:P:rS:V", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'a':
            assembler = true;
            break;

        case 'I':
            language::depends_include_directory(optarg);
            break;

        case 'J':
            language::depends_view_directory(optarg);
            break;

        case 'L':
            language::depends_one_per_line();
            break;

        case 'o':
            outfile = optarg;
            break;

        case 'P':
            language::depends_prefix(optarg);
            break;

        case 'r':
            language::depends_recursive_set();
            break;

        case 'S':
            language::depends_suffix(optarg);
            break;

        case 'V':
            version_print();
            return 0;

        default:
            usage();
        }
    }
    if (argc - optind != 1)
        usage();
    rcstring infile = argv[optind];
    language::pointer lp =
        (
            assembler
        ?
            language_assembler::create()
        :
            language_pascal::create()
        );
    lp->depends(infile);
    lp->print_results(outfile);
    return 0;
}


// vim: set ts=8 sw=4 et :
