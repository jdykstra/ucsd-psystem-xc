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
#include <lib/type/boolean.h>
#include <lib/type/integer.h>
#include <lib/type/string.h>

#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/pseudo/print.h>


opcode_pseudo_print::~opcode_pseudo_print()
{
}


opcode_pseudo_print::opcode_pseudo_print(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_print::pointer
opcode_pseudo_print::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_print(a_context));
}


void
opcode_pseudo_print::interpret(const expression_list &args)
{
    if (args.empty())
    {
        check_argument_count(args, 1);
        return;
    }
    rcstring text;
    for (size_t j = 0; j < args.size(); ++j)
    {
        expression::pointer ep = args[j];
        ep = ep->optimize();
        if (!ep->is_constant())
        {
            yuck:
            lex_error
            (
                ep->get_location(),
                "%s: argument %s: a constant string is required, not a %s",
                get_name().c_str(),
                cardinal(j + 1).c_str(),
                ep->get_description().c_str()
            );
            return;
        }
        if
        (
            !type_string::is_a(ep)
        &&
            !type_integer::is_a(ep)
        &&
            !type_boolean::is_a(ep)
        )
            goto yuck;
        text += ep->get_printable_value();
    }
    lex_message(args.get_location(), "%s", text.c_str());
}


rcstring
opcode_pseudo_print::get_name(void)
    const
{
    return ".print";
}


opcode::kind_t
opcode_pseudo_print::get_kind(void)
    const
{
    return kind_prelude;
}


// vim: set ts=8 sw=4 et :
