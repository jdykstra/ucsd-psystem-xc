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

#include <lib/expression/list.h>

#include <ucsdpsys_assemble/expression/range_check.h>
#include <ucsdpsys_assemble/opcode/6502/smb.h>


opcode_6502_smb::~opcode_6502_smb()
{
}


opcode_6502_smb::opcode_6502_smb(assembler *a_context, model_t a_model) :
    opcode_6502(a_context, a_model)
{
}


opcode_6502_smb::pointer
opcode_6502_smb::create(assembler *a_context, model_t a_model)
{
    return pointer(new opcode_6502_smb(a_context, a_model));
}


void
opcode_6502_smb::interpret(const expression_list &args)
{
    if (!check_argument_count(args, 2))
        return;
    model_warning(model_65c02s);

    if (!check_argument_const_int(1, args[0], 0, 7))
        return;
    int bit = args[0]->optimize()->get_integer_value();

    emit_byte(0x87 + (bit << 4));
    emit_byte(expression_range_check::create(args[1], 0, 255));
}


rcstring
opcode_6502_smb::get_name(void)
    const
{
    return "smb";
}


// vim: set ts=8 sw=4 et :
