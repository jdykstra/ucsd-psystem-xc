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
#include <lib/type/string.h>

#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/pseudo/include.h>


opcode_pseudo_include::~opcode_pseudo_include()
{
}


opcode_pseudo_include::opcode_pseudo_include(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_include::pointer
opcode_pseudo_include::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_include(a_context));
}


void
opcode_pseudo_include::interpret(const expression_list &args)
{
    if (!check_argument_count(args, 1))
        return;
    expression::pointer ep = args[0];
    if (type_string::is_a(ep))
    {
        rcstring fn = ep->get_string_value();
        fn = trim_quotes(fn.trim());
        lex_open_include(fn);
    }
    else
    {
        grammar_error
        (
            "expected a string, not a %s",
            ep->get_description().c_str()
        );
    }
}


rcstring
opcode_pseudo_include::get_name(void)
    const
{
    return ".include";
}


opcode::kind_t
opcode_pseudo_include::get_kind(void)
    const
{
    return kind_prelude;
}


bool
opcode_pseudo_include::next_argument_always_a_string(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
