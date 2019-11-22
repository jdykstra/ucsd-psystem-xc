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

#include <lib/pcode.h>
#include <lib/expression/list.h>

#include <ucsdpsys_assemble/expression/range_check.h>
#include <ucsdpsys_assemble/opcode/pcode/stm.h>


opcode_pcode_stm::~opcode_pcode_stm()
{
}


opcode_pcode_stm::opcode_pcode_stm(assembler *a_context) :
    opcode_pcode(a_context)
{
}


opcode_pcode_stm::pointer
opcode_pcode_stm::create(assembler *a_context)
{
    return pointer(new opcode_pcode_stm(a_context));
}


void
opcode_pcode_stm::interpret(const expression_list &args)
{
    if (!check_argument_count(args, 1))
        return;
    emit_byte(STM);
    emit_byte(expression_range_check::create(args[0], 0, 255));
}


rcstring
opcode_pcode_stm::get_name(void)
    const
{
    return "stm";
}


// vim: set ts=8 sw=4 et :
