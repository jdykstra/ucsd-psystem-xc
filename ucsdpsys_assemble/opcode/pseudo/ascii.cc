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
#include <ucsdpsys_assemble/opcode/pseudo/ascii.h>


opcode_pseudo_ascii::~opcode_pseudo_ascii()
{
}


opcode_pseudo_ascii::opcode_pseudo_ascii(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_ascii::pointer
opcode_pseudo_ascii::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_ascii(a_context));
}


void
opcode_pseudo_ascii::interpret(const expression_list &args)
{
    if (!check_argument_count(args, 1))
        return;
    if (!type_string::is_a(args[0]))
    {
        lex_error
        (
            args[0]->get_location(),
            "opcode \".ascii\": parameter one: require a string value, "
                "not a %s",
            args[0]->get_description().c_str()
        );
        return;
    }
    rcstring value = args[0]->optimize()->get_string_value();
    for (size_t j = 0; j < value.size(); ++j)
        emit_byte(value[j]);
}


rcstring
opcode_pseudo_ascii::get_name(void)
    const
{
    return ".ascii";
}


// vim: set ts=8 sw=4 et :
