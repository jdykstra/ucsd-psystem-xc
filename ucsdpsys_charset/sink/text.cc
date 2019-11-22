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
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>

#include <ucsdpsys_charset/pretty_char_name.h>
#include <ucsdpsys_charset/sink/text.h>


sink_text::~sink_text()
{
    explain_fclose_or_die(fp);
    fp = 0;
}


sink_text::sink_text(FILE *a_fp) :
    fp(a_fp)
{
    assert(fp);
}


sink_text::pointer
sink_text::create(const rcstring &filename)
{
    FILE *fp =
        (
            looks_like_a_stdout_synonym(filename)
        ?
            stdout
        :
            explain_fopen_or_die(filename.c_str(), "w")
        );
    return pointer(new sink_text(fp));
}


bool
sink_text::candidate(const rcstring &filename)
{
    return (filename.ends_with_nc(".text") || filename.ends_with_nc(".txt"));
}


void
sink_text::write_whole_font_begin(void)
{
    rewind(fp);
    print_decode_header(fp);
}


void
sink_text::print_bits(const unsigned char *data, size_t data_size_in_bits)
{
    fputs("    \"", fp);
    for (unsigned x = 0; x < data_size_in_bits; ++x)
    {
        const unsigned char *byte = data + (x >> 3);
        unsigned bit = 1 << (x & 7);
        char c = (*byte & bit) ? 'X' : ' ';
        fputc(c, fp);
    }
    fputs("\",\n", fp);
}


void
sink_text::write_one_glyph(const glyph::pointer &gp)
{
    if (!gp)
        return;

    //
    // Don't print anything at all if the glyph is entirely zero.
    // This makes some font definition files very much smaller.
    //
    if (gp->all_zero())
        return;

    //
    // Print the character number, as pretty as possible.
    //
    unsigned char c = gp->get_number();
    fprintf(fp, "\n# %s\n", unicode_char_name(c).c_str());
    fprintf(fp, "%s = {\n", pretty_char_name(c).c_str());

    //
    // Print each row, as a C string.
    //
    unsigned width = gp->get_width();
    unsigned height = gp->get_height();
    for (unsigned y = 0; y < height; ++y)
    {
        print_bits(gp->get_row(y), width);
    }
    fputs("};\n", fp);
}


void
sink_text::write_whole_font_end(void)
{
    explain_fflush_or_die(fp);
}


// vim: set ts=8 sw=4 et :
