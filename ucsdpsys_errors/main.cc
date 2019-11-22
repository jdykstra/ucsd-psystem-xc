//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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

#include <lib/version.h>

#include <ucsdpsys_errors/arch.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <infile> <outfile>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    typedef void (arch::*method_t)(const rcstring &infile,
        const rcstring &outfile);
    method_t method = &arch::no_function_specified;
    arch::pointer ap;
    for (;;)
    {
        static const struct option options[] =
        {
            { "architecture", 1, 0, 'A' },
            { "decode", 0, 0, 'd' },
            { "encode", 0, 0, 'e' },
            { "host", 1, 0, 'H' },
            { "list", 0, 0, 'l' },
            { "machine", 1, 0, 'A' },
            { "version", 0, 0, 'V' },
            { 0, 0, 0, 0 }
        };
        int c = getopt_long(argc, argv, "A:deH:V", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'A':
            ap = arch::factory_by_arch(optarg);
            break;

        case 'd':
            method = &arch::decode;
            break;

        case 'e':
            method = &arch::encode;
            break;

        case 'H':
            ap = arch::factory_by_host(optarg);
            break;

        case 'l':
            arch::list();
            break;

        case 'V':
            version_print();
            return 0;

        default:
            usage();
        }
    }
    if (!ap)
        explain_output_error_and_die("no architecture specified");
    rcstring infile = "-";
    rcstring outfile = "-";
    switch (argc - optind)
    {
    case 2:
        outfile = argv[optind + 1];
        // Fall through...

    case 1:
        infile = argv[optind];
        break;

    case 0:
        break;

    default:
        usage();
    }

    ((*ap).*method)(infile, outfile);

    return 0;
}


// vim: set ts=8 sw=4 et :
