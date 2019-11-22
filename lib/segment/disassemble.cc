//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#include <lib/codefile.h>
#include <lib/disassembler.h>
#include <lib/output/filter/prefix.h>
#include <lib/segment.h>


static int
proc_cmp(const void *va, const void *vb)
{
    const unsigned a = *(const unsigned *)va;
    const unsigned b = *(const unsigned *)vb;
    return (int)(a - b);
}


static void
paddr(output::pointer &os, unsigned address, bool no_addr)
{
    if (no_addr)
        os->fputs("****");
    else
        os->fprintf("%04X", address);
    os->fputc(':');
}


void
segment::disassemble(output::pointer &os, int flags, codefile *cfp)
    const
{
    //
    // Insert the library map at the start of the segment.
    //
    {
        output::pointer pfx = output_filter_prefix::create(os, "; ");
        library_map_columns cols(pfx);
        cols.eoln();
        cols.headings();
        print_library_map(cols);
    }

    bool no_addr = !!(flags & 1);
    unsigned nblocks = (get_code_length() + 511) >> 9;
    unsigned char data[(size_t)1 << 16];
    read_block(0, data, nblocks);

    unsigned segnumprocs = get_word(data + get_code_length() - 2);
    unsigned num_procs = (segnumprocs >> 8) & 0xFF;
    unsigned seg_num = segnumprocs & 0xFF;

    //
    // Build the procedure table, and sort it by code offset, so that we
    // can disassemble in order.
    //
    unsigned proc_dict[256];
    unsigned proc_dict_len = 0;
    for (unsigned pn = 0; pn < num_procs; ++pn)
    {
        unsigned base = get_code_length() - 4 - 2 * pn;
        unsigned value = get_word(data + base);
        unsigned where = (base - value) & 0xFFFF;
        if (!(where & 1) && where >= 6 && where < base)
            proc_dict[proc_dict_len++] = where;
    }
    qsort(proc_dict, proc_dict_len, sizeof(unsigned), proc_cmp);

    // FIXME: How do we customise for other systems?  e.g. Z80
    //        If and when we actually need to cross to compile to them.
    //
    // Guessing: an mtype of mtype_pcode_{b,l}e means there is no assembler
    // in the file.  Otherwise, the pcode endian-ness follows the machine.
    mtype_t default_native_hint = mtype_6502;

    mtype_t pcode_hint = get_mtype();
    mtype_t native_hint = get_mtype();
    switch (get_mtype())
    {
    case mtype_undefined:
        native_hint = default_native_hint;
        pcode_hint = mtype_pcode_from_byte_sex(get_byte_sex());
        break;

    case mtype_pcode_be:
    case mtype_pcode_le:
        native_hint = default_native_hint;
        break;

    case mtype_pdp11:
    case mtype_8080:
    case mtype_z80:
    case mtype_ga440:
    case mtype_6502:
    case mtype_6800:
    case mtype_ti9900:
        pcode_hint = mtype_pcode_from_byte_sex(get_byte_sex());
        break;
    }

    discolumns report(os);

    mtype_t cur_mtype = pcode_hint;
    unsigned address = 0;
    for (unsigned pn = 0; pn < proc_dict_len; ++pn)
    {
        os->fputs(";\n");
        unsigned proc_attr_table_offset = proc_dict[pn];
        unsigned proc_number = data[proc_attr_table_offset];
        mtype_t m_type = pcode_hint;
        bool native = (proc_number == 0);
        if (native)
        {
            for (unsigned k = 0; k < num_procs; ++k)
            {
                unsigned base = get_code_length() - 4 - 2 * k;
                unsigned value = get_word(data + base);
                if (proc_attr_table_offset == base - value)
                {
                    proc_number = k + 1;
                    break;
                }
            }
            assert(proc_number != 0);
            m_type = native_hint;
        }
        dislabel where(rcstring::printf("p%u", proc_number));
        report.full->fprintf("; Procedure %d:", proc_number);
        if (native)
        {
            report.comment_column->fprintf
            (
                "; native %s code",
                mtype_name(native_hint).c_str()
            );
        }
        report.eoln();

        if (m_type != cur_mtype)
        {
            report.opcode_column->fputs(".arch");
            report.argument_column->fputs(mtype_name(m_type).quote_c());
            report.eoln();
            cur_mtype = m_type;

            // Nudge the radix back to sane.
            if (m_type != pcode_hint)
            {
                report.opcode_column->fputs(".radix");
                report.argument_column->fputs("10.");
                report.eoln();
            }
        }

        disassembler::pointer dis =
            (
                native
            ?
                disassembler::factory
                (
                    native_hint,
                    where,
                    report,
                    get_segment_number(),
                    proc_number,
                    address,
                    0
                )
            :
                cfp->native_disassembler_factory
                (
                    pcode_hint,
                    where,
                    report,
                    get_segment_number(),
                    proc_number,
                    address,
                    0
                )
            );
        if (no_addr)
            dis->print_addresses(false);
        dis->process(data, address, proc_attr_table_offset + 2, proc_number);
        address = proc_attr_table_offset + 2;
    }

    //
    // Last of all in the segment, print the procedure dictionary
    //
    os->fputs("\n;\n; Procedure dictionary:\n;\n");
    for (int pn = num_procs - 1; pn >= 0; --pn)
    {
        unsigned base = get_code_length() - 4 - 2 * pn;
        paddr(report.address_column, base, no_addr);
        report.data_column->fprintf("%02X ", data[base]);
        report.data_column->fprintf("%02X", data[base + 1]);
        report.opcode_column->fprintf(".word");
        unsigned value = get_word(data + base);
        unsigned where = (base - value) & 0xFFFF;
        if (!(where & 1) && where >= 6 && where < base)
            report.argument_column->fprintf("$ - p%d_at", pn + 1);
        else
            report.argument_column->fprintf("%d", value);
        report.comment_column->fprintf("; procedure %d", pn + 1);
        if (value == 0)
            report.comment_column->fputs(" external");
        report.eoln();
    }

    // FIXME: byte sex
    paddr(report.address_column, get_code_length() - 2, no_addr);
    report.data_column->fprintf("%02X", seg_num);
    report.opcode_column->fprintf(".byte");
    report.argument_column->fprintf("%d", seg_num);
    report.comment_column->fprintf("; segment number");
    report.eoln();

    paddr(report.address_column, get_code_length() - 1, no_addr);
    report.data_column->fprintf("%02X", num_procs);
    report.opcode_column->fprintf(".byte");
    report.argument_column->fprintf("%d", num_procs);
    report.comment_column->fprintf("; number of procedures");
    report.eoln();

    // Separate each segment from the next.
    // This is what the p-System disassembler does.
    os->fputs
    (
        ";\n"
        "; ------------------------------------------------------------------\n"
    );
}


// vim: set ts=8 sw=4 et :
