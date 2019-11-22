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

#include <lib/ac/assert.h>
#include <lib/ac/ctype.h>
#include <libexplain/output.h>

#include <lib/rcstring/accumulator.h>
#include <lib/output/text_encode.h>


output_text_encode::~output_text_encode()
{
    //
    // Make sure the buffered data has been given to our write_inner method.
    //
    flush();

    //
    // Make sure the last line is written out.
    //
    if (!line_accumulator.empty())
        write_one_line();

    //
    // Make sure the last block is NUL padded.
    //
    while (address & 1023)
    {
        deeper->fputc(0);
        ++address;
    }
}


output_text_encode::output_text_encode(const output::pointer &a_deeper,
        bool a_use_dle, bool a_nul_guarantee) :
    deeper(a_deeper),
    line_number(1),
    address(0),
    column(0),
    non_white(false),
    use_dle(a_use_dle),
    nul_guarantee(a_nul_guarantee)
{
    for (int j = 0; j < 1024; ++j)
        deeper->fputc(0);
}


void
output_text_encode::line_character(unsigned char c)
{
    switch (c)
    {
    case '\0':
        // Ignore
        break;

    case '\t':
        if (non_white)
        {
            for (;;)
            {
                line_accumulator.push_back(' ');
                ++column;
                if ((column & 7) == 0)
                    break;
            }
        }
        else
            column = (column + 8) & ~7;
        break;

    case 16:
        // Ignore.
        break;

    case ' ':
        if (non_white)
            line_accumulator.push_back(' ');
        ++column;
        break;

    default:
        if (!non_white)
        {
            int ocol = 0;
            if (use_dle)
            {
                while (ocol + (255 - 32) <= column)
                {
                    line_accumulator.push_back((char)16);
                    line_accumulator.push_back((char)255);
                    ocol += (255 - 32);
                }
            }
            switch (column - ocol)
            {
            case 0:
                break;

            case 2:
                line_accumulator.push_back(' ');
                // Fall through...

            case 1:
                line_accumulator.push_back(' ');
                break;

            default:
                if (use_dle)
                {
                    line_accumulator.push_back((char)16);
                    line_accumulator.push_back((char)(column - ocol + 32));
                }
                else
                {
                    while (ocol < column)
                    {
                        line_accumulator.push_back(' ');
                        ++ocol;
                    }
                }
                break;
            }
            non_white = true;
        }
        if (isprint(c))
            line_accumulator.push_back(c);
        else
        {
            line_accumulator.printf("\\x%02X", c);
            explain_output_warning
            (
                "%s: %d: unprintable '\\x%02X' character",
                deeper->filename().c_str(),
                line_number,
                c
            );
        }
        ++column;
        break;
    }
}


void
output_text_encode::write_one_line()
{
    //
    // the carriage return is NOT included in the buffer
    //
    unsigned characters_per_block = (nul_guarantee ? 1023 : 1024);
    assert(address < 1024);
    line_accumulator.push_back('\r');
    rcstring line = line_accumulator.mkstr();
    line_accumulator.clear();

    //
    // If the line does not fit in the current block,
    // pad the block with NUL charactres.
    //
    if (address + line.size() > characters_per_block)
    {
        for (;;)
        {
            deeper->fputc(0);
            ++address;
            if ((address & 1023) == 0)
            {
                address = 0;
                break;
            }
        }
    }

    //
    // Cope with exceptionally very long lines.
    //
    if (address == 0)
    {
        while (line.size() > characters_per_block)
        {
            explain_output_error
            (
                "%s: %d: warning: line too long (%ld) split at column %d",
                deeper->filename().c_str(),
                line_number,
                (long)line.size(),
                characters_per_block - 1
            );
            deeper->write(line.c_str(), characters_per_block - 1);
            deeper->fputc('\r');
            if (nul_guarantee)
                deeper->fputc(0);
            line = line.substring(characters_per_block - 1, line.size());
        }
    }

    //
    // Add the line to the current block.
    //
    deeper->write(line.c_str(), line.size());
    address += line.size();
    assert(address > 0);
    assert(address <= characters_per_block);
    address &= 1023;
    ++line_number;

    column = 0;
    non_white = false;
}


void
output_text_encode::write_inner(const void *vdata, size_t size)
{
    const unsigned char *data = (const unsigned char *)vdata;
    while (size > 0)
    {
        unsigned char c = *data++;
        --size;
        if (c == '\n' || c == '\r')
            write_one_line();
        else
            line_character(c);
    }
}


void
output_text_encode::flush_inner()
{
    deeper->flush();
}


rcstring
output_text_encode::filename()
{
    return deeper->filename();
}


void
output_text_encode::utime(struct utimbuf &utb)
{
    deeper->utime(utb);
}


// vim: set ts=8 sw=4 et :
