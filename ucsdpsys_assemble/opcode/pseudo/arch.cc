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

#include <lib/expression.h>
#include <lib/expression/list.h>
#include <lib/type/string.h>

#include <ucsdpsys_assemble/assembler.h>
#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/pseudo/arch.h>


opcode_pseudo_arch::~opcode_pseudo_arch()
{
}


opcode_pseudo_arch::opcode_pseudo_arch(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_arch::pointer
opcode_pseudo_arch::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_arch(a_context));
}


void
opcode_pseudo_arch::interpret(const expression_list &args)
{
    if (!check_argument_count(args, 1))
        return;
    expression::pointer ep = args[0];
    if (!ep->is_constant() || !type_string::is_a(ep))
    {
        lex_error
        (
            ep->get_location(),
            "%s: argument one: a constant string is required, not a %s",
            get_name().c_str(),
            ep->get_description().c_str()
        );
        return;
    }
    rcstring name = ep->optimize()->get_string_value();
    name = trim_quotes(name);
    dot_architecture(name);
}


rcstring
opcode_pseudo_arch::get_name(void)
    const
{
    return ".arch";
}


opcode::kind_t
opcode_pseudo_arch::get_kind(void)
    const
{
    return kind_prelude;
}


bool
opcode_pseudo_arch::next_argument_always_a_string(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
