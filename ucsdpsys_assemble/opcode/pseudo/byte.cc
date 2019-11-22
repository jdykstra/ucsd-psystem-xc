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
#include <ucsdpsys_assemble/opcode/pseudo/byte.h>


opcode_pseudo_byte::~opcode_pseudo_byte()
{
}


opcode_pseudo_byte::opcode_pseudo_byte(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_byte::pointer
opcode_pseudo_byte::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_byte(a_context));
}


void
opcode_pseudo_byte::interpret(const expression_list &args)
{
    if (args.size() < 1)
    {
        lex_error(args.get_location(), "requires at least one argument");
        return;
    }
    for (size_t j = 0; j < args.size(); ++j)
    {
        expression::pointer ep = args[j];
        if (ep->is_error())
        {
            // already complained about
            emit_byte(0);
            continue;
        }
        if (!ep->is_constant())
        {
            lex_error
            (
                ep->get_location(),
                "argument %s: not constant",
                cardinal(j + 1).c_str()
            );
            emit_byte(0);
        }
        else
        {
            long n = ep->optimize()->get_integer_value();
            if (n < -128 || n >= 256)
            {
                lex_error
                (
                    ep->get_location(),
                    "argument %s: value (%ld) not in range 0..255",
                    cardinal(j + 1).c_str(),
                    n
                );
            }
            emit_byte(n);
        }
    }
}


rcstring
opcode_pseudo_byte::get_name()
    const
{
    return ".byte";
}


// vim: set ts=8 sw=4 et :
