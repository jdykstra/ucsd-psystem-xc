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

#include <ucsdpsys_assemble/expression/6502/absolute.h>


expression_6502_absolute::~expression_6502_absolute()
{
}


expression_6502_absolute::expression_6502_absolute(
    const expression::pointer &a_address
) :
    expression_6502(a_address->get_location()),
    address(a_address)
{
}


expression_6502_absolute::pointer
expression_6502_absolute::create(const expression::pointer &a_address)
{
    return pointer(new expression_6502_absolute(a_address));
}


expression::pointer
expression_6502_absolute::optimize(void)
    const
{
    return create(address->optimize());
}


rcstring
expression_6502_absolute::lisp_representation(void)
    const
{
    return ("(6502-absolute " + address->lisp_representation() + ")");
}


expression::pointer
expression_6502_absolute::clone(const location &locn)
    const
{
    return create(address->clone(locn));
}


// vim: set ts=8 sw=4 et :
