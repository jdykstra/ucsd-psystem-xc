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
#include <lib/ac/stdlib.h>
#include <lib/ac/getopt.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>

#include <lib/mtype.h>
#include <lib/version.h>

#include <ucsdpsys_osmakgen/collect.h>
#include <ucsdpsys_osmakgen/target.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <filename>...\n", prog);
    fprintf(stderr, "Usage: %s --arch-from-host <host>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


static const struct option options[] =
{
    { "architecture", 1, 0, 'A' },
    { "arch-from-host", 1, 0, 'a' },
    { "no-blurb", 0, 0, 'b' },
    { "change", 1, 0, 'c' },
    { "copyright", 1, 0, 'C' },
    { "host", 1, 0, 'H' },
    { "output", 1, 0, 'o' },
    { "project", 1, 0, 'p' },
    { "revision", 1, 0, 'v' },
    { "version", 0, 0, 'V' },
    { 0, 0, 0, 0 }
};


static void
use_output_first(int val)
{
    for (const struct option *op = options; op->name; ++op)
    {
        if (op->val == val)
        {
            explain_output_error_and_die
            (
                "you must use the --output option before you may use "
                    "the --%s option",
                op->name
            );
        }
    }
    explain_output_error_and_die
    (
        "you must use the -o option before you may use the -%c option",
        val
    );
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    rcstring project_name;
    rcstring change_number;
    target::pointer tp;
    for (;;)
    {
        int c = getopt_long(argc, argv, "A:a:bC:c:H:o:p:T:Vv:", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'A':
            if (!tp)
                use_output_first(c);
            tp->set_default_arch(rcstring(optarg).downcase());
            break;

        case 'a':
            // arch from host
            {
                mtype_t mtype = mtype_from_host(optarg);
                if (mtype == mtype_undefined)
                {
                    explain_output_error_and_die
                    (
                        "host %s unknown",
                        rcstring(optarg).quote_c().c_str()
                    );
                }
                printf("%s\n", mtype_name(mtype).downcase().c_str());
            }
            return 0;

        case 'b':
            if (!tp)
                use_output_first(c);
            tp->no_blurb();
            break;

        case 'C':
            if (!tp)
                use_output_first(c);
            tp->set_notice(optarg);
            break;

        case 'c':
            change_number = optarg;
            break;

        case 'H':
            if (!tp)
                use_output_first(c);
            tp->set_default_host(rcstring(optarg).downcase());
            break;

        case 'o':
            if (tp)
                explain_output_error_and_die("too many --output options");
            tp = target::factory(optarg);
            break;

        case 'p':
            project_name = optarg;
            break;

        case 'V':
            version_print();
            return 0;

        case 'v':
            if (!tp)
                use_output_first(c);
            tp->set_version(optarg);
            break;

        default:
            usage();
        }
    }
    rcstring_list file_manifest;
    while (optind < argc)
        file_manifest.push_back(argv[optind++]);

    //
    // Read the file manifest from Aegis.
    // (This could be generalized for other DVCS.)
    //
    if (!project_name.empty() || !change_number.empty())
    {
        rcstring command = "aelsf";
        if (!project_name.empty())
            command += " --project=" + project_name.quote_shell();
        if (!change_number.empty())
            command += " --change=" + change_number.quote_shell();
        collect_output_of(command, file_manifest);
    }
    if (file_manifest.empty())
    {
        fprintf(stderr, "%s: no files named\n", argv[0]);
        exit(1);
    }

    // For predictable output for automated tests,
    // plus it helps find things in the output.
    file_manifest.sort();

    if (!tp)
        tp = target::factory("Makefile");
    tp->generate(file_manifest);

    // Flush all the output, and complain if something goes wrong.
    tp.reset();

    return 0;
}


// vim: set ts=8 sw=4 et :
