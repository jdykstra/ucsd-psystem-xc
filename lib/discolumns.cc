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

#include <lib/discolumns.h>


bool discolumns::long_address_format;


discolumns::~discolumns()
{
}


discolumns::discolumns(const output::pointer &os) :
    col(os)
{
    // There are two alternate address columns
    // 1. a simple 4-digit hexadecimal offset into the segment
    //       XXXX:            (width = 5)
    // 2. segment, proc and offset, all in decimal
    //       s63 p99 o999:    (width = 13)
    //    This is the same format as used by ucsdpsys_vm --trace
    //    Using this format makes debugging easier.

    int left = 0;
    full = col.create_new_column(left, 0);
    int addr_width = (long_address_format ? 13 : 5);
    address_column = col.create_new_column(left, left + addr_width);
    left += addr_width + 1;
    data_column = col.create_new_column(left, left + 16);
    left += 18;
    source_column = col.create_new_column(left, 0);
    label_column = col.create_new_column(left, left + 7);
    left += 8;
    opcode_column = col.create_new_column(left, left + 7);
    left += 8;
    argument_column = col.create_new_column(left, 0);
    left += 16;
    comment_column = col.create_new_column(left, 0);

    left = 0;
    sym_location_column = col.create_new_column(left, left + 23);
    left += 24;
    sym_address_column = col.create_new_column(left, left + 5);
    left += 6;
    sym_size_column = col.create_new_column(left, left + 4);
    left += 5;
    sym_kind_column = col.create_new_column(left, left + 11);
    left += 12;
    sym_name_column = col.create_new_column(left, left + 9);
    left += 10;
    sym_type_column = col.create_new_column(left, 0);
}


void
discolumns::eoln(void)
{
    col.eoln();
}


// vim: set ts=8 sw=4 et :
