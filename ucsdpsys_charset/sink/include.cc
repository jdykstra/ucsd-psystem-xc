//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#include <lib/ac/assert.h>
#include <lib/ac/string.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>

#include <lib/get_filename.h>

#include <ucsdpsys_charset/pretty_char_name.h>
#include <ucsdpsys_charset/sink/include.h>


sink_include::~sink_include()
{
    explain_fclose_or_die(fp);
    fp = 0;
}


sink_include::sink_include(FILE *a_fp) :
    fp(a_fp)
{
    assert(fp);
}


sink_include::pointer
sink_include::create(const rcstring &filename)
{
    FILE *fp =
        (
            looks_like_a_stdout_synonym(filename)
        ?
            stdout
        :
            explain_fopen_or_die(filename.c_str(), "w")
        );
    return pointer(new sink_include(fp));
}


bool
sink_include::candidate(const rcstring &filename)
{
    return filename.ends_with(".h");
}


void
sink_include::write_whole_font_begin(void)
{
    rewind(fp);
    memset(used, 0, sizeof(used));
    print_decode_header(fp);
}


void
sink_include::write_one_glyph(const glyph::pointer &gp)
{
    if (!gp)
        return;
    if (gp->all_zero())
        return;
    unsigned row_bytes = gp->get_row_bytes();
    unsigned height = gp->get_height();

    used[gp->get_number()] = 1;
    fprintf
    (
        fp,
        "\n\n/* %s => %s */\n\n",
        gp->get_number_pretty().c_str(),
        unicode_char_name(gp->get_number()).c_str()
    );
    fprintf(fp, "static glyph::pointer\n");
    fprintf(fp, "glyph_%02X(void)\n", gp->get_number());
    fprintf(fp, "{\n");
    fprintf(fp, "    static const unsigned char data[] =\n");
    fprintf(fp, "    {\n");
    for (unsigned row = 0; row < height; ++row)
    {
        const unsigned char *data = gp->get_row(row);
        for (unsigned x = 0; x < row_bytes; ++x)
        {
            if (x)
                fprintf(fp, " ");
            else
                fprintf(fp, "        ");
            fprintf(fp, "0x%02X,", data[x]);
        }
        fprintf(fp, "\n");
    }
    fprintf(fp, "    };\n");
    fprintf
    (
        fp,
        "    return glyph::create(%d, %d, %d, data);\n",
        gp->get_number(),
        gp->get_width(),
        gp->get_height()
    );
    fprintf(fp, "}\n");
}


void
sink_include::write_whole_font_end(void)
{
    rcstring filename = filename_from_stream(fp);

    fprintf(fp, "\n");
    fprintf(fp, "typedef glyph::pointer (*create_t)(void);\n");
    fprintf(fp, "\n");
    fprintf(fp, "static const create_t create[256] =\n");
    fprintf(fp, "{\n");
    for (unsigned c = 0; c < 256; ++c)
    {
        fprintf(fp, "    ");
        if (used[c])
        {
            fprintf
            (
                fp,
                "glyph_%02X, /* %s, %s */\n",
                c,
                pretty_char_name(c).c_str(),
                unicode_char_name(c).c_str()
            );
        }
        else
        {
            fprintf(fp, "NULL,     /* %s */\n", pretty_char_name(c).c_str());
        }
    }
    fprintf(fp, "};\n");

    fprintf(fp, "\n");
    fprintf(fp, "glyph::pointer\n");
    fprintf
    (
        fp,
        "%s_glyph_create(int c)\n",
        filename.basename().trim_suffix().identifier().downcase().c_str()
    );
    fprintf(fp, "{\n");
    fprintf(fp, "    if (c < 0 || c >= 256 || !create[c])\n");
    fprintf(fp, "        return glyph::pointer();\n");
    fprintf(fp, "    return create[c]();\n");
    fprintf(fp, "}\n");

    explain_fflush_or_die(fp);
}


// vim: set ts=8 sw=4 et :
