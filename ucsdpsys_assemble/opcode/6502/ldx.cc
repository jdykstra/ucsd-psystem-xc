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

#include <ucsdpsys_assemble/opcode/6502/ldx.h>


opcode_6502_ldx::~opcode_6502_ldx()
{
}


opcode_6502_ldx::opcode_6502_ldx(assembler *a_context) :
    opcode_6502(a_context)
{
}


opcode_6502_ldx::pointer
opcode_6502_ldx::create(assembler *a_context)
{
    return pointer(new opcode_6502_ldx(a_context));
}


void
opcode_6502_ldx::interpret(const expression_list &args)
{
    if
    (
        !mode_is_zeropage(args, 0xA6)
    &&
        !mode_is_zeropage_indexed_y(args, 0xB6)
    &&
        !mode_is_absolute(args, 0xAE)
    &&
        !mode_is_absolute_indexed_y(args, 0xBE)
    &&
        !mode_is_immediate(args, 0xA2)
    )
        argument_list_error(args);
}


rcstring
opcode_6502_ldx::get_name(void)
    const
{
    return "ldx";
}


opcode::kind_t
opcode_6502_ldx::get_kind(void)
    const
{
    return kind_6502;
}


// vim: set ts=8 sw=4 et :
