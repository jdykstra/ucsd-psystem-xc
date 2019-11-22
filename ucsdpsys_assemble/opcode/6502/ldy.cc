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

#include <ucsdpsys_assemble/opcode/6502/ldy.h>


opcode_6502_ldy::~opcode_6502_ldy()
{
}


opcode_6502_ldy::opcode_6502_ldy(assembler *a_context) :
    opcode_6502(a_context)
{
}


opcode_6502_ldy::pointer
opcode_6502_ldy::create(assembler *a_context)
{
    return pointer(new opcode_6502_ldy(a_context));
}


void
opcode_6502_ldy::interpret(const expression_list &args)
{
    if
    (
        !mode_is_zeropage(args, 0xA4)
    &&
        !mode_is_zeropage_indexed_x(args, 0xB4)
    &&
        !mode_is_absolute(args, 0xAC)
    &&
        !mode_is_absolute_indexed_x(args, 0xBC)
    &&
        !mode_is_immediate(args, 0xA0)
    )
        argument_list_error(args);
}


rcstring
opcode_6502_ldy::get_name(void)
    const
{
    return "ldy";
}


opcode::kind_t
opcode_6502_ldy::get_kind(void)
    const
{
    return kind_6502;
}


// vim: set ts=8 sw=4 et :
