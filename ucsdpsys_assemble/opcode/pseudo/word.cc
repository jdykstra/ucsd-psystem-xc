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
#include <ucsdpsys_assemble/opcode/pseudo/word.h>


opcode_pseudo_word::~opcode_pseudo_word()
{
}


opcode_pseudo_word::opcode_pseudo_word(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_word::pointer
opcode_pseudo_word::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_word(a_context));
}


void
opcode_pseudo_word::interpret(const expression_list &args)
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
            // Already complained about.
            emit_word(0);
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
            emit_word(0);
        }
        else
        {
            long n = ep->optimize()->get_integer_value();
            if (n < -32768 || n >= 32768)
            {
                lex_error
                (
                    ep->get_location(),
                    "argument %s: value (%ld) not in range 0..32767",
                    cardinal(j + 1).c_str(),
                    n
                );
            }
            emit_word(n);
        }
    }
}


rcstring
opcode_pseudo_word::get_name(void)
    const
{
    return ".word";
}


// vim: set ts=8 sw=4 et :
