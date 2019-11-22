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

#include <lib/ac/stdarg.h>
#include <lib/ac/stdio.h>
#include <lib/ac/string.h>

#include <lib/debug.h>


int debug_level;


void
debug_print(const char *file, int line, const char *fmt, ...)
{
    if (file && line)
        fprintf(stderr, "%s: %d: ", file, line);

    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    size_t len = strlen(fmt);
    if (!len || fmt[len - 1] != '\n')
        fprintf(stderr, "\n");
}


void
debug_hexdump(int lvl, const void *data, unsigned data_size)
{
    if (lvl > debug_level)
        return;
    const unsigned char *cp = (const unsigned char *)data;
    for (unsigned addr = 0; addr < data_size; addr += 16)
    {
        fprintf(stderr, "%04X:", addr);
        unsigned k = 0;
        for (; k < 16 && addr + k < data_size; ++k)
        {
            if (k == 8)
                fprintf(stderr, " ");
            unsigned char c = cp[addr + k];
            fprintf(stderr, " %02X", c);
        }
        for (; k < 16; ++k)
            fprintf(stderr, "   ");
        fprintf(stderr, "  ");
        for (k = 0; k < 16 && addr + k < data_size; ++k)
        {
            if (k == 8)
                fprintf(stderr, " ");
            unsigned char c = cp[addr + k] & 0x7F;
            if (c <= ' ' || c >= 0x7F)
                c = '.';
            fprintf(stderr, "%c", c);
        }
        fprintf(stderr, "  ");
        fprintf(stderr, "\n");
    }
}


// vim: set ts=8 sw=4 et :
