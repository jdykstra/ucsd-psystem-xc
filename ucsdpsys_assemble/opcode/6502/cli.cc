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

#include <ucsdpsys_assemble/opcode/6502/cli.h>


opcode_6502_cli::~opcode_6502_cli()
{
}


opcode_6502_cli::opcode_6502_cli(assembler *a_context) :
    opcode_6502(a_context)
{
}


opcode_6502_cli::pointer
opcode_6502_cli::create(assembler *a_context)
{
    return pointer(new opcode_6502_cli(a_context));
}


void
opcode_6502_cli::interpret(const expression_list &args)
{
    if (!mode_is_implicit(args, 0x58))
        argument_list_error(args);
}


rcstring
opcode_6502_cli::get_name(void)
    const
{
    return "cli";
}


// vim: set ts=8 sw=4 et :
