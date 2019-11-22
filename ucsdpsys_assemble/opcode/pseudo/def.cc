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

#include <ucsdpsys_assemble/expression/name/asm.h>
#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/pseudo/def.h>
#include <ucsdpsys_assemble/symbol/expression/asm.h>


opcode_pseudo_def::~opcode_pseudo_def()
{
}


opcode_pseudo_def::opcode_pseudo_def(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_def::pointer
opcode_pseudo_def::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_def(a_context));
}


void
opcode_pseudo_def::interpret(const expression_list &args)
{
    if (args.empty())
    {
        check_argument_count(args, 1);
        return;
    }

    for (size_t j = 0; j < args.size(); ++j)
    {
        expression::pointer ep1 = args[j];
        if (ep1->is_error())
            continue;
        expression_name_asm::pointer ep2 =
            boost::dynamic_pointer_cast<expression_name_asm>(ep1);
        if (ep2)
        {
            symbol::pointer sp1 = ep2->get_sp();
            assert(sp1);
            symbol_expression_asm::pointer sp2 =
                boost::dynamic_pointer_cast<symbol_expression_asm>(sp1);
            if (sp2)
            {
                sp2->export_def();
            }
            else
            {
                lex_error
                (
                    ep1->get_location(),
                    "cannot export %s %s",
                    sp1->get_kind(),
                    sp1->get_name().quote_c().c_str()
                );
            }
        }
        else
        {
            lex_error
            (
                ep1->get_location(),
                "need label name, not a %s",
                ep1->get_description().c_str()
            );
        }
    }
}


rcstring
opcode_pseudo_def::get_name(void)
    const
{
    return ".def";
}


// vim: set ts=8 sw=4 et :
