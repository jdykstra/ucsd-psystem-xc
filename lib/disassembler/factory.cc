//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#include <lib/disassembler/6502.h>
#include <lib/disassembler/hexdump.h>
#include <lib/disassembler/pdp11.h>


disassembler::pointer
disassembler::factory(mtype_t mtype, dislabel &a_where, discolumns &a_report,
    int seg_num, int proc_num, int proc_base, code_source_map *listing)
{
    switch (mtype)
    {
    case mtype_undefined:
        // This is what pre-II.0 used for code segments.  And,
        // strangely, this is what the Apple compiler uses for
        // segment 0 of (*$U-*) system programs.
        //
        // Fall through...

    case mtype_pcode_be:
    case mtype_pcode_le:
        assert(!"use the codefile::native_disassembler_factory instead");

    case mtype_pdp11:
        return
            disassembler_pdp11::create
            (
                a_where,
                a_report,
                seg_num,
                proc_num,
                proc_base,
                listing
            );

    case mtype_8080:
    case mtype_z80:
    case mtype_ga440:
        break;

    case mtype_6502:
        return
            disassembler_6502::create
            (
                a_where,
                a_report,
                seg_num,
                proc_num,
                proc_base,
                listing
            );

    case mtype_6800:
    case mtype_ti9900:
        break;
    }
    return
        disassembler_hexdump::create
        (
            a_where,
            a_report,
            seg_num,
            proc_num,
            proc_base,
            listing
        );
}


// vim: set ts=8 sw=4 et :
