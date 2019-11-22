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

#include <ucsdpsys_assemble/expression/boolean/asm.h>


expression_boolean_asm::~expression_boolean_asm()
{
}


expression_boolean_asm::expression_boolean_asm(
    const location &a_locn,
    bool a_value
) :
    expression_boolean(a_locn, a_value)
{
}


expression_boolean_asm::pointer
expression_boolean_asm::create(const location &a_locn, bool a_value)
{
    return pointer(new expression_boolean_asm(a_locn, a_value));
}


void
expression_boolean_asm::traversal(int)
    const
{
    assert(!"not used");
}


expression::pointer
expression_boolean_asm::optimize(void)
    const
{
    return create(get_location(), get_boolean_value());
}


expression::pointer
expression_boolean_asm::clone(const location &where)
    const
{
    return create(where, get_boolean_value());
}


// vim: set ts=8 sw=4 et :
