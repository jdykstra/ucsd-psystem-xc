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

#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/pcode/sldc.h>


opcode_pcode_sldc::~opcode_pcode_sldc()
{
}


opcode_pcode_sldc::opcode_pcode_sldc(assembler *a_context) :
    opcode_pcode(a_context)
{
}


opcode_pcode_sldc::pointer
opcode_pcode_sldc::create(assembler *a_context)
{
    return pointer(new opcode_pcode_sldc(a_context));
}


rcstring
opcode_pcode_sldc::get_name(void)
    const
{
    return "sldc";
}


void
opcode_pcode_sldc::interpret(const expression_list &args)
{
    if (args.size() != 1)
    {
        lex_error
        (
            args.get_location(),
            "the sldc opcode requires one argument, not %s",
            cardinal(args.size()).c_str()
        );
        return;
    }
    if (!args[0]->is_constant())
    {
        lex_error(args.get_location(), "argument not constant");
        return;
    }
    int n = args[0]->optimize()->get_integer_value();
    if (n < 0 || n >= 128)
    {
        lex_error(args.get_location(), "argument (%d) not in range 0..127", n);
        return;
    }
    emit_byte(n);
}


// vim: set ts=8 sw=4 et :
