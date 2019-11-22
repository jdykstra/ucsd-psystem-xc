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
#include <lib/pcode.h>
#include <lib/type/integer.h>

#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/pcode/rbp.h>


opcode_pcode_rbp::~opcode_pcode_rbp()
{
}


opcode_pcode_rbp::opcode_pcode_rbp(assembler *a_context) :
    opcode_pcode(a_context)
{
}


opcode_pcode_rbp::pointer
opcode_pcode_rbp::create(assembler *a_context)
{
    return pointer(new opcode_pcode_rbp(a_context));
}


void
opcode_pcode_rbp::interpret(const expression_list &args)
{
    if (!check_argument_count(args, 1))
        return;

    expression::pointer ep = args[0];
    if (!ep->is_constant() || !type_integer::is_a(ep))
    {
        lex_error
        (
            ep->get_location(),
            "%s: argument one: expected a constant integer value, not a %s",
            get_name().c_str(),
            ep->get_description().c_str()
        );
        return;
    }
    long n = ep->optimize()->get_integer_value();
    if (n < 0 || n >= 256)
    {
        lex_error
        (
            ep->get_location(),
            "%s: argument one: value (%ld) out of range 0..255",
            get_name().c_str(),
            n
        );
        return;
    }

    emit_byte(RBP);
    emit_byte(n);
}


rcstring
opcode_pcode_rbp::get_name(void)
    const
{
    return "rbp";
}


// vim: set ts=8 sw=4 et :
