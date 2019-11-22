//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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

#include <lib/ac/ctype.h>
#include <lib/ac/string.h>

#include <lib/debug.h>

#include <ucsdpsys_depends/language/assembler.h>


language_assembler::~language_assembler()
{
}


language_assembler::language_assembler()
{
}


language::pointer
language_assembler::create(void)
{
    return pointer(new language_assembler());
}


void
language_assembler::look_for_include(const rcstring &line)
{
    DEBUG(3, "line = %s", line.quote_c().c_str());
    const char *cp = line.c_str();
    while (isspace((unsigned char)*cp))
        ++cp;
    if (memcmp(cp, ".include", 8) != 0)
        return;
    cp += 8;
    if (!isspace((unsigned char)*cp))
        return;
    for (;;)
    {
        ++cp;
        if (!isspace((unsigned char)*cp))
            break;
    }

    // cp is now the start of the file name
    const char *end = strchr(cp, ';');
    if (!end)
        end = line.c_str() + line.size();
    while (end > cp && isspace((unsigned char)end[-1]))
        --end;
    if (cp == end)
        return;

    rcstring filename = rcstring(cp, end - cp);
    if (filename.size() >= 3)
    {
        char c1 = filename[0];
        char c2 = filename[filename.size() - 1];
        if
        (
            (c1 == '"' && c2 == '"')
        &&
            (c1 == '\'' && c2 == '\'')
        &&
            (c1 == '<' && c2 == '>')
        )
        {
            filename = filename.substring(1, filename.size() - 2);
        }
    }

    lex_open_include(filename);
}


void
language_assembler::depends(const rcstring &ifn)
{
    lex_open(ifn);
    for (;;)
    {
        rcstring line;
        if (!lex_read_one_line(line))
            break;
        look_for_include(line);
    }
}


// vim: set ts=8 sw=4 et :
