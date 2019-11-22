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

#include <lib/code_source_map.h>
#include <lib/debug.h>
#include <lib/disassembler.h>


disassembler::~disassembler()
{
}


disassembler::disassembler(
    byte_sex_t a_byte_sex,
    dislabel &a_where,
    discolumns &a_report,
    int a_segnum,
    int a_proc_num,
    int a_proc_base,
    code_source_map *a_listing
) :
    where(a_where),
    report(a_report),
    byte_sex(a_byte_sex),
    no_addr(false),
    segnum(a_segnum),
    proc_num(a_proc_num),
    proc_base(a_proc_base),
    listing(a_listing)
{
}


void
disassembler::print_addresses(bool yesno)
{
    no_addr = !yesno;
}


unsigned
disassembler::get_word(const unsigned char *data)
    const
{
    return byte_sex_get_word(byte_sex, data);
}


void
disassembler::print_address(unsigned address)
    const
{
    if (listing)
        listing->list_to_here(segnum, address);

    if (no_addr)
    {
        report.address_column->fputs("****");
    }
    else if (report.long_address_format)
    {
        report.address_column->fprintf
        (
            "s%d p%d o%d",
            segnum,
            proc_num,
            address - proc_base
        );
    }
    else
    {
        report.address_column->fprintf("%04X", address);
    }
    report.address_column->fputc(':');

    rcstring lbl = where.get_name(address);
    if (!lbl.empty())
    {
        report.label_column->fputs(lbl);
        report.label_column->fputc(':');
    }
}


void
disassembler::find_reloc(const unsigned char *data, unsigned lo, unsigned &hi,
    unsigned &r) const
{
    r = 0;
    if (lo >= hi)
        return;
    hi -= 2;
    r = hi;
    unsigned n = get_word(data + hi);
    unsigned nbytes = n * 2;
    if (lo + nbytes >= hi)
        hi = lo;
    else
        hi -= nbytes;
}


void
disassembler::find_relocs(const unsigned char *data, unsigned lo, unsigned hi,
    unsigned r[4]) const
{
    find_reloc(data, lo, hi, r[3]);
    find_reloc(data, lo, hi, r[2]);
    find_reloc(data, lo, hi, r[1]);
    find_reloc(data, lo, hi, r[0]);
}


