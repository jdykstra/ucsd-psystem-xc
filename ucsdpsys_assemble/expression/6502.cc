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

#include <lib/type/integer.h>

#include <ucsdpsys_assemble/expression/6502.h>


expression_6502::~expression_6502()
{
}


expression_6502::expression_6502(const location &locn) :
    expression(locn)
{
}


void
expression_6502::traversal(int)
    const
{
    assert(false);
}


void
expression_6502::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(false);
}


int
expression_6502::get_precedence(void)
    const
{
    return 0;
}


type::pointer
expression_6502::get_type(void)
    const
{
    return type_integer::create();
}


bool
expression_6502::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_6502::has_side_effect(void)
    const
{
    return side_effect_no;
}


// vim: set ts=8 sw=4 et :
