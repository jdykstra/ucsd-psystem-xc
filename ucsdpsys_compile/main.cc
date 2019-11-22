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
#include <lib/ac/stdlib.h>
#include <lib/ac/string.h>
#include <lib/ac/getopt.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <unistd.h>

#include <lib/debug.h>
#include <lib/discolumns.h>
#include <lib/pascal/lex.h>
#include <lib/p_machine.h>
#include <lib/rcstring.h>
#include <lib/segfault.h>
#include <lib/time_value.h>
#include <lib/version.h>

#include <ucsdpsys_compile/translator/compile.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <input-file-name>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


static rcstring
pretty_quantity(double x)
{
    const char *prefix[] = { "", "k", "M", "G", "T", "P", "E" };
    int pfxidx = 0;
    while (pfxidx < 7 && x >= 1000.0)
    {
        ++pfxidx;
        x /= 1000.0;
    }
    if (x < 10)
        return rcstring::printf("%4.2f%s", x, prefix[pfxidx]);
    if (x < 100)
        return rcstring::printf("%4.1f%s", x, prefix[pfxidx]);
    return rcstring::printf("%3.0f%s", x, prefix[pfxidx]);
}


int
main(int argc, char **argv)
{
    set_segfault_handler();
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    time_value start;
    start.get_time_of_day();
    rcstring output_file_name;
    bool grammar_debug = false;
    bool verbose = false;
    rcstring_list warnings;
    rcstring listing_file_name;
    for (;;)
    {
        static struct option options[] =
        {
            { "architecture", 1, 0, 'A' },
            { "debug", 0, 0, 'd' },
            { "feature", 1, 0, 'f' },
            { "grammar-trace", 0, 0, 'y' },
            { "host", 1, 0, 'H' },
            { "include", 1, 0, 'I' },
            { "view-path", 1, 0, 'J' },
            { "library", 1, 0, 'L' },
            { "listing", 1, 0, 'M' },
            { "long-addresses", 0, 0, 'l' }, // affects disassembler
            { "machine", 1, 0, 'A' },
            { "output", 1, 0, 'o' },
            { "p-machine", 1, 0, 'P' },
            { "verbose", 0, 0, 'v' },
            { "version", 0, 0, 'V' },
            { "warning", 0, 0, 'W' },
            { 0, 0, 0, 0 }
        };
        int c = getopt_long(argc, argv, "A:df:H:I:J:L:lM:o:VvW:y", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'A':
            warnings.push_back(rcstring::printf("$feature arch %s", optarg));
            break;

        case 'd':
            ++debug_level;
            break;

        case 'f':
            {
                const char *eq = strchr(optarg, '=');
                if (eq)
                {
                    rcstring name(optarg, eq - optarg);
                    rcstring value(eq + 1);
                    warnings.push_back
                    (
                        rcstring::printf
                        (
                            "$feature %s %s",
                            name.c_str(),
                            value.c_str()
                        )
                    );
                }
                else
                {
                    rcstring name = optarg;
                    rcstring value = "true";
                    if (name.downcase().substring(0, 3) == "no-")
                    {
                        name = name.substring(3, name.size() - 3);
                        value = "false";
                    }
                    warnings.push_back("$feature " + name + " " + value);
                }
                warnings.push_back(optarg);
            }
            break;

        case 'H':
            warnings.push_back(rcstring::printf("$feature host %s", optarg));
            break;

        case 'I':
            pascal_lex_include_directory(optarg);
            break;

        case 'J':
            pascal_lex_viewpath(optarg);
            break;

        case 'L':
            translator::library_path(optarg);
            break;

        case 'l':
            // When disassembling, instead of a hex address, use the
            // segment number, the procedure number, and the address, all in
            // decimal.  Otherwise it uses a simple hex segment offset.
            discolumns::long_address_format = true;
            break;

        case 'M':
            listing_file_name = (optarg && *optarg ? optarg : "-");
            break;

        case 'o':
            output_file_name = optarg;
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

        case 'v':
            verbose = true;
            break;

        case 'W':
            {
                const char *eq = strchr(optarg, '=');
                if (eq)
                {
                    rcstring name(optarg, eq - optarg);
                    rcstring value(eq + 1);
                    warnings.push_back
                    (
                        rcstring::printf
                        (
                            "$warning %s %s",
                            name.c_str(),
                            value.c_str()
                        )
                    );
                }
                else
                {
                    rcstring name = optarg;
                    rcstring value = "true";
                    if (name.downcase().substring(0, 3) == "no-")
                    {
                        name = name.substring(3, name.size() - 3);
                        value = "false";
                    }
                    warnings.push_back("$warning " + name + " " + value);
                }
                warnings.push_back(optarg);
            }
            break;

        case 'y':
            grammar_debug = true;
            break;

        default:
            usage();
            // NOTREACHED
        }
    }
    if (optind + 1 != argc)
        usage();
    rcstring input_file_name(argv[optind]);

    //
    // Default the output file name, if necessary.
    //
    if (output_file_name.empty())
        output_file_name = input_file_name.trim_suffix() + ".code";
    translator_compile code(output_file_name);

    if (!listing_file_name.empty())
        code.listing_to_file(listing_file_name);

    // set the selected warnings
    for (size_t j = 0; j < warnings.size(); ++j)
        code.comment(warnings[j]);

    //
    // Parse the Pascal file and print it out again as Pascal.
    //
    if (grammar_debug)
        code.enable_grammar_debug();
    code.parse(input_file_name);

    //
    // Write out the code file.
    //
    code.traversal();

    if (verbose)
    {
        time_value finish;
        finish.get_time_of_day();
        finish -= start;
        if (finish <= time_value(0))
            finish = time_value(1);
        double lps = pascal_lex_number_of_lines() / double(finish);
        explain_output_error
        (
            "%s lines per second",
            pretty_quantity(lps).c_str()
        );
    }
    return 0;
}


// vim: set ts=8 sw=4 et :
