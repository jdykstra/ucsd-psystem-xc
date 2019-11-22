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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#include <lib/ac/ctype.h>

#include <lib/rcstring.h>
#include <lib/output/text_decode.h>


output_text_decode::~output_text_decode()
{
    //
    // Make sure all buffered data has been passed to our write_inner
    // method.
    //
    flush();
}


output_text_decode::output_text_decode(const output::pointer &a_deeper,
        bool a_use_tabs) :
    deeper(a_deeper),
    column(0),
    non_white(false),
    start_of_file(1024),
    dle_seen(false),
    use_tabs(a_use_tabs)
{
}


static inline bool
is_text_character(unsigned char c)
{
    return (isprint(c) || isspace(c) || c == 16);
}


static bool
is_text_buffer(const unsigned char *buffer, size_t size)
{
    for (size_t j = 0; j < 16 && j < size; ++j)
        if (!is_text_character(buffer[j]))
            return false;
    return true;
}


void
output_text_decode::write_inner(const void *vdata, size_t size)
{
    const unsigned char *data = (const unsigned char *)vdata;

    //
    // we have to cope with the case where the strat of the file could
    // be 1KB of binary data used by the text editor.  But sometimes,
    // this header has been stripped off the file (especially if we are
    // called twice for the same file).
    //
    if (start_of_file == 1024 && is_text_buffer(data, size))
        start_of_file = 0;
    while (size > 0 && start_of_file > 0)
    {
        ++data;
        --size;
        --start_of_file;
    }

    //
    // Process the text.
    //
    // Discard NUL characters.  They should only appear at the end of a
    // 1KB block, but we will discard all that we see.
    //
    // Expand DLE sequences.  They should only appear at the start of
    // lines, but we will expand all that we see.
    //
    // Turn CR line terminators into NL line terminators, especially is
    // the stupid humans call us twice for the same file.
    //
    while (size > 0)
    {
        unsigned char c = *data++;
        --size;
        if (dle_seen)
        {
            dle_seen = false;
            if (c < 32)
            {
                --data;
                ++size;
                c = 16;
                goto normal;
            }
            c -= 32;
            if (non_white)
            {
                // Insert spaces here.
                // We only insert tabs at the start of the line.
                for (unsigned x = 0; x < c; ++x)
                    deeper->fputc(' ');
            }
            column += c;
            continue;
        }
        switch (c)
        {
        case '\0':
            // ignore
            break;

        case '\r':
        case '\n':
            deeper->fputc('\n');
            column = 0;
            non_white = false;
            break;

        case 16:
            dle_seen = true;
            break;

        default:
            normal:
            if (!non_white)
            {
                int ocol = 0;
                if (use_tabs)
                {
                    for (;;)
                    {
                        if (ocol + 1 == column)
                            break;
                        int ocol2 = (ocol + 8) & ~7;
                        if (ocol2 > column)
                            break;
                        deeper->fputc('\t');
                        ocol = ocol2;
                    }
                }
                while (ocol < column)
                {
                    deeper->fputc(' ');
                    ++ocol;
                }
                non_white = true;
            }
            deeper->fputc(c);
            ++column;
            break;
        }
    }
}


void
output_text_decode::flush_inner()
{
    deeper->flush();
}


rcstring
output_text_decode::filename()
{
    return deeper->filename();
}


void
output_text_decode::utime(struct utimbuf &utb)
{
    deeper->utime(utb);
}


// vim: set ts=8 sw=4 et :
