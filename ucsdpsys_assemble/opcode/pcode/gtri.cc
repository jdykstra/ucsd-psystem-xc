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

#include <ucsdpsys_assemble/opcode/pcode/gtri.h>


opcode_pcode_gtri::~opcode_pcode_gtri()
{
}


opcode_pcode_gtri::opcode_pcode_gtri(assembler *a_context) :
    opcode_pcode(a_context)
{
}


opcode_pcode_gtri::pointer
opcode_pcode_gtri::create(assembler *a_context)
{
    return pointer(new opcode_pcode_gtri(a_context));
}


void
opcode_pcode_gtri::interpret(const expression_list &args)
{
    if (!check_argument_count(args, 0))
        return;

    emit_byte(GTRI);
}


rcstring
opcode_pcode_gtri::get_name(void)
    const
{
    return "gtri";
}


// vim: set ts=8 sw=4 et :
