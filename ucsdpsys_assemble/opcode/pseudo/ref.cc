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

#include <ucsdpsys_assemble/expression/name/asm.h>
#include <ucsdpsys_assemble/expression/linker_reference.h>
#include <ucsdpsys_assemble/symbol/expression/asm.h>
#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/pseudo/ref.h>
#include <ucsdpsys_assemble/type/linksym_relative.h>


opcode_pseudo_ref::~opcode_pseudo_ref()
{
}


opcode_pseudo_ref::opcode_pseudo_ref(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_ref::pointer
opcode_pseudo_ref::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_ref(a_context));
}


void
opcode_pseudo_ref::interpret(const expression_list &args)
{
    for (size_t j = 0; j < args.size(); ++j)
    {
        expression::pointer ep1 = args[j];
        assert(!ep1->is_error());

        expression_name_asm::pointer ep2 =
            boost::dynamic_pointer_cast<expression_name_asm>(ep1);
        if (!ep2)
        {
            yuck:
            lex_error
            (
                ep1->get_location(),
                "need a label name, not a %s",
                ep1->get_description().c_str()
            );
            continue;
        }

        // it is a user-defined symbol of some sort
        symbol::pointer sp1 = ep2->get_sp();
        assert(sp1);
        symbol_expression_asm::pointer sp2 =
            boost::dynamic_pointer_cast<symbol_expression_asm>(sp1);
        if (!sp2)
            goto yuck;

        expression::pointer value = sp2->get_value();
        if (value)
        {
            lex_error
            (
                ep1->get_location(),
                "pseudo-op .ref: parameter %s: symbol %s is already defined...",
                cardinal(j + 1).c_str(),
                sp1->get_name().quote_c().c_str()
            );
            lex_error
            (
                sp2->get_declaration_location(),
                "... here is the previois %s definition",
                sp2->get_name().quote_c().c_str()
            );
            continue;
        }

        // now we can build its value
        value =
            expression_linker_reference::create
            (
                ep1->get_location(),
                GLOBREF,
                type_linksym_relative::create(sp2->get_name()),
                0,
                get_context()
            );
        sp2->set_value(value);
    }
}


rcstring
opcode_pseudo_ref::get_name(void)
    const
{
    return ".ref";
}


opcode::kind_t
opcode_pseudo_ref::get_kind(void)
    const
{
    return kind_name_list;
}


// vim: set ts=8 sw=4 et :
