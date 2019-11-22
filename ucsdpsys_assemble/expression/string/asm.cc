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

#include <ucsdpsys_assemble/expression/string/asm.h>


expression_string_asm::~expression_string_asm()
{
}


expression_string_asm::expression_string_asm(
    const location &a_loc,
    const rcstring &a_value
) :
    expression_string(a_loc, a_value)
{
}


expression_string_asm::pointer
expression_string_asm::create(const location &a_loc, const rcstring &a_value)
{
    return pointer(new expression_string_asm(a_loc, a_value));
}


void
expression_string_asm::traversal(int)
    const
{
}


void
expression_string_asm::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
}


expression::pointer
expression_string_asm::optimize()
    const
{
    return create(get_location(), get_string_value());
}


expression::pointer
expression_string_asm::clone(const location &locn)
    const
{
    return create(locn, get_string_value());
}


// vim: set ts=8 sw=4 et :
