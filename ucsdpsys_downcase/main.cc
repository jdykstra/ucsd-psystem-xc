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

#include <lib/ac/ctype.h>
#include <lib/ac/stdio.h>
#include <lib/ac/getopt.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/getc.h>
#include <libexplain/output.h>
#include <libexplain/putc.h>
#include <libexplain/program_name.h>
#include <libexplain/rename.h>
#include <unistd.h>

#include <lib/codefile.h>
#include <lib/output.h>
#include <lib/rcstring.h>
#include <lib/version.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <filename>...\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


static void
convert(FILE *ifp, FILE *ofp)
{
    for (;;)
    {
        int c = explain_getc_or_die(ifp);
        if (c == EOF)
            break;
        switch (c)
        {
        case '{':
            // comment
            explain_putc_or_die(c, ofp);
            for (;;)
            {
                c = explain_getc_or_die(ifp);
                if (c == EOF)
                    break;
                explain_putc_or_die(c, ofp);
                if (c == '}')
                    break;
            }
            break;

        case '(':
            explain_putc_or_die(c, ofp);
            c = explain_getc_or_die(ifp);
            if (c != '*')
            {
                if (c != EOF)
                    ungetc(c, ifp);
                break;
            }
            explain_putc_or_die(c, ofp);

            // comment
            for (;;)
            {
                c = explain_getc_or_die(ifp);
                if (c == EOF)
                    break;
                explain_putc_or_die(c, ofp);
                if (c == '*')
                {
                    c = explain_getc_or_die(ifp);
                    if (c == EOF)
                        break;
                    if (c == ')')
                    {
                        explain_putc_or_die(c, ofp);
                        break;
                    }
                    ungetc(c, ifp);
                }
            }
            break;

        case '\'':
            // string constant
            explain_putc_or_die(c, ofp);
            for (;;)
            {
                c = explain_getc_or_die(ifp);
                if (c == EOF)
                    break;
                explain_putc_or_die(c, ofp);
                if (c == '\'')
                {
                    c = explain_getc_or_die(ifp);
                    if (c != '\'')
                    {
                        if (c != EOF)
                            ungetc(c, ifp);
                        break;
                    }
                    explain_putc_or_die(c, ofp);
                }
            }
            break;

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            c = tolower(c);
            // fall through...

        default:
            explain_putc_or_die(c, ofp);
            break;
        }
    }
}


static void
convert(const rcstring &ifn)
{
    rcstring ofn = ifn + ".new";
    FILE  *ifp = explain_fopen_or_die(ifn.c_str(), "r");
    FILE  *ofp = explain_fopen_or_die(ofn.c_str(), "w");
    convert(ifp, ofp);
    explain_fclose_or_die(ofp);
    explain_fclose_or_die(ifp);

    explain_rename_or_die(ofn.c_str(), ifn.c_str());
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    for (;;)
    {
        static const struct option options[] =
        {
            { "version", 0, 0, 'V' },
            { 0, 0, 0, 0 }
        };
        int c = getopt_long(argc, argv, "V", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'V':
            version_print();
            return 0;

        default:
            usage();
        }
    }
    if (optind >= argc)
    {
        convert(stdin, stdout);
        explain_fflush_or_die(stdout);
    }
    else
    {
        for (; optind < argc; ++optind)
            convert(argv[optind]);
    }
    return 0;
}


// vim: set ts=8 sw=4 et :
