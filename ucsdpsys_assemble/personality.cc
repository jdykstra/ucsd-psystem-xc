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

#include <ucsdpsys_assemble/personality.h>


personality::~personality()
{
}


personality::personality()
{
}


personality::personality(const personality &arg) :
    opcodes(arg.opcodes)
{
}


void
personality::register_opcode(const opcode::pointer &op)
{
    register_opcode(op->get_name(), op);
}


void
personality::register_opcode(const rcstring &name, const opcode::pointer &op)
{
    opcodes.assign(name.downcase(), op);
}


opcode::pointer
personality::lookup_opcode(const rcstring &name)
    const
{
    return opcodes.lookup(name.downcase());
}


void
personality::register_special(const expression_special::pointer &ep)
{
    specials.assign(ep->get_name().downcase(), ep);
}


expression::pointer
personality::lookup_special(const rcstring &name)
    const
{
    return specials.lookup(name.downcase());
}


byte_sex_t
personality::get_byte_sex(void)
    const
{
    return byte_sex_from_mtype(get_mtype());
}


int
personality::get_default_radix(void)
    const
{
    return 16;
}


// vim: set ts=8 sw=4 et :
