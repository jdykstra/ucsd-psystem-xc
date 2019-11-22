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

#include <lib/cardinal.h>
#include <lib/expression/list.h>

#include <ucsdpsys_assemble/expression/range_check.h>
#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/6502/jsr.h>


opcode_6502_jsr::~opcode_6502_jsr()
{
}


opcode_6502_jsr::opcode_6502_jsr(assembler *a_context) :
    opcode_6502(a_context)
{
}


opcode_6502_jsr::pointer
opcode_6502_jsr::create(assembler *a_context)
{
    return pointer(new opcode_6502_jsr(a_context));
}


void
opcode_6502_jsr::interpret(const expression_list &args)
{
    if (!mode_is_absolute(args, 0x20))
    {
        if (!check_argument_count(args, 1))
            return;
        lex_error
        (
            args[0]->get_location(),
            "parameter %s: absolute address expected, not a %s",
            cardinal(1).c_str(),
            args[0]->get_description().c_str()
        );
    }
}


rcstring
opcode_6502_jsr::get_name(void)
    const
{
    return "jsr";
}



opcode::kind_t
opcode_6502_jsr::get_kind(void)
    const
{
    return kind_6502;
}


// vim: set ts=8 sw=4 et :
