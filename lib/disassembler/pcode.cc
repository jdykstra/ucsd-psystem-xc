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

#include <lib/code_source_map.h>
#include <lib/disassembler/pcode.h>
#include <lib/disassembler/pcode/i_3.h>
#include <lib/disassembler/pcode/i_5.h>
#include <lib/disassembler/pcode/ii_0.h>
#include <lib/disassembler/pcode/ii_1.h>


disassembler_pcode::~disassembler_pcode()
{
}


disassembler_pcode::disassembler_pcode(
    byte_sex_t a_byte_sex,
    dislabel &a_where,
    discolumns &a_report,
    int a_seg_num,
    int a_proc_num,
    int a_proc_base,
    code_source_map *a_listing
) :
    disassembler(a_byte_sex, a_where, a_report, a_seg_num, a_proc_num,
        a_proc_base, a_listing)
{
}


disassembler_pcode::pointer
disassembler_pcode::create(byte_sex_t a_byte_sex, dislabel &a_where,
    discolumns &a_report, int a_seg_num, int a_proc_num, int a_proc_base,
    code_source_map *a_listing, p_machine_t pm)
{
    switch (pm)
    {
    case p_machine_i_3:
        return
            disassembler_pcode_i_3::create
            (
                a_byte_sex,
                a_where,
                a_report,
                a_seg_num,
                a_proc_num,
                a_proc_base,
                a_listing
            );

    case p_machine_i_5:
        return
            disassembler_pcode_i_5::create
            (
                a_byte_sex,
                a_where,
                a_report,
                a_seg_num,
                a_proc_num,
                a_proc_base,
                a_listing
            );

    case p_machine_ii_0:
        return
            disassembler_pcode_ii_0::create
            (
                a_byte_sex,
                a_where,
                a_report,
                a_seg_num,
                a_proc_num,
                a_proc_base,
                a_listing
            );

    case p_machine_ii_1:
        return
            disassembler_pcode_ii_1::create
            (
                a_byte_sex,
                a_where,
                a_report,
                a_seg_num,
                a_proc_num,
                a_proc_base,
                a_listing
            );

    default:
        assert(!"you need to add support for another p-machine");
        return pointer();
    }
}


bool
disassembler_pcode::is_native(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :
