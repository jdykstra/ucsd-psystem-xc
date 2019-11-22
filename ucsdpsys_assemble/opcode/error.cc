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

#include <ucsdpsys_assemble/opcode/error.h>


opcode_error::~opcode_error()
{
}


opcode_error::opcode_error(assembler *a_context, const rcstring &a_name) :
    opcode(a_context),
    name(a_name)
{
}


opcode_error::pointer
opcode_error::create(assembler *a_context, const rcstring &a_name)
{
    return pointer(new opcode_error(a_context, a_name));
}


void
opcode_error::interpret(const expression_list &)
{
    // Do nothing (already complained).
}


rcstring
opcode_error::get_name(void)
    const
{
    return name;
}


opcode::kind_t
opcode_error::get_kind(void)
    const
{
    return kind_standard;
}


// vim: set ts=8 sw=4 et :
