//
// UCSD p-System operating system
// Copyright (C) 2006, 2010, 2012 Peter Miller
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
#include <lib/ac/string.h>
#include <lib/ac/stdlib.h>
#include <lib/ac/time.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/fread.h>
#include <libexplain/output.h>

#include <lib/endof.h>
#include <lib/rcstring.h>
#include <ucsdpsys_opcodes/decode.h>
#include <ucsdpsys_opcodes/bitflip.h>


static bool
stdio_name(const rcstring &fn)
{
    return (fn.empty() || fn == "-");
}

static const char *optype[] =
{
    "short",
    "one",
    "opt",
    "two",
    "lopt",
    "words",
    "chrs",
    "blk",
    "cmprss",
    "cmprss2",
    "word",
};


static void
print_table(const unsigned char *buffer, const rcstring &ofn)
{
    const unsigned char *tp = buffer + 8 * (256 - 52);
    FILE *ofp = stdout;
    if (!stdio_name(ofn))
    {
        ofp = explain_fopen_or_die(ofn.c_str(), "w");
    }

    for (size_t k = 0; k < 256; ++k)
    {
        const unsigned char *name = 0;
        if (k >= 52)
        {
            name = buffer + 8 * (k - 52);
            if (*name == ' ' || *name == 0)
                continue;
        }
        const unsigned char *ttp = tp + 2 * k;
        unsigned x = two_byte_decode(ttp);
        if (x >= SIZEOF(optype))
        {
            explain_output_error_and_die
            (
                "opcode %u: type %u out of range",
                (unsigned)k,
                x
            );
        }

        const char *tname = optype[x];
        fprintf(ofp, "%3u = %s", (unsigned)k, tname);
        if (name)
        {
            fprintf(ofp, "%-*s", int(9 - strlen(tname)), ",");

            const unsigned char *ep =
                (const unsigned char *)memchr(name, ' ', 8);
            if (!ep)
                ep = name + 8;
            fprintf(ofp, "\"%.*s\"", int(ep - name), name);
        }
        fprintf(ofp, ";\n");
    }
    explain_fflush_or_die(ofp);
    if (ofp != stdout)
        explain_fclose_or_die(ofp);
}


void
random_test_data(const rcstring &, const rcstring &ofn)
{
    srandom(time(0));

    size_t size = (256 - 52) * 8 + 256 * 2;
    unsigned char *buffer = new unsigned char [size];
    unsigned char *tp = buffer + 8 * (256 - 52);

    for (unsigned j = 0; j < 256; ++j)
    {
        unsigned char *p = tp + j * 2;
        two_byte_encode(p, random() % SIZEOF(optype));
        if (j >= 52)
        {
            p = buffer + 8 * (j - 52);
            memset(p, ' ', 8);
            unsigned len = random() % 5 + 3;
            for (unsigned k = 0; k < len; ++k)
                p[k] = 'A' + random() % 26;
        }
    }

    print_table(buffer, ofn);
    delete [] buffer;
}


void
decode(const rcstring &ifn, const rcstring &ofn)
{
    FILE *ifp = stdin;
    if (!stdio_name(ifn))
        ifp = explain_fopen_or_die(ifn.c_str(), "r");

    size_t size = (256 - 52) * 8 + 256 * 2;
    unsigned char *buffer = new unsigned char [size];
    size_t n = explain_fread_or_die(buffer, 1, size, ifp);
    if (n == 0)
    {
        explain_output_error_and_die
        (
            "read %s: expected %ld, but got %ld",
            ifn.quote_c().c_str(),
            (long)n,
            (long)size
        );
    }
    if (ifp != stdin)
        explain_fclose_or_die(ifp);

    print_table(buffer, ofn);
    delete [] buffer;
}


// vim: set ts=8 sw=4 et :
