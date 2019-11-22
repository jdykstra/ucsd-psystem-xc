//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#include <lib/bitmap/invert.h>
#include <lib/bitmap/raw.h>
#include <lib/bitmap/png.h>
#include <lib/version.h>


static bool invert;
static unsigned width;
static unsigned height;


static void
pedantic_size_warning(const char *filename)
{
    if (width != 320 || height != 240)
    {
        explain_output_warning
        (
            "%s: the image --size=%dx%d is not strictly correct, "
                "UCSD Pascal foto files are of --size=320x240",
            filename,
            width,
            height
        );
    }
}


static void
foto_to_png(const char *ifn, const char *ofn)
{
    bitmap::pointer bm1 = bitmap_raw::create(ifn);
    if (width != 0)
        bm1->set_size_hint(width, height);
    if (invert)
        bm1 = bitmap_invert::create(bm1);
    unsigned char *data = 0;
    bm1->read(width, height, data);

    pedantic_size_warning(ifn);

    bitmap::pointer bm2 = bitmap_png::create(ofn);
    bm2->write(width, height, data);
    delete [] data;
}


static void
png_to_foto(const char *ifn, const char *ofn)
{
    bitmap::pointer bm1 = bitmap_png::create(ifn);
    if (width != 0)
        bm1->set_size_hint(width, height);
    if (invert)
        bm1 = bitmap_invert::create(bm1);
    unsigned char *data = 0;
    bm1->read(width, height, data);

    pedantic_size_warning(ifn);

    bitmap::pointer bm2 = bitmap_raw::create(ofn);
    bm2->write(width, height, data);
    delete [] data;
}


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] -d <foto> <png>\n", prog);
    fprintf(stderr, "Usage: %s [ <option>... ] -e <png> <foto>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    explain_option_hanging_indent_set(4);
    char action = 0;
    for (;;)
    {
        static const struct option options[] =
        {
            { "decode", 0, 0, 'd' },
            { "encode", 0, 0, 'e' },
            { "invert", 0, 0, 'i' },
            { "size", 1, 0, 's' },
            { 0, 0, 0, 0 }
        };

        int c = getopt_long(argc, argv, "deis:V", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'd':
            // decode: foto => png
            action = 'd';
            break;

        case 'e':
            // encode: png => foto
            action = 'e';
            break;

        case 'i':
            invert = !invert;
            break;

        case 's':
            // should be of the form %dx%d
            {
                int w = 0;
                int h = 0;
                if (2 != sscanf(optarg, "%dx%d", &w, &h))
                {
                    explain_output_error_and_die
                    (
                        "the string %s does not look like a size",
                        rcstring(optarg).quote_c().c_str()
                    );
                }
                if (w & 7)
                {
                    explain_output_error_and_die
                    (
                        "the width (%d) is not a multiple of 8 (r=%d)",
                        w,
                        (w & 7)
                    );
                }
                if (w < 8 || h < 1)
                {
                    explain_output_error_and_die
                    (
                        "the --size=%dx%d is too small",
                        w,
                        h
                    );
                }
                if (w * h >= 65536 * 8)
                {
                    explain_output_error_and_die
                    (
                        "the --size=%dx%d is too large for the image to "
                            "fit in a UCSD Pascal system's memory; aborting",
                        w,
                        h
                    );
                }
                width = w;
                height = h;
            }
            break;

        case 'V':
            version_print();
            return 0;

        default:
            usage();
        }
    }
    if (!action)
    {
        explain_output_error
        (
            "you must specify either --encode or --decode on the "
            "command line"
        );
        usage();
    }
    if (optind + 2 != argc)
    {
        explain_output_error
        (
            "you must name both an input file and an output file on the "
            "command line"
        );
        usage();
    }
    if (action == 'd')
        foto_to_png(argv[optind], argv[optind + 1]);
    else
        png_to_foto(argv[optind], argv[optind + 1]);
    return 0;
}


// vim: set ts=8 sw=4 et :
