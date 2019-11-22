//
// UCSD p-System filesystem in user space
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

#include <lib/ac/ctype.h>
#include <lib/ac/stdio.h>
#include <lib/ac/stdlib.h>
#include <lib/ac/getopt.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <unistd.h>

#include <lib/version.h>

#include <ucsdpsys_charset/arch.h>
#include <ucsdpsys_charset/font.h>
#include <ucsdpsys_charset/sink.h>
#include <ucsdpsys_charset/sink/add_boot_logo.h>
#include <ucsdpsys_charset/source.h>
#include <ucsdpsys_charset/source/add_boot_logo.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s -d [ <infile> [ <outfile> ]]\n", prog);
    fprintf(stderr, "       %s -e [ <infile> [ <outfile> ]]\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    arch::pointer ap;
    char action = 0;
    rcstring input_format_name;
    rcstring output_format_name;
    bool invert_top_half = false;

    for (;;)
    {
        enum
        {
            INPUT_FORMAT = 256,
            OUTPUT_FORMAT,
        };
        static const struct option options[] =
        {
            { "architecture", 1, 0, 'A' },
            { "decode", 0, 0, 'd' },
            { "encode", 0, 0, 'e' },
            { "host", 1, 0, 'H' },
            { "machine", 1, 0, 'A' },
            { "negative", 0, 0, 'n' },
            { "invert-top-half", 0, 0, 'n' },
            { "version", 0, 0, 'V' },
            { "input-format", 1, 0, INPUT_FORMAT },
            { "output-format", 1, 0, OUTPUT_FORMAT },
            { 0, 0, 0, 0 }
        };

        int c = getopt_long(argc, argv, "A:DdEeH:inV", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'A':
            if (ap)
                usage();
            ap = arch::factory_by_arch(optarg);
            break;

        case 'd':
            // decode: binary to text
            action = c;
            break;

        case 'e':
            // encode: text to binary
            action = c;
            break;

        case 'H':
            if (ap)
                usage();
            ap = arch::factory_by_host(optarg);
            break;

        case 'i':
            // text to (text) include file
            action = 'e';
            break;

        case 'n':
            invert_top_half = true;
            break;

        case 'V':
            version_print();
            return 0;

        case INPUT_FORMAT:
            input_format_name = optarg;
            break;

        case OUTPUT_FORMAT:
            output_format_name = optarg;
            break;

        default:
            usage();
        }
    }
    if (!ap)
        ap = arch::factory_by_host("klebsch");
    if (!action)
    {
        explain_output_error_and_die
        (
            "you must specify either --encode or --decode on the command line"
        );
    }

    rcstring ifn = "-";
    rcstring ofn = "-";
    rcstring png_file;
    switch (argc - optind)
    {
    case 0:
        break;

    case 3:
        png_file = argv[optind + 2];
        // Fall through...

    case 2:
        ofn = argv[optind + 1];
        // Fall through...

    case 1:
        ifn = argv[optind];
        break;

    default:
        usage();
    }

    //
    // Fill in some defaults, based on the architectured specified on
    // the command line.
    //
    if (input_format_name.empty())
    {
        if (ifn.ends_with(".text"))
            input_format_name = ap->get_text_format_name();
        else if (ifn.ends_with(".charset"))
            input_format_name = ap->get_binary_format_name();
    }
    if (output_format_name.empty())
    {
        if (ofn.ends_with(".text"))
            output_format_name = ap->get_text_format_name();
        else if (ofn.ends_with(".charset"))
            output_format_name = ap->get_binary_format_name();
    }

    //
    // read the font
    //
    source::pointer ifp =
        (
            input_format_name.empty()
        ?
            source::factory(ifn)
        :
            source::factory_by_name(input_format_name, ifn)
        );
    switch (action)
    {
    case 'e':
        if (ifp->is_binary() && input_format_name.empty())
        {
            explain_output_error_and_die
            (
                "the --encode option looks incorrect in this context"
            );
        }
        break;

    case 'd':
        if (!ifp->is_binary() && output_format_name.empty())
        {
            explain_output_error_and_die
            (
                "the --decode option looks incorrect in this context"
            );
        }
        break;
    }
    font::pointer data = ifp->read_whole_font();

    if (invert_top_half)
        data = data->invert_top_half();

    //
    // write the font
    //
    sink::pointer ofp =
        (
            output_format_name.empty()
        ?
            sink::factory(ofn)
        :
            sink::factory_by_name(output_format_name, ofn)
        );
    ofp->write_whole_font(data);

    //
    // muck about with the icon file
    //
    if (!png_file.empty())
    {
        source::pointer ifp2 = ifp;
        if (action == 'e')
            ifp2 = source_add_boot_logo::create(ifp2, png_file);

        sink::pointer ofp2 = ofp;
        if (action == 'd')
            ofp2 = sink_add_boot_logo::create(ofp2, png_file);

        unsigned char boot_logo[512];
        if (ifp2->get_boot_logo(boot_logo))
            ofp2->put_boot_logo(boot_logo);
    }

    ifp.reset();
    ofp.reset();

    return 0;
}


// vim: set ts=8 sw=4 et :
