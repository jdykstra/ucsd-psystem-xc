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

#include <ucsdpsys_assemble/expression/integer/asm.h>


expression_integer_asm::~expression_integer_asm()
{
}


expression_integer_asm::expression_integer_asm(
    long a_value,
    const location &a_loc
) :
    expression_integer(a_value, a_loc)
{
}


expression_integer_asm::pointer
expression_integer_asm::create(long a_value, const location &a_loc)
{
    return pointer(new expression_integer_asm(a_value, a_loc));
}


void
expression_integer_asm::traversal(int)
    const
{
}


void
expression_integer_asm::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
}


expression::pointer
expression_integer_asm::optimize()
    const
{
    return create(get_integer_value(), get_location());
}


expression::pointer
expression_integer_asm::clone(const location &locn)
    const
{
    return create(get_integer_value(), locn);
}


// vim: set ts=8 sw=4 et :
