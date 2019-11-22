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

#include <ucsdpsys_assemble/opcode/pseudo/nolist.h>


opcode_pseudo_nolist::~opcode_pseudo_nolist()
{
}


opcode_pseudo_nolist::opcode_pseudo_nolist(assembler *a_context) :
    opcode_pseudo(a_context)
{
}


opcode_pseudo_nolist::pointer
opcode_pseudo_nolist::create(assembler *a_context)
{
    return pointer(new opcode_pseudo_nolist(a_context));
}


void
opcode_pseudo_nolist::interpret(const expression_list &args)
{
    // Other than checking that it has no parameters, we completely
    // ignore the pseudo-op.  For now, anyways.
    check_argument_count(args, 0);
}


rcstring
opcode_pseudo_nolist::get_name(void)
    const
{
    return ".nolist";
}


opcode::kind_t
opcode_pseudo_nolist::get_kind(void)
    const
{
    return kind_prelude;
}


// vim: set ts=8 sw=4 et :
