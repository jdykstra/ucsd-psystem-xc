//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <unistd.h>

#include <lib/cardinal.h>
#include <lib/version.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ -aomtz ] <number>...\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    bool call_ordinal = false;
    int style = 0;
    for (;;)
    {
        int c = getopt(argc, argv, "amotVz");
        if (c < 0)
            break;
        switch (c)
        {
        case 'a':
            style |= cardinal_style_american;
            break;

        case 'm':
            style |= cardinal_style_ten;
            break;

        case 'o':
            call_ordinal = true;
            break;

        case 't':
            style |= cardinal_style_teen_hundred;
            break;

        case 'V':
            version_print();
            return 0;

        case 'z':
            style |= cardinal_style_zero_is_no;
            break;

        default:
            usage();
        }
    }
    if (optind == argc)
        usage();
    for (;;)
    {
        long n = atol(argv[optind++]);
        if (call_ordinal)
            printf("%s\n", ordinal(n, style).c_str());
        else
            printf("%s\n", cardinal(n, style).c_str());
        if (optind >= argc)
            break;
    }
    return 0;
}


// vim: set ts=8 sw=4 et :
