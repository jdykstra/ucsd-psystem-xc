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

#include <lib/library_map_columns.h>


library_map_columns::~library_map_columns()
{
}


library_map_columns::library_map_columns(output::pointer &os) :
    cols(os)
{
    codeaddr = cols.create_new_column(0, 5);
    codeleng = cols.create_new_column(6, 11);
    name = cols.create_new_column(13, 21);
    segkind = cols.create_new_column(23, 31);
    segnum = cols.create_new_column(32, 35);
    mtype = cols.create_new_column(36, 45);
    version = cols.create_new_column(46, 54);
    comments = cols.create_new_column(5, 78);
}


void
library_map_columns::headings(void)
{
    codeaddr->fputs("Code Addr");
    codeleng->fputs("Code Leng");
    name->fputs("Segment Name");
    segkind->fputs("Segment Kind");
    segnum->fputs("SegNum");
    mtype->fputs("Machine Type");
    version->fputs("System Version");
    eoln();

    codeaddr->fputs("-----");
    codeleng->fputs("-----");
    name->fputs("--------");
    segkind->fputs("--------");
    segnum->fputs("---");
    mtype->fputs("---------");
    version->fputs("--------");
    eoln();
}


void
library_map_columns::eoln(void)
{
    cols.eoln();
}


// vim: set ts=8 sw=4 et :
