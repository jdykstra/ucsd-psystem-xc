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

#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode/pseudo/radix.h>


opcode_pseudo_radix::~opcode_pseudo_radix()
{
}


opcode_pseudo_radix::opcode_pseudo_radix(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_radix::pointer
opcode_pseudo_radix::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_radix(a_context));
}


void
opcode_pseudo_radix::interpret(const expression_list &args)
{
    if (!check_argument_count(args, 1))
        return;
    if (!check_argument_const_int(1, args[0], 2, 36))
        return;
    lex_default_radix_set(args[0]->optimize()->get_integer_value());
}


rcstring
opcode_pseudo_radix::get_name(void)
    const
{
    return ".radix";
}


opcode::kind_t
opcode_pseudo_radix::get_kind(void)
    const
{
    return kind_prelude;
}


// vim: set ts=8 sw=4 et :
