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

#include <lib/ac/stdio.h>
#include <libexplain/fclose.h>
#include <libexplain/fopen.h>
#include <libexplain/fputc.h>

#include <lib/sizeof.h>

#include <ucsdpsys_charset/sink/psftools.h>
#include <ucsdpsys_charset/pretty_char_name.h>


sink_psftools::~sink_psftools()
{
}


sink_psftools::sink_psftools(const rcstring &a_filename) :
    filename(a_filename)
{
}


sink_psftools::pointer
sink_psftools::create(const rcstring &a_filename)
{
    return pointer(new sink_psftools(a_filename));
}


bool
sink_psftools::candidate(const rcstring &fn)
{
    return (fn.ends_with(".text") || fn.ends_with(".txt"));
}


void
sink_psftools::write_one_glyph(const glyph::pointer &gp)
{
    unsigned n = gp->get_number();
    if (n < SIZEOF(glyphs))
        glyphs[n] = gp;
}


void
sink_psftools::write_whole_font_end(void)
{
    FILE *fp =
        (
            looks_like_a_stdout_synonym(filename)
        ?
            stdout
        :
            explain_fopen_or_die(filename.c_str(), "w")
        );

    fprintf(fp, "%%PSF2\n");
    fprintf(fp, "Version: 0\n");
    fprintf(fp, "Flags: 1\n");

    unsigned width = 1;
    unsigned height = 1;
    {
        unsigned length = 0;
        for (unsigned j = 0; j < 256; ++j)
        {
            glyph::pointer gp = glyphs[j];
            if (gp)
            {
                if (width < gp->get_width())
                    width = gp->get_width();
                if (height < gp->get_height())
                    height = gp->get_height();
                ++length;
            }
        }

        // This is why we defer writing until the very end.
        fprintf(fp, "Length: %u\n", length);
    }

    fprintf(fp, "Width: %u\n", width);
    fprintf(fp, "Height: %u\n", height);

    unsigned char_num = 0;
    for (unsigned j = 0; j < 256; ++j)
    {
        glyph::pointer gp = glyphs[j];
        if (gp)
        {
            gp = gp->crop(width, height);
            fprintf(fp, "%%\n");
            fprintf(fp, "// Character %u\n", char_num++);
            fprintf
            (
                fp,
                "// %s, %s\n",
                gp->get_number_pretty().c_str(),
                unicode_char_name(gp->get_number()).c_str()
            );
            fprintf(fp, "Bitmap: ");
            for (unsigned y = 0; y < height; ++y)
            {
                if (y)
                    fprintf(fp, " \\\n        ");
                const unsigned char *p = gp->get_row(y);
                for (unsigned x = 0; x < width; ++x)
                {
                    bool pixel = (0 != (p[x >> 3] & (1 << (x & 7))));
                    char c = (pixel ? '#' : '-');
                    explain_fputc_or_die(c, fp);
                }
            }
            fprintf(fp, "\nUnicode: [%04X];\n", gp->get_number());
        }
    }
    explain_fclose_or_die(fp);
}


// vim: set ts=8 sw=4 et :
