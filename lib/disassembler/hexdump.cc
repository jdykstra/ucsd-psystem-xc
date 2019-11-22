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

#include <lib/disassembler/hexdump.h>


disassembler_hexdump::~disassembler_hexdump()
{
}


disassembler_hexdump::disassembler_hexdump(
    dislabel &a_where,
    discolumns &a_report,
    int a_segnum,
    int a_proc_num,
    int a_proc_base,
    code_source_map *a_listing
) :
    disassembler(little_endian, a_where, a_report, a_segnum, a_proc_num,
        a_proc_base, a_listing),
    highwater(0)
{
}


disassembler_hexdump::pointer
disassembler_hexdump::create(dislabel &a_where, discolumns &a_report,
    int a_segnum, int a_proc_num, int a_proc_base, code_source_map *a_listing)
{
    return
        pointer
        (
            new disassembler_hexdump
            (
                a_where,
                a_report,
                a_segnum,
                a_proc_num,
                a_proc_base,
                a_listing
            )
        );
}



void
disassembler_hexdump::preprocess(const unsigned char *, unsigned, unsigned,
    size_t data_size)
{
    highwater = data_size - 10;
}


unsigned
disassembler_hexdump::disassemble_one_instruction(const unsigned char *data,
    unsigned address, size_t, int)
{
    unsigned start_address = address;
    report.opcode_column->fputs(".byte");
    report.comment_column->fputs("; ");
    bool first = true;
    for (;;)
    {
        unsigned char c = data[address++];
        if (!first)
        {
            report.data_column->fputc(' ');
            report.argument_column->fputs(", ");
        }
        report.data_column->fprintf("%02X", c);
        report.argument_column->fprintf("%d", c);
        c &= 0x7F;
        report.comment_column->fputc((c <= ' ' || c > '~') ? '.' : c);
        first = false;
        if (address >= highwater)
            break;
        if ((address & 3) == 0)
            break;
        if (!where.get_name(address).empty())
            break;
    }
    return (address - start_address);
}


bool
disassembler_hexdump::is_native(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