void
disassembler::process(const unsigned char *data, unsigned &address,
    size_t outer_data_size, unsigned procedure_number)
{
    assert(procedure_number >= 1);
    assert(procedure_number < 256);
    unsigned proc_attr_table_offset = outer_data_size - 2;
    where.add(proc_attr_table_offset, "at");
    int lex_level = data[proc_attr_table_offset + 1];
    unsigned enter_ic = proc_attr_table_offset - 2 -
        get_word(data + proc_attr_table_offset - 2);
    where.add(enter_ic);
    unsigned exit_ic = 0;
    unsigned param_size = 0;
    unsigned data_size = 0;

    unsigned proc_attr_start = proc_attr_table_offset - 8;
    if (is_native())
    {
        proc_attr_start += 6;
        unsigned reloc_table_size = 0;

        reloc_table_size += 2;
        if (proc_attr_start >= enter_ic + reloc_table_size)
        {
            int n = get_word(data + proc_attr_start - reloc_table_size);
            DEBUG(3, "n = %d", n);
            reloc_table_size += 2 * n;
        }

        reloc_table_size += 2;
        if (proc_attr_start >= enter_ic + reloc_table_size)
        {
            int n = get_word(data + proc_attr_start - reloc_table_size);
            DEBUG(3, "n = %d", n);
            reloc_table_size += 2 * n;
        }

        reloc_table_size += 2;
        if (proc_attr_start >= enter_ic + reloc_table_size)
        {
            int n = get_word(data + proc_attr_start - reloc_table_size);
            DEBUG(3, "n = %d", n);
            reloc_table_size += 2 * n;
        }

        reloc_table_size += 2;
        if (proc_attr_start >= enter_ic + reloc_table_size)
        {
            int n = get_word(data + proc_attr_start - reloc_table_size);
            DEBUG(3, "n = %d", n);
            reloc_table_size += 2 * n;
        }

        if (proc_attr_start >= enter_ic + reloc_table_size)
            proc_attr_start -= reloc_table_size;
        else
            proc_attr_start = enter_ic;
    }
    else
    {
        exit_ic = proc_attr_table_offset - 4 -
            get_word(data + proc_attr_table_offset - 4);
        where.add(exit_ic);
        param_size = get_word(data + proc_attr_table_offset - 6);
        data_size = get_word(data + proc_attr_table_offset - 8);
    }
    preprocess(data, enter_ic, exit_ic, outer_data_size);

    while (address < enter_ic)
    {
        unsigned char c = data[address++];
        report.data_column->fprintf("%02X", c);
        report.opcode_column->fputs(".byte");
        report.argument_column->fprintf("%u", c);
        report.eoln();
    }
    while (address < proc_attr_start)
    {
        print_address(address);

        unsigned nbytes =
            disassemble_one_instruction
            (
                data,
                address,
                outer_data_size,
                procedure_number
            );
        if (nbytes == 0)
        {
            unsigned char c = data[address++];
            report.data_column->fprintf("%02X", c);
            report.opcode_column->fputs(".byte");
            report.argument_column->fprintf("%u", c);
        }
        else
            address += nbytes;
        report.eoln();
    }

    if (is_native())
    {
        // Relocation data
        report.full->fprintf
        (
            "; Procedure %d Relocation Data:\n",
            procedure_number
        );
        report.eoln();
        unsigned r[4];
        find_relocs(data, address, proc_attr_table_offset - 2, r);
        while (address < proc_attr_table_offset - 2)
        {
            print_address(address);
            report.data_column->fprintf("%02X ", data[address]);
            report.data_column->fprintf("%02X", data[address + 1]);
            report.opcode_column->fputs(".word");
            unsigned n = get_word(data + address);
            report.argument_column->fprintf("%d", n);

            if (address == r[0])
                report.comment_column->fputs("; .interp relative");
            else if (address == r[1])
                report.comment_column->fputs("; pc relative");
            else if (address == r[2])
                report.comment_column->fputs("; .ref relative");
            else if (address == r[3])
                report.comment_column->fputs("; .public relative");

            report.eoln();
            address += 2;
        }
        report.full->fprintf
        (
            "; Procedure %d Attribute Table:\n",
            procedure_number
        );
        report.eoln();
    }
    else
    {
        report.full->fprintf
        (
            "; Procedure %d Attribute Table:\n",
            procedure_number
        );
        report.eoln();

        print_address(proc_attr_table_offset - 8);
        report.data_column->fprintf("%02X ", data[proc_attr_table_offset - 8]);
        report.data_column->fprintf("%02X", data[proc_attr_table_offset - 7]);
        report.opcode_column->fputs(".word");
        report.argument_column->fprintf("%d", data_size);
        report.comment_column->fputs("; data size");
        report.eoln();

        print_address(proc_attr_table_offset - 6);
        report.data_column->fprintf("%02X ", data[proc_attr_table_offset - 6]);
        report.data_column->fprintf("%02X", data[proc_attr_table_offset - 5]);
        report.opcode_column->fputs(".word");
        report.argument_column->fprintf("%d", param_size);
        report.comment_column->fputs("; param size");
        report.eoln();

        print_address(proc_attr_table_offset - 4);
        report.data_column->fprintf("%02X ", data[proc_attr_table_offset - 4]);
        report.data_column->fprintf("%02X", data[proc_attr_table_offset - 3]);
        report.opcode_column->fputs(".word");
        report.argument_column->fputs("$ - ");
        report.argument_column->fputs(where.get_name(exit_ic));
        report.comment_column->fputs("; exit ic");
        report.eoln();
    }

    print_address(proc_attr_table_offset - 2);
    report.data_column->fprintf("%02X ", data[proc_attr_table_offset - 2]);
    report.data_column->fprintf("%02X", data[proc_attr_table_offset - 1]);
    report.opcode_column->fputs(".word");
    report.argument_column->fprintf("$ - ");
    report.argument_column->fputs(where.get_name(enter_ic));
    report.comment_column->fputs("; enter ic");
    report.eoln();

    print_address(proc_attr_table_offset);
    report.data_column->fprintf("%02X", data[proc_attr_table_offset]);
    report.opcode_column->fputs(".byte");
    report.argument_column->fprintf("%d", data[proc_attr_table_offset]);
    if (!data[proc_attr_table_offset])
        report.comment_column->fputs("; native machine code");
    else
        report.comment_column->fputs("; procedure number");
    report.eoln();

    print_address(proc_attr_table_offset + 1);
    report.data_column->fprintf("%02X", data[proc_attr_table_offset + 1]);
    report.opcode_column->fputs(".byte");
    report.argument_column->fprintf("%d", lex_level == 255 ? -1 : lex_level);
    report.comment_column->fputs("; lex level");
    report.eoln();
}


void
disassembler::preprocess(const unsigned char *, unsigned entry_ic,
    unsigned exit_ic, size_t data_size)
{
    assert(entry_ic < data_size);
    assert(exit_ic < data_size);
    assert(entry_ic <= exit_ic);
}


// vim: set ts=8 sw=4 et :
